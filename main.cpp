#include <iostream>
#include <opencv2/opencv.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <vector>

#define PARAM_HELP	"help"
#define PARAM_BACKGROUND "background"
#define PARAM_OBJECT "object"
#define PARAM_OUTPUTIMAGE "outputimage"
#define PARAM_GAUSSIAN_ITERATIONS "gaussianiterations"
#define PARAM_WINDOW_RATIO "windowratio"

const std::string window_name = "SubstractImgGrabcutGUI";

//Trackbar size
int openingSize,closingSize,erodeSize, dilateSize;
cv::Mat1b imgClose, imgOpen, oriDiff;
cv::Mat mObject;
cv::Mat1b background, foreground;
cv::Mat1b mask_fgpf;
cv::Mat1b grabCutMask;
std::vector<int> compression_params;
bool maskApplied;

const int KOPENINGSIZE = 5;
const int KCLOSINGSIZE = 51;
const int KERODESIZE = 31;
const int KDILATESIZE = 31;

void Morphology_OpeningClean( int, void* );
void Morphology_ClosingForm( int, void* );
void ApplyGrabcut();
void ApplyMask();
void SaveMaskApplied(const std::string&);
void MaskForeground(int, void*);
void MaskBackground(int, void*);

bool parse_command_line_options(boost::program_options::variables_map &variables_map, const int &argc, char **argv){

	try{

		boost::program_options::options_description desc("Allowed descriptions");
		desc.add_options()
			(PARAM_HELP, "Produce help message")
			(PARAM_GAUSSIAN_ITERATIONS, boost::program_options::value<int>()->default_value(5),
					"How many times apply gaussian filter")
			(PARAM_WINDOW_RATIO, boost::program_options::value<int>()->default_value(2), "Ratio of window in first charge")
			(PARAM_BACKGROUND, boost::program_options::value<std::string>()->required(), "Image background")
			(PARAM_OBJECT, boost::program_options::value<std::string>()->required(), "Object to substract")
			(PARAM_OUTPUTIMAGE, boost::program_options::value<std::string>()->default_value("finalimage.png"),
					"Name of final image(If NOT apply mask, it will save mask file)");

		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), variables_map);

		if (variables_map.count(PARAM_HELP)){
			std::cout << desc << "\n";
			return true;
		}

		boost::program_options::notify(variables_map);

	}
	catch (std::exception &e){
		std::cerr << "Error: " << e.what() << "\n";
		return true;
	}

	return false;
}


int main(int argc, char **argv){
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	//Show help message if PARAM_HELP
	boost::program_options::variables_map variablesMap;
	if (parse_command_line_options(variablesMap, argc, argv)){
		return 1;
	}

	//General params
	std::string sImageCut = "imagecut.jpg";
	std::string sBackground = variablesMap[PARAM_BACKGROUND].as<std::string>();
	std::string sObject = variablesMap[PARAM_OBJECT].as<std::string>();
	std::string sOutputimage = variablesMap[PARAM_OUTPUTIMAGE].as<std::string>();
	int gaussianIterations = variablesMap[PARAM_GAUSSIAN_ITERATIONS].as<int>();
	int winRatio = variablesMap[PARAM_WINDOW_RATIO].as<int>();

	maskApplied=false;
	mObject = cv::imread(sObject);
	cv::Mat mBackground = cv::imread(sBackground);
	cv::Size initSize = mObject.size();

	cv::Mat1b mBackgroundBlured;
	cv::Mat1b mObjectBlured;

	//Convertimos a escala de grises
	cv::cvtColor(mBackground, mBackgroundBlured, CV_BGR2GRAY);
	cv::cvtColor(mObject, mObjectBlured, CV_BGR2GRAY);

	//Apply gaussian filter to reduce noise
	for (int i = 0; i < gaussianIterations; ++i){
		cv::GaussianBlur(mBackgroundBlured, mBackgroundBlured, cv::Size(5, 5), 1, 1);
		cv::GaussianBlur(mObjectBlured, mObjectBlured, cv::Size(5, 5), 1, 1);
	}

	//Extract the object
	oriDiff = mBackgroundBlured - mObjectBlured;
	//Convert image to B&W
	cv::threshold(oriDiff, oriDiff, 25, 255, CV_THRESH_BINARY);

	openingSize = KOPENINGSIZE;
	closingSize = KCLOSINGSIZE;
	erodeSize = KERODESIZE;
	dilateSize = KDILATESIZE;

	/// Create window
	cv::namedWindow(window_name, CV_WINDOW_NORMAL | CV_WINDOW_OPENGL);
	cv::resizeWindow(window_name, mObject.cols/winRatio, mObject.rows/winRatio);
	//Apply morphology operation. Opening to get object  //Size odd 2n+1
	cv::createTrackbar("Opening to get object. Size: ", window_name, &openingSize, 10, Morphology_OpeningClean);
	cv::createTrackbar("Closing to clean image. Size: ", window_name, &closingSize, 100, Morphology_ClosingForm);
	cv::createTrackbar("Background morphology dilate. Size: ", window_name, &dilateSize, 100, MaskBackground);
	cv::createTrackbar("Foreground morphology erode. Size: ", window_name, &erodeSize, 100, MaskForeground);

	//Defualt values
	Morphology_OpeningClean(openingSize,0);
	Morphology_ClosingForm(closingSize,0);
	MaskBackground(0,0);
	MaskForeground(0,0);
	cv::imshow(window_name, imgClose);

	std::cout << "Press 's' to save the mask of image with the name of '--outputimage'" << std::endl;
	std::cout << "Press 'g' to apply grabcut" << std::endl;
	std::cout << "Press 'm' to apply mask" << std::endl;
	std::cout << "Press 'a' to save image with mask applied" << std::endl;
	char c;
	while((c=cv::waitKey(0))!='q'){
		if(c=='g' || c=='G'){
			std::cout << "Grabcut applied" << std::endl;
			ApplyGrabcut();
		}else if(c=='s' || c=='S'){
			std::cout << "Mask saved in: " << sOutputimage << std::endl;
			cv::imwrite(sOutputimage, grabCutMask, compression_params);
		}else if(c=='m' || c=='M'){
			std::cout << "Mask applied" << std::endl;
			ApplyMask();
		}else if(c=='a' || c=='A'){
			std::cout << "Save image with mask applied in: " << sImageCut << std::endl;
			SaveMaskApplied(sImageCut);
		}
	}

	return 1;
}

