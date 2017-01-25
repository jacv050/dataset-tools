/*
 * gui.cpp
 *
 *  Created on: 25/01/2017
 *      Author: john
 */

#include "gui/gui.h"
#include <QtWidgets>

gui::gui() {
	resize(600, 400);
	mCentralWidget = new QWidget(this);
	mCentralWidget->setObjectName(QString::fromUtf8("centralWidget"));
	mCentralWidget->setMinimumSize(QSize(0, 0));
	mCentralWidget->resize(600, 400);

	mStackedWidget = new QStackedWidget(mCentralWidget);
	mStackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
	mStackedWidget->setGeometry(QRect(0, 0, 1061, 620));

	setCentralWidget(mCentralWidget);
	mStackedWidget->setCurrentIndex(KINITPAGE);
	//QMetaObject::connectSlotsByName(this);
}

void gui::addPage(QWidget *page){
	page->setParent(mStackedWidget);
	mStackedWidget->addWidget(page);
}

QWidget* gui::getParentPages() const{
	return mStackedWidget;
}

gui::~gui() {
	// TODO Auto-generated destructor stub
}

