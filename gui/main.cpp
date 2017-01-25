#include <iostream>
#include "gui/gui.h"
#include <qapplication.h>

#include "gui/pages/mainPage.h"

int main(int argc, char* argv[]){

	QApplication app(argc, argv);
	//Main interface
	gui mGui;

	//Add pages
	mainPage *mMainPage = new mainPage();
	mGui.addPage(mMainPage);

	mGui.show();

	return app.exec();
}