void MaskBackground(int, void*){
	if(dilateSize==0){
		std::cout << "0 is not valid" << std::endl;
		return;
	}
	//Mask background
	cv::Mat element3 = cv::getStructuringElement(2, cv::Size(dilateSize, dilateSize), cv::Point(ceil(dilateSize/2), ceil(dilateSize/2)));
	cv::Mat backgroundApplied;
	cv::dilate(imgClose, background, element3);
	background = 255 - background;
	mObject.copyTo(backgroundApplied, background);
	cv::imshow(window_name, backgroundApplied);
}

void MaskForeground(int, void*){
	if(erodeSize==0){
		std::cout << "0 is not valid" << std::endl;
		return;
	}
	cv::Mat element3 = cv::getStructuringElement(2, cv::Size(erodeSize, erodeSize), cv::Point(ceil(erodeSize/2), ceil(erodeSize/2)));
	//Mask foreground
	cv::Mat foregroundApplied;
	cv::erode(imgClose, foreground, element3);
	mObject.copyTo(foregroundApplied, foreground);
	cv::imshow(window_name, foregroundApplied);
}

void Morphology_OpeningClean( int, void* ){
	if(openingSize==0){
		std::cout << "0 is not valid" << std::endl;
		return;
	}
	cv::Mat element1 = cv::getStructuringElement(2, cv::Size(openingSize, openingSize),
			cv::Point(ceil(openingSize/2), ceil(openingSize/2)));
	morphologyEx(oriDiff, imgOpen, 2, element1); //Opening to clean noise
	Morphology_ClosingForm(0,0);
	//cv::imshow(window_name, imgOpen);
}

void Morphology_ClosingForm( int, void* ){
	if(closingSize==0){
		std::cout << "0 is not valid" << std::endl;
		return;
	}
	cv::Mat element2 = cv::getStructuringElement(2, cv::Size(closingSize, closingSize),
			cv::Point(ceil(closingSize/2), ceil(closingSize/2)));
	morphologyEx(imgOpen, imgClose, 3, element2); //Closing
	cv::imshow(window_name, imgClose);
}

void ApplyGrabcut(){
	//Mask
	cv::Mat1b markers(imgClose.rows, imgClose.cols);
	markers.setTo(cv::GC_PR_FGD);

	//Mask background
	markers.setTo(cv::GC_BGD, background);

	//Mask foreground
	markers.setTo(cv::GC_FGD, foreground);

	//Apply grabcut
	cv::Mat bgd, fgd;
	int iterations = 1;
	cv::grabCut(mObject, markers, cv::Rect(), bgd, fgd, iterations, cv::GC_INIT_WITH_MASK);

	mask_fgpf = (markers == cv::GC_FGD) | (markers == cv::GC_PR_FGD);
	// and copy all the foreground-pixels to a temporary image

	cv::Mat1b mBWObject = cv::Mat1b::ones(mObject.rows, mObject.cols) * 255;
	mBWObject.copyTo(grabCutMask, mask_fgpf);
	cv::imshow( window_name, grabCutMask);
}

void ApplyMask(){
	maskApplied=true;
	cv::Mat maskedObject;
	mObject.copyTo(maskedObject, mask_fgpf);
	cv::imshow(window_name, maskedObject);
}

void SaveMaskApplied(const std::string& name){
	cv::Mat maskedObject;
	if(!maskApplied)
		ApplyGrabcut();
	mObject.copyTo(maskedObject, mask_fgpf);
	cv::imwrite(name, maskedObject, compression_params);
}
