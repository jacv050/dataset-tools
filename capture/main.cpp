
#include "Grabber.h"
#include "boost/program_options.hpp"
#include <boost/filesystem.hpp>
#include <iostream>

#define PARAM_HELP	"help"
#define PARAM_NAME_SCENE "n"


bool parse_command_line_options(boost::program_options::variables_map &variables_map, const int &argc, char **argv){

	try{

		boost::program_options::options_description desc("Allowed descriptions");
		desc.add_options()
				(PARAM_HELP, "produce help message")
				(PARAM_NAME_SCENE, boost::program_options::value<std::string>()->required(), "Scene name");

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


	//Get name scene
	Grabber g(variablesMap[PARAM_NAME_SCENE].as<std::string>());

	g.run();

	return 0;
}
