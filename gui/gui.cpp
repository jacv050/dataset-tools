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
		//mGui = new QMainWindow();
		mGui = new gui();
		mCentralWidget = new QWidget(mGui);
		mCentralWidget->setObjectName(QString::fromUtf8("centralWidget"));
		mCentralWidget->setMinimumSize(QSize(0, 0));
		mCentralWidget->resize(100, 30);

		mStackedWidget = new QStackedWidget(mCentralWidget);
		mStackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
		mStackedWidget->setGeometry(QRect(0, 0, 1061, 620));

		mGui->setCentralWidget(mCentralWidget);
		mStackedWidget->setCurrentIndex(KINITPAGE);
		//currentChanged(int)
		connect(mStackedWidget, SIGNAL(currentChanged(int)), mGui, SLOT(indexChanged(int)));
		//connect(mStackedWidget,SIGNAL(currentChanged(int)),this,indexChanged(int));
	}
}

void gui::destroyClass(){
	//delete mStackedWidget;
	//delete mCentralWidget;
	delete mGui;//Destroyin mGui the others wigets will be free too.
}

void gui::goToPage(const std::string& page){
	QString namePage(page.c_str());
	for(int i=0; i<mStackedWidget->count(); ++i){
		if(mStackedWidget->widget(i)->objectName() == namePage)
			mStackedWidget->setCurrentIndex(i);
	}
}

void gui::indexChanged(int index){
	mGui->setMinimumSize(mStackedWidget->currentWidget()->minimumSize());
	mGui->setMaximumSize(mStackedWidget->currentWidget()->maximumSize());
}

void gui::setMinimumConstraint(int width, int height){
	mGui->setMinimumSize(width, height);
}

void gui::setMaximumConstraint(int width, int height){
	mGui->setMaximumSize(width, height);
}

void gui::resize(int width, int heihgt){
	mGui->resize(width, heihgt);
}

void gui::show(){
	mGui->show();
}

gui::~gui() {
}

