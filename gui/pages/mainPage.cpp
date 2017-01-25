/*
 * wmain.cpp
 *
 *  Created on: 25/01/2017
 *      Author: john
 */

#include "gui/pages/mainPage.h"

mainPage::mainPage(QWidget* parent) :
QWidget(parent),
ui(new Ui::DatasetTool){
	ui->setupUi(this);
}

mainPage::~mainPage() {

}

