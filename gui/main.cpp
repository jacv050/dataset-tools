#include <iostream>
#include "gui/gui.h"
#include <qapplication.h>

#include "gui/pages/mainPage.h"
#include "gui/pages/substractToolPage.h"

int main(int argc, char* argv[]){

	QApplication app(argc, argv);
	//Main interface
	gui::initClass();

	//Add pages
	mainPage *mMainPage = new mainPage();
	substractToolPage *mSubstractToolPage = new substractToolPage();

	gui::addPage(mMainPage);
	gui::addPage(mSubstractToolPage);

	gui::show();
	int output = app.exec();

	gui::destroyClass();
	delete mMainPage;
	delete mSubstractToolPage;

	return output;
}

