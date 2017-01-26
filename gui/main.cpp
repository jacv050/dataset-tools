#include <iostream>
#include "gui/gui.h"
#include <qapplication.h>

#include "gui/pages/mainPage.h"

int main(int argc, char* argv[]){

	QApplication app(argc, argv);
	//Main interface
	gui::initClass();

	//Add pages
	mainPage *mMainPage = new mainPage();
	gui::addPage(mMainPage);

	gui::show();

	return app.exec();
}

