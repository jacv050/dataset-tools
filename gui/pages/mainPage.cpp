/*
 * wmain.cpp
 *
 *  Created on: 25/01/2017
 *      Author: john
 */

#include "gui/pages/mainPage.h"

mainPage::mainPage(QWidget* parent) :
QWidget(parent),
ui(new Ui::pageMain){
	ui->setupUi(this);
	connect(ui->pb_substractTool, SIGNAL(clicked()), this, SLOT(goSubstracToolPage()));
}

mainPage::~mainPage() {
	delete ui;
}

void mainPage::goSubstracToolPage(){
	gui::goToPage("pageSubstractTool");
}

void mainPage::goLabelToolPage(){

}
