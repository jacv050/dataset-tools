/*
 * Grabber.cpp
 *
 *  Created on: 09/09/2016
 *      Author: john
 */

#include "Grabber.h"
#include <iostream>
#include <fstream>
#include <sstream>

/*#include <pcl/io/image.h>
#include <pcl/io/image_rgb24.h>
#include <pcl/io/image_yuv422.h>
#include <pcl/io/image_depth.h>
#include <pcl/io/image_ir.h>*/

Grabber::Grabber(const std::string& dir) : viewer("PCL OpenNI Viewer") {
		thread = new boost::thread(boost::bind(&Grabber::keyListener, this));
		dir_ = dir;

		//Directory with scene captures
		dDepth = dir_ + "/depth_images";
		boost::filesystem::path dirDepth(dDepth.c_str());
		dColor = dir_ + "/color_images";
		boost::filesystem::path dirColor(dColor.c_str());
		dPCD = dir_ + "/original_clouds/pcd";
		boost::filesystem::path dirPCD(dPCD.c_str());
		dPLY = dir_ + "/original_clouds/ply";
		boost::filesystem::path dirPLY(dPLY.c_str());
		if(boost::filesystem::create_directories(dirDepth)){
			std::cout << "Depth directory generated." << "\n";
		}
		if(boost::filesystem::create_directories(dirColor)){
			std::cout << "Color directory generated." << "\n";
		}
		if(boost::filesystem::create_directories(dirPCD)){
			std::cout << "PCD directory generated." << "\n";
		}
		if(boost::filesystem::create_directories(dirPLY)){
			std::cout << "PLY directory generated." << "\n";
		}
}

Grabber::~Grabber() {
}

void Grabber::keyListener(){
	std::string s;

	for(;;){
		getline(std::cin, s);

		mutex.lock();
		//boost::regex reg("[0-9]+");
		//if(boost::regex_match(s, reg)){
		if(s != "q"){
			cout << "Number: " << s << "\n";
			static boost::shared_array<unsigned char> rgb_array;
			static unsigned char* rgb_buffer = 0;
			static unsigned rgb_array_size = rgbImage_->getWidth() * rgbImage_->getHeight() * 3;
			rgb_array.reset (new unsigned char [rgb_array_size]);
			rgb_buffer = rgb_array.get ();
			rgbImage_->fillRGB (rgbImage_->getWidth(), rgbImage_->getHeight(), rgb_buffer, rgbImage_->getWidth() * 3);

			pcl::io::savePCDFile(dPCD + "/scan_" + s + ".pcd", *cloud_);
			pcl::io::savePLYFile(dPLY + "/scan_" + s + ".ply", *cloud_);
			//pcl::io::savePNGFile("test_pcd.png", *cloud_, "rgb");
			pcl::io::saveRgbPNGFile(dColor + "/scan_" + s + ".png", rgb_buffer, rgbImage_->getWidth(), rgbImage_->getHeight());
			//pcl::io::saveShortPNGFile(dDepth + "/scan_" + s +".png", reinterpret_cast<const unsigned short*>(&depthImage_->getDepthMetaData().Data()[0]), depthImage_->getWidth(), depthImage_->getHeight(), 1);
			pcl::io::saveShortPNGFile(dDepth + "/scan_" + s +".png", reinterpret_cast<const unsigned short*>(&depthImage_->getData()[0]), depthImage_->getWidth(), depthImage_->getHeight(), 1);
			std::cout << "Captures " << s << " recorded." << std::endl;
		}else if(s == "q"){
			std::cout << "Exiting..." << std::endl;
			interface->stop();
			exit(1);
		}
		mutex.unlock();
	}

}

void Grabber::images_cb_(const boost::shared_ptr<pcl::io::Image>& rgbImage,
		const boost::shared_ptr<pcl::io::DepthImage>& depthImage,
		float constant){
/*void Grabber::images_cb_(const boost::shared_ptr<openni_wrapper::Image>& rgbImage,
		const boost::shared_ptr<openni_wrapper::DepthImage>& depthImage,
		float constant){*/
	mutex.lock();
	cloud_ = convertToXYZRGBPointCloud(rgbImage, depthImage, "pene", constant);
	rgbImage_ = rgbImage;
	depthImage_ = depthImage;
	mutex.unlock();

	viewer.showCloud(cloud_);

}

