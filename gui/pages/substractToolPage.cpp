/*
 * substractToolPage.cpp
 *
 *  Created on: 26/01/2017
 *      Author: john
 */

#include <gui/pages/substractToolPage.h>

substractToolPage::substractToolPage(QWidget *parent) :
QWidget(parent),
ui(new Ui::substractToolPage){
	ui->setupUi(this);
	connect(ui->pbReturnMainPage, SIGNAL(clicked()), this, SLOT(goMainPage()));
	connect(ui->pbBackground, SIGNAL(clicked()), this, SLOT(setBackgroundImage()));
	connect(ui->pbObject, SIGNAL(clicked()), this, SLOT(setObjectImage()));
	connect(ui->pbSubstractTool, SIGNAL(clicked()), this, SLOT(substractObject()));
}

void substractToolPage::goMainPage(){
	gui::goToPage("mainPage");
}

substractToolPage::~substractToolPage() {
	delete ui;
}

void substractToolPage::setBackgroundImage(){
	QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open File"),
			mLastPage.toLocal8Bit().constData(),
			tr("Images (*.png *.jpg)"));
	if(!fileName.isNull())
		ui->txtBackground->setPlainText(fileName);
}

void substractToolPage::setObjectImage(){

}

void substractToolPage::substractObject(){
	bool isBackgroundImage = true;
	//Execute application to substract
	if(ui->txtBackground->toPlainText().isEmpty() ||
			ui->txtObject->toPlainText().isEmpty()){
		//throw window error
		QMessageBox msgBox(this);
		msgBox.setText("You must introduce the background and object images.");
		msgBox.exec();
	}else{
		QString command = "./imagesubstract --background ";
		command.append(ui->txtBackground->toPlainText());
		command.append(" --object ");
		command.append(ui->txtObject->toPlainText());
		system(command.toLocal8Bit().constData());
	}
}

