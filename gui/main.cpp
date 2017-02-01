#include <iostream>
#include "gui/gui.h"
#include <qapplication.h>

#include "gui/pages/mainPage.h"
#include "gui/pages/substractToolPage.h"
#include "gui/pages/labelToolPage.h"
#include "gui/pages/captureToolPage.h"

int main(int argc, char* argv[]){

	QApplication app(argc, argv);
	//Main interface
	gui::initClass();

	//Add pages
	mainPage *mMainPage = new mainPage();
	substractToolPage *mSubstractToolPage = new substractToolPage();
	labelToolPage *mLabelToolPage = new labelToolPage();
	captureToolPage *mCaptureToolPage = new captureToolPage();

	gui::addPage(mMainPage);
	gui::addPage(mSubstractToolPage);
	gui::addPage(mLabelToolPage);
	gui::addPage(mCaptureToolPage);

	gui::show();
	int output = app.exec();
	gui::destroyClass();//The pages will be destroyed too
	return output;
}

