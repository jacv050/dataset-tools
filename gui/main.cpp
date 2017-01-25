#include <iostream>
#include "gui/gui.h"
#include <qapplication.h>

int main(int argc, char* argv[]){

	QApplication app(argc, argv);
	gui mGui;

	mGui.show();

	return app.exec();
}

