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
	mLabelToolProcess = new QProcess(this);
	connect(mUi->pbReturnMainPage, SIGNAL(clicked()), this, SLOT(goMainPage()));
	connect(mUi->pbSetDataset, SIGNAL(clicked()), this, SLOT(setDataset()));
	connect(mUi->pbSetOutput, SIGNAL(clicked()), this, SLOT(setOutput()));
}

void labelToolPage::labelDataset(){
	QStringList arguments;
	arguments << "python";
	arguments << "indexPngDataset.py";
	arguments << mUi->txtDataset->text();
	arguments << mUi->txtOutput->text();
}

void labelToolPage::setDataset(){

}

void labelToolPage::setOutput(){

}

void labelToolPage::goMainPage(){
	gui::goToPage("mainPage");
}

labelToolPage::~labelToolPage() {
	delete mUi;
}

