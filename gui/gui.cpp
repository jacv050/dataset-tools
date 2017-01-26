/*
 * gui.cpp
 *
 *  Created on: 25/01/2017
 *      Author: john
 */

#include "gui/gui.h"
#include <QtWidgets>
#include <iostream>

QStackedWidget *gui::mStackedWidget = NULL;
QWidget *gui::mCentralWidget;
QMainWindow *gui::mGui;

gui::gui() {
	//QMetaObject::connectSlotsByName(this);
}

void gui::addPage(QWidget *page){
	page->setParent(mStackedWidget);
	mStackedWidget->addWidget(page);
}

QWidget* gui::getParentPages(){
	return mStackedWidget;
}

void gui::initClass(){
	if(mGui == NULL){
		mGui = new QMainWindow();
		mGui->resize(600, 400);
		mCentralWidget = new QWidget(mGui);
		mCentralWidget->setObjectName(QString::fromUtf8("centralWidget"));
		mCentralWidget->setMinimumSize(QSize(0, 0));
		mCentralWidget->resize(600, 400);

		mStackedWidget = new QStackedWidget(mCentralWidget);
		mStackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
		mStackedWidget->setGeometry(QRect(0, 0, 1061, 620));

		mGui->setCentralWidget(mCentralWidget);
		mStackedWidget->setCurrentIndex(KINITPAGE);
	}
}

void gui::goToPage(QWidget* page){
	for(int i=0; i<mStackedWidget->count(); ++i){
		if(dynamic_cast<mainPage*>(page))
			std::cout << i << std::endl;

	}
}

void gui::show(){
	mGui->show();
}

gui::~gui() {
	// TODO Auto-generated destructor stub
}

