/*
 * substractToolPage.cpp
 *
 *  Created on: 26/01/2017
 *      Author: john
 */

#include <gui/pages/substractToolPage.h>

substractToolPage::substractToolPage(QWidget *parent) :
QWidget(parent),
ui(new Ui::pageSubstractTool){
	ui->setupUi(this);
	connect(ui->pb_returnMainPage, SIGNAL(clicked()), this, SLOT(goMainPage()));
}

void substractToolPage::goMainPage(){

}

substractToolPage::~substractToolPage() {
	delete ui;
}

