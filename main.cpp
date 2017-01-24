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
#define PARAM_OPENING_SIZE "openingsize"
#define PARAM_CLOSING_SIZE "closingsize"
#define PARAM_ERODE_SIZE "erodesize"
#define PARAM_DILATE_SIZE "dilatesize"
#define PARAM_PROBABLY_FOREGROUND "prbfgb"
#define PARAM_THRESHOLD "threshold"
#define PARAM_SUBSTRACT_MODE "substractmode"
#define PARAM_HSV_CHANNEL "hsvchannel"

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
bool prbfgb;

//Imagenes 640x480
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
			(PARAM_OPENING_SIZE, boost::program_options::value<int>()->default_value(KOPENINGSIZE), "Opening to clean mask")
			(PARAM_CLOSING_SIZE, boost::program_options::value<int>()->default_value(KCLOSINGSIZE), "Closing to close figure")
			(PARAM_ERODE_SIZE, boost::program_options::value<int>()->default_value(KERODESIZE), "Erode to get foreground of grabcut")
			(PARAM_DILATE_SIZE, boost::program_options::value<int>()->default_value(KDILATESIZE), "Dilate to get background of grabcut")
			(PARAM_PROBABLY_FOREGROUND, boost::program_options::value<bool>()->default_value(false), "Base mask of grabcut. 1 = Probably foreground and 0 = Probably background")
			(PARAM_BACKGROUND, boost::program_options::value<std::string>()->required(), "Image background")
			(PARAM_THRESHOLD, boost::program_options::value<int>()->default_value(25), "Threshold tu substract object")
			(PARAM_SUBSTRACT_MODE, boost::program_options::value<int>()->default_value(0), "0 = Substract with gray scale images. 1 = Susbtract in channel 'H' converting RGB images in HSV")
			(PARAM_HSV_CHANNEL, boost::program_options::value<int>()->default_value(2), "Channel H=0,S=1,V=2 to substract object")
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
	openingSize = variablesMap[PARAM_OPENING_SIZE].as<int>();
	closingSize = variablesMap[PARAM_CLOSING_SIZE].as<int>();
	erodeSize = variablesMap[PARAM_ERODE_SIZE].as<int>();
	dilateSize = variablesMap[PARAM_DILATE_SIZE].as<int>();
	prbfgb =variablesMap[PARAM_PROBABLY_FOREGROUND].as<bool>();
	int threshold =variablesMap[PARAM_THRESHOLD].as<int>();
    int substractmode = variablesMap[PARAM_SUBSTRACT_MODE].as<int>();
    int hsvchannel = variablesMap[PARAM_HSV_CHANNEL].as<int>();

	maskApplied=false;
	mObject = cv::imread(sObject);
	cv::Mat mBackground = cv::imread(sBackground);
	cv::Size initSize = mObject.size();

	cv::Mat mBackgroundBlured;
	cv::Mat mObjectBlured;

	//Convertimos a escala de grises ***Tests in gray scale***
    if(substractmode == 0){
    	cv::cvtColor(mBackground, mBackgroundBlured, CV_BGR2GRAY);
	    cv::cvtColor(mObject, mObjectBlured, CV_BGR2GRAY);
    }else if(substractmode == 1){
        //Covert to HSV and extract PARAM_HSV_CHANNEL channel
        cv::Mat mBackgroundHSV, mObjectHSV;
        cv::cvtColor(mBackground, mBackgroundHSV, CV_BGR2HSV);
        cv::cvtColor(mObject, mObjectHSV, CV_BGR2HSV);
        std::vector<cv::Mat> chBackground(hsvchannel);
        std::vector<cv::Mat> chObject(hsvchannel);
        split(mBackgroundHSV, chBackground);
        split(mObjectHSV, chObject);
        mBackgroundBlured = chBackground[hsvchannel];
        mObjectBlured = chObject[hsvchannel];
    }

	//Apply gaussian filter to reduce noise
	for (int i = 0; i < gaussianIterations; ++i){
		cv::GaussianBlur(mBackgroundBlured, mBackgroundBlured, cv::Size(5, 5), 1, 1);
		cv::GaussianBlur(mObjectBlured, mObjectBlured, cv::Size(5, 5), 1, 1);
	}

	//Extract the object
	//Convert image to B&W
	//oriDiff = mBackgroundBlured - mObjectBlured;
    //DIFF option 1 - Good results
    if(substractmode == 0){
    	oriDiff = (mBackgroundBlured > (mObjectBlured+threshold));
        oriDiff += oriDiff != (mBackgroundBlured < (mObjectBlured-threshold));
    }else if(substractmode == 1){
        oriDiff = (mBackgroundBlured > (mObjectBlured+threshold));
        oriDiff += oriDiff != (mBackgroundBlured < (mObjectBlured-threshold));
        //oriDiff = oriDiff == false;
    }

	/// Create window
	cv::namedWindow(window_name, CV_WINDOW_NORMAL | CV_WINDOW_OPENGL);
	cv::resizeWindow(window_name, mObject.cols/winRatio, mObject.rows/winRatio);
	//Apply morphology operation. Opening to get object  //Size odd 2n+1
	cv::createTrackbar("Opening to get object. Size: ", window_name, &openingSize, 100, Morphology_OpeningClean);
	cv::createTrackbar("Closing to clean image. Size: ", window_name, &closingSize, 200, Morphology_ClosingForm);
	cv::createTrackbar("Background morphology dilate. Size: ", window_name, &dilateSize, 300, MaskBackground);
	cv::createTrackbar("Foreground morphology erode. Size: ", window_name, &erodeSize, 200, MaskForeground);

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
			std::cout << "Applying Grabcut..." << std::endl;
			ApplyGrabcut();
			std::cout << "Grabcut applied" << std::endl;
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
    cv::Mat auxMaskAplied;
    mObject.copyTo(auxMaskAplied, imgClose);
	//cv::imshow(window_name, imgClose);   
    cv::imshow(window_name, auxMaskAplied);
}

void ApplyGrabcut(){
	//Mask
	grabCutMask = cv::Mat1b::zeros(imgClose.rows, imgClose.cols);
	cv::Mat1b markers(imgClose.rows, imgClose.cols);
	if(prbfgb)
		markers.setTo(cv::GC_PR_FGD);
	else
		markers.setTo(cv::GC_PR_BGD);

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
