/*
 * wmain.cpp
 *
 *  Created on: 25/01/2017
 *      Author: john
 */

#include "gui/pages/mainPage.h"

mainPage::mainPage(QWidget* parent) :
QWidget(parent),
ui(new Ui::mainPage){
	ui->setupUi(this);
	connect(ui->pbSubstractTool, SIGNAL(clicked()), this, SLOT(goSubstracToolPage()));
}

mainPage::~mainPage() {
	delete ui;
}

void mainPage::goSubstracToolPage(){
	gui::goToPage("substractToolPage");
}

void mainPage::goLabelToolPage(){

}
