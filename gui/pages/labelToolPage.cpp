/*
 * labelToolPage.cpp
 *
 *  Created on: 28/01/2017
 *      Author: john
 */

#include <gui/pages/labelToolPage.h>

labelToolPage::labelToolPage(QWidget *parent) :
QWidget(parent),
mUi(new Ui::labelToolPage){
	mUi->setupUi(this);
	connect(mUi->pbReturnMainPage, SIGNAL(clicked()), this, SLOT(goMainPage()));
}

void labelToolPage::goMainPage(){
	gui::goToPage("mainPage");
}

labelToolPage::~labelToolPage() {
}