pcl::PointCloud<pcl::PointXYZRGB>::Ptr Grabber::convertToXYZRGBPointCloud (
			const boost::shared_ptr<pcl::io::Image> &image,
			const boost::shared_ptr<pcl::io::DepthImage> &depth_image,
			std::string rgb_frame_id, float constant_)
{
/*pcl::PointCloud<pcl::PointXYZRGB>::Ptr Grabber::convertToXYZRGBPointCloud (
			const boost::shared_ptr<openni_wrapper::Image> &image,
			const boost::shared_ptr<openni_wrapper::DepthImage> &depth_image,
			std::string rgb_frame_id, float constant_){*/
	static unsigned rgb_array_size = 0;
	static boost::shared_array<unsigned char> rgb_array;
	static unsigned char* rgb_buffer = 0;

	int image_width_=image->getWidth();
	int image_height_=image->getHeight();
	int depth_width_=depth_image->getWidth();
	int depth_height_=depth_image->getHeight();

	  boost::shared_ptr<pcl::PointCloud<pcl::PointXYZRGB> > cloud (new pcl::PointCloud<pcl::PointXYZRGB>);

	  cloud->header.frame_id = rgb_frame_id;
	  cloud->height = std::max (image_height_, depth_height_);
	  cloud->width = std::max (image_width_, depth_width_);
	  cloud->is_dense = false;

	  cloud->points.resize (cloud->height * cloud->width);

	  float constant = constant_;//1.0f / -1.f; //device->getImageFocalLength (depth_width_);
	  register int centerX = (depth_width_ >> 1);
	  int centerY = (depth_height_ >> 1);

	  //register const XnDepthPixel* depth_map = depth_image->getDepthMetaData().Data();
	  register const XnDepthPixel* depth_map = depth_image->getData();
	  if (depth_image->getWidth () != depth_width_ || depth_image->getHeight() != depth_height_)
	  {
		static unsigned buffer_size = 0;
		static boost::shared_array<unsigned short> depth_buffer;

		if (buffer_size < depth_width_ * depth_height_)
		{
		  buffer_size = depth_width_ * depth_height_;
		  depth_buffer.reset (new unsigned short [buffer_size]);
		}

		depth_image->fillDepthImageRaw (depth_width_, depth_height_, depth_buffer.get ());
		depth_map = depth_buffer.get ();
	  }

	  // here we need exact the size of the point cloud for a one-one correspondence!
	  if (rgb_array_size < image_width_ * image_height_ * 3)
	  {
		rgb_array_size = image_width_ * image_height_ * 3;
		rgb_array.reset (new unsigned char [rgb_array_size]);
		rgb_buffer = rgb_array.get ();
	  }
	  image->fillRGB (image_width_, image_height_, rgb_buffer, image_width_ * 3);
	  float bad_point = std::numeric_limits<float>::quiet_NaN ();

	  // set xyz to Nan and rgb to 0 (black)
	  if (image_width_ != depth_width_)
	  {
		pcl::PointXYZRGB pt;
		pt.x = pt.y = pt.z = bad_point;
		pt.b = pt.g = pt.r = 0;
		pt.a = 255; // point has no color info -> alpha = max => transparent
		cloud->points.assign (cloud->points.size (), pt);
	  }

	  // fill in XYZ values
	  unsigned step = cloud->width / depth_width_;
	  unsigned skip = cloud->width * step - cloud->width;

	  int value_idx = 0;
	  int point_idx = 0;
	  for (int v = -centerY; v < centerY; ++v, point_idx += skip)
	  {
			for (register int u = -centerX; u < centerX; ++u, ++value_idx, point_idx += step)
			{
				pcl::PointXYZRGB& pt = cloud->points[point_idx];
				/// @todo Different values for these cases
				// Check for invalid measurements

				if (depth_map[value_idx] != 0 &&
					depth_map[value_idx] != depth_image->getNoSampleValue () &&
					depth_map[value_idx] != depth_image->getShadowValue ())
				{
					pt.z = depth_map[value_idx] * 0.001f;
					pt.x = static_cast<float> (u) * pt.z * constant;
					pt.y = static_cast<float> (v) * pt.z * constant;
				}
				else
				{
					pt.x = pt.y = pt.z = bad_point;
				}
			}
	  }

	  // fill in the RGB values
	  step = cloud->width / image_width_;
	  skip = cloud->width * step - cloud->width;

	  value_idx = 0;
	  point_idx = 0;
	  pcl::RGBValue color;
	  color.Alpha = 0;

	  for (unsigned yIdx = 0; yIdx < image_height_; ++yIdx, point_idx += skip)
	  {
		for (unsigned xIdx = 0; xIdx < image_width_; ++xIdx, point_idx += step, value_idx += 3)
		{
		  pcl::PointXYZRGB& pt = cloud->points[point_idx];

		  color.Red   = rgb_buffer[value_idx];
		  color.Green = rgb_buffer[value_idx + 1];
		  color.Blue  = rgb_buffer[value_idx + 2];

		  pt.rgba = color.long_value;
		}
	  }

	  /*cloud->sensor_origin_.setZero ();
	  cloud->sensor_orientation_.w () = 0.0;
	  cloud->sensor_orientation_.x () = 1.0;
	  cloud->sensor_orientation_.y () = 0.0;
	  cloud->sensor_orientation_.z () = 0.0;*/

	return (cloud);
}

void Grabber::run(){
	//viewer.registerKeyboardCallback(&Grabber::keyboard_cb, *this);
	//interface = new pcl::OpenNIGrabber();
	interface = new pcl::io::OpenNI2Grabber();

	/*boost::function<void (const boost::shared_ptr<openni_wrapper::Image> &, const boost::shared_ptr<openni_wrapper::DepthImage> &, float constant)> f =
			boost::bind(&Grabber::images_cb_, this, _1, _2, _3);*/
	boost::function<void (const boost::shared_ptr<pcl::io::Image> &, const boost::shared_ptr<pcl::io::DepthImage> &, float constant)> f =
				boost::bind(&Grabber::images_cb_, this, _1, _2, _3);

	interface->registerCallback(f);
	interface->start();

	while(!viewer.wasStopped()){
		boost::this_thread::sleep(boost::posix_time::seconds (1));
	}

	interface->stop();
}
