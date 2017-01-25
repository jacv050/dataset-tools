/*
 * gui.cpp
 *
 *  Created on: 25/01/2017
 *      Author: john
 */

#include <gui/gui.h>
#include <QtWidgets>

gui::gui() {
	centralWidget = new QWidget(this);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    centralWidget->setMinimumSize(QSize(0, 0));
    centralWidget->setStyleSheet(QString::fromUtf8("QWidget {\n"
    		"	background-color: #1abc9c;\n"
    		"{\n""	background-color: #1abc9c;\n"
    		"}"));
	stackedWidget = new QStackedWidget(centralWidget);
}

gui::~gui() {
	// TODO Auto-generated destructor stub
}

