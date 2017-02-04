
#include "boost/program_options.hpp"
#include <boost/filesystem.hpp>
#include <iostream>

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>

#define PARAM_HELP	"help"
#define PARAM_PATH "n"


bool parse_command_line_options(boost::program_options::variables_map &variables_map, const int &argc, char **argv){

	try{

		boost::program_options::options_description desc("Allowed descriptions");
		desc.add_options()
				(PARAM_HELP, "produce help message")
				(PARAM_PATH, boost::program_options::value<std::string>()->required(), "Path to pcd route");

		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), variables_map);

		if(variables_map.count(PARAM_HELP)){
			std::cout << desc << "\n";
			return true;
		}

		boost::program_options::notify(variables_map);

	}catch(std::exception &e){
		std::cerr << "Error: " << e.what() << "\n";
		return true;
	}

	return false;
}

int main(int argc, char **argv){

	boost::program_options::variables_map variablesMap;
	if(parse_command_line_options(variablesMap, argc, argv)){
		return 1;
	}

	pcl::visualization::CloudViewer viewer("Viewer for GUI");
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGB>);

	if(pcl::io::loadPCDFile<pcl::PointXYZRGB>(variablesMap[PARAM_PATH].as<std::string>().c_str(), *cloud) == -1){
		PCL_ERROR ("Couldn't read file test_pcd.pcd \n");
		return -1;
	}

	viewer.showCloud(cloud);
	while(!viewer.wasStopped());

	return 0;
}
