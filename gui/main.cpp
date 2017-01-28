#include <iostream>
#include "gui/gui.h"
#include <qapplication.h>

#include "gui/pages/mainPage.h"
#include "gui/pages/substractToolPage.h"
#include "gui/pages/labelToolPage.h"

int main(int argc, char* argv[]){

	QApplication app(argc, argv);
	//Main interface
	gui::initClass();

	//Add pages
	mainPage *mMainPage = new mainPage();
	substractToolPage *mSubstractToolPage = new substractToolPage();
	labelToolPage *mLabelToolPage = new labelToolPage();

	gui::addPage(mMainPage);
	gui::addPage(mSubstractToolPage);
	gui::addPage(mLabelToolPage);

	gui::show();
	int output = app.exec();
	gui::destroyClass();//The pages will be destroyed too
	return output;
}

