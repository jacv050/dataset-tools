/*
 * wmain.cpp
 *
 *  Created on: 25/01/2017
 *      Author: john
 */

#include "gui/pages/wmain.h"

wmain::wmain(QWidget* parent) :
QWidget(parent),
ui(new Ui::DatasetTool){
	ui->setupUi(this);
}

wmain::~wmain() {

}

