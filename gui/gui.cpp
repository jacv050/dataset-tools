/*
 * gui.cpp
 *
 *  Created on: 25/01/2017
 *      Author: john
 */

#include "gui/gui.h"
#include <QtWidgets>

gui::gui() {
	mCentralWidget = new QWidget(this);
	mCentralWidget->setObjectName(QString::fromUtf8("centralWidget"));
	mCentralWidget->setMinimumSize(QSize(0, 0));
	//mCentralWidget->setStyleSheet(QString::fromUtf8("QWidget {\n background-color: #1abc9c;\n {\n background-color: #1abc9c;\n }"));

	mStackedWidget = new QStackedWidget(mCentralWidget);
	mStackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
	mStackedWidget->setGeometry(QRect(20, 120, 1061, 620));
	//Init pages
	mWmain = new wmain(mStackedWidget);

	//Add pages
	mStackedWidget->addWidget(mWmain);

	setCentralWidget(mCentralWidget);
	mStackedWidget->setCurrentIndex(10);
	//QMetaObject::connectSlotsByName(this);
}

gui::~gui() {
	// TODO Auto-generated destructor stub
}

