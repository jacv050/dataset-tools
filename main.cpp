#include <iostream>
#include <opencv2/opencv.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <vector>

#define PARAM_HELP	"help"
#define PARAM_BACKGROUND "background"
#define PARAM_OBJECT "object"
#define PARAM_OUTPUTIMAGE "outputimage"
#define PARAM_APPLYMASK "applymask"
#define PARAM_GAUSSIAN_ITERATIONS "gaussianiterations"
#define PARAM_RESIZE "resize"

const std::string window_name = "SubstractImgGrabcutGUI";

//Trackbar size
int openingSize;
int closingSize;
cv::Mat1b imgClose, imgOpen, oriDiff;
cv::Mat mObject;
cv::Mat1b mask_fgpf;
cv::Mat1b grabCutMask;

const int KOPENINGSIZE = 5;
const int KCLOSINGSIZE = 51;

void Morphology_OpeningClean( int, void* );
void Morphology_ClosingForm( int, void* );
void Button_Grabcut(int, void*);

bool parse_command_line_options(boost::program_options::variables_map &variables_map, const int &argc, char **argv){

	try{

		boost::program_options::options_description desc("Allowed descriptions");
		desc.add_options()
			(PARAM_HELP, "Produce help message")
			(PARAM_GAUSSIAN_ITERATIONS, boost::program_options::value<int>()->default_value(5),
					"How many times apply gaussian filter")
			(PARAM_BACKGROUND, boost::program_options::value<std::string>()->required(), "Image background")
			(PARAM_OBJECT, boost::program_options::value<std::string>()->required(), "Object to substract")
			(PARAM_APPLYMASK, boost::program_options::value<bool>()->default_value(false), "Apply mask to real image")
			(PARAM_OUTPUTIMAGE, boost::program_options::value<std::string>()->default_value("finalimage.png"),
					"Name of final image(If NOT apply mask, it will save mask file)")
			(PARAM_RESIZE, boost::program_options::value<bool>()->default_value(true), "Resize image");

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

	/// Create window
	cv::namedWindow(window_name, CV_WINDOW_NORMAL | CV_WINDOW_OPENGL);

	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	//Show help message if PARAM_HELP
	boost::program_options::variables_map variablesMap;
	if (parse_command_line_options(variablesMap, argc, argv)){
		return 1;
	}

	//General params
	std::string sBackground = variablesMap[PARAM_BACKGROUND].as<std::string>();
	std::string sObject = variablesMap[PARAM_OBJECT].as<std::string>();
	std::string sOutputimage = variablesMap[PARAM_OUTPUTIMAGE].as<std::string>();
	int gaussianIterations = variablesMap[PARAM_GAUSSIAN_ITERATIONS].as<int>();
	bool bApplyMask = variablesMap[PARAM_APPLYMASK].as<bool>();
	bool bResize = variablesMap[PARAM_RESIZE].as<bool>();

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

	//Apply morphology operation. Opening to get object  //Size odd 2n+1
	cv::createTrackbar("Opening to get object. Size: ", window_name, &openingSize, 10, Morphology_OpeningClean);
	cv::createTrackbar("Closing to clean image. Size: ", window_name, &closingSize, 100, Morphology_ClosingForm);
	cv::createButton("Apply GrabCut", Button_Grabcut, NULL, CV_PUSH_BUTTON, 0);

	//Defualt values

	openingSize = KOPENINGSIZE;
	closingSize = KCLOSINGSIZE;
	Morphology_OpeningClean(0,0);
	Morphology_ClosingForm(0,0);

	cv::waitKey(0);

	return 1;
}

void Morphology_OpeningClean( int, void* ){
	cv::Mat element1 = cv::getStructuringElement(2, cv::Size(openingSize, openingSize),
			cv::Point(ceil(openingSize/2), ceil(openingSize/2)));
	morphologyEx(oriDiff, imgOpen, 2, element1); //Opening to clean noise
	Morphology_ClosingForm(0,0);
	//cv::imshow(window_name, imgOpen);
}

void Morphology_ClosingForm( int, void* ){
	cv::Mat element2 = cv::getStructuringElement(2, cv::Size(closingSize, closingSize),
			cv::Point(ceil(closingSize/2), ceil(closingSize/2)));
	morphologyEx(imgOpen, imgClose, 3, element2); //Closing
	cv::imshow(window_name, imgClose);
}

void Button_Grabcut(int, void*){

	//Mask
	cv::Mat1b markers(imgClose.rows, imgClose.cols);
	markers.setTo(cv::GC_PR_FGD);

	//Mask background
	cv::Mat element3 = cv::getStructuringElement(2, cv::Size(31, 31), cv::Point(16, 16));
	cv::Mat1b background;
	cv::dilate(imgClose, background, element3);
	background = 255 - background;
	markers.setTo(cv::GC_BGD, background);

	//Mask foreground
	cv::Mat1b foreground;
	cv::erode(imgClose, foreground, element3);
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
