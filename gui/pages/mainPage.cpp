/*
 * wmain.cpp
 *
 *  Created on: 25/01/2017
 *      Author: john
 */

#include "gui/pages/mainPage.h"

mainPage::mainPage(QWidget* parent) :
QWidget(parent),
mUi(new Ui::mainPage){
	mUi->setupUi(this);
	connect(mUi->pbSubstractTool, SIGNAL(clicked()), this, SLOT(goSubstracToolPage()));
	connect(mUi->pbLabelTool, SIGNAL(clicked()), this, SLOT(goLabelToolPage()));
}

mainPage::~mainPage() {
	delete mUi;
}

void mainPage::goSubstracToolPage(){
	gui::goToPage("substractToolPage");
}

void mainPage::goLabelToolPage(){
	gui::goToPage("labelToolPage");
}
