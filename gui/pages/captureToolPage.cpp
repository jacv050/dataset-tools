/*
 * captureToolPage.cpp
 *
 *  Created on: 31/01/2017
 *      Author: john
 */

#include <gui/pages/captureToolPage.h>

captureToolPage::captureToolPage(QWidget *parent) :
QWidget(parent),
mUi(new Ui::captureToolPage){
	mUi->setupUi(this);
	connect(mUi->pbReturnMainPage, SIGNAL(clicked()), this, SLOT(goMainPage()));
}

void captureToolPage::goMainPage(){
	gui::goToPage("mainPage");
}

captureToolPage::~captureToolPage() {
	delete mUi;
}

