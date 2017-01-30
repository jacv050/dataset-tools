/*
 * selectObjectDialog.cpp
 *
 *  Created on: 30/01/2017
 *      Author: john
 */

#include <gui/pages/selectObjectDialog.h>

selectObjectDialog::selectObjectDialog(){
	//setParent(parent);
	mVerticalLayout = new QVBoxLayout(this);
	mDatasetList = new QListWidget(this);
	mVerticalLayout->addWidget(mDatasetList);
}

selectObjectDialog::~selectObjectDialog() {
}

