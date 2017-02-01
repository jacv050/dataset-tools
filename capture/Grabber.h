/*
 * Grabber.h
 *
 *  Created on: 09/09/2016
 *      Author: john
 */

#ifndef GRABBER_H_
#define GRABBER_H_

#include <pcl/visualization/cloud_viewer.h>
#include <pcl/visualization/boost.h>
//#include <pcl/io/image.h>
//#include <pcl/io/image_depth.h>
#include <pcl/io/openni_camera/openni_depth_image.h>
#include <pcl/io/openni_camera/openni_image.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/io/png_io.h>

#include <pcl/io/oni_grabber.h>
#include <pcl/io/pcd_grabber.h>

#include <pcl/io/openni_grabber.h>
#include <pcl/io/openni2_grabber.h>
#include <boost/timer/timer.hpp>

 #include <pcl/io/image_metadata_wrapper.h>
#include <pcl/io/lzf_image_io.h>

#include <cstdio>

#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/regex.hpp>

namespace pcl
{
	typedef union
	{
		struct
		{
			unsigned char Blue;
			unsigned char Green;
			unsigned char Red;
			unsigned char Alpha;
		};
		float float_value;
		uint32_t long_value;
	}RGBValue;
}

class Grabber {
public:
	Grabber(const std::string& dir);
	void run();

	/*pcl::PointCloud<pcl::PointXYZRGB>::Ptr convertToXYZRGBPointCloud (
				const boost::shared_ptr<openni_wrapper::Image> &image,
				const boost::shared_ptr<openni_wrapper::DepthImage> &depth_image,
				std::string rgb_frame_id, float constant_);*/
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr convertToXYZRGBPointCloud (
					const boost::shared_ptr<pcl::io::Image> &image,
					const boost::shared_ptr<pcl::io::DepthImage> &depth_image,
					std::string rgb_frame_id, float constant_);
	/*void images_cb_(const boost::shared_ptr<openni_wrapper::Image>& rgbImage,
				const boost::shared_ptr<openni_wrapper::DepthImage>& depthImage,
				float constant);*/
	void images_cb_(const boost::shared_ptr<pcl::io::Image>& rgbImage,
					const boost::shared_ptr<pcl::io::DepthImage>& depthImage,
					float constant);

	pcl::visualization::CloudViewer viewer;

	virtual ~Grabber();

private:
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_;

	//boost::shared_ptr<openni_wrapper::Image> rgbImage_;
	//boost::shared_ptr<openni_wrapper::DepthImage> depthImage_;

	boost::shared_ptr<pcl::io::Image> rgbImage_;
	boost::shared_ptr<pcl::io::DepthImage> depthImage_;
	boost::thread *thread;

	boost::mutex mutex;
	std::string dir_;
	void keyListener();

	std::string dPCD;
	std::string dPLY;
	std::string dColor;
	std::string dDepth;

	pcl::Grabber* interface;
};

#endif /* GRABBER_H_ */
