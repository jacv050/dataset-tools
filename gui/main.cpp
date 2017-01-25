#include <iostream>
#include "gui/gui.h"
#include <qapplication.h>

#include "gui/pages/wmain.h"

int main(int argc, char* argv[]){

	QApplication app(argc, argv);
	//Main interface
	gui mGui;

	//Add pages
	wmain *mWmain = new wmain();
	mGui.addPage(mWmain);

	mGui.show();

	return app.exec();
}

