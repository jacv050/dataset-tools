/*
 * substractToolPage.cpp
 *
 *  Created on: 26/01/2017
 *      Author: john
 */

#include <gui/pages/substractToolPage.h>

const QString substractToolPage::imageFormats = tr("Images (*.png *.jpg)");

substractToolPage::substractToolPage(QWidget *parent) :
QWidget(parent),
ui(new Ui::substractToolPage){
	ui->setupUi(this);
	mImageSubstractProcess = new QProcess(this);
	connect(ui->pbReturnMainPage, SIGNAL(clicked()), this, SLOT(goMainPage()));
	connect(ui->pbBackground, SIGNAL(clicked()), this, SLOT(setBackgroundImage()));
	connect(ui->pbObject, SIGNAL(clicked()), this, SLOT(setObjectImage()));
	connect(ui->pbSubstractTool, SIGNAL(clicked()), this, SLOT(substractObject()));
	connect(mImageSubstractProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyOutputMsgProcess()));
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
			mLastPageBackground.toLocal8Bit().constData(),
			imageFormats);
	if(!fileName.isNull()){
		mLastPageBackground = fileName;
		ui->txtBackground->setText(fileName);
	}
}

void substractToolPage::setObjectImage(){
	QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open File"),
			mLastPageObject.toLocal8Bit().constData(),
			imageFormats);
	if(!fileName.isNull()){
		mLastPageObject = fileName;
		ui->txtObject->setText(fileName);
	}
}

void substractToolPage::readyOutputMsgProcess(){
	QString s = QString(mImageSubstractProcess->readAllStandardOutput());
	//ui->txtOutputProcess->setPlainText(s);
	ui->txtOutputProcess->appendPlainText(s);
}

void substractToolPage::substractObject(){
	bool isBackgroundImage = true;
	//Execute application to substract
	if(ui->txtBackground->text().isEmpty() ||
			ui->txtObject->text().isEmpty()){
		//throw window error
		QMessageBox msgBox(this);
		msgBox.setText("You must introduce the background and object images.");
		msgBox.exec();
	}else{
		//Arguments
		QStringList args;
		args << "--background";
		args << ui->txtBackground->text();
		args << "--object";
		args << ui->txtObject->text();

		//QDir::currentPath().append("/imagesubstract") //QDir::separator
		QDir dir(".");
		//Check if there is a process active and close it to reopen
		if(mImageSubstractProcess->isOpen()){
			mImageSubstractProcess->kill();
			mImageSubstractProcess->waitForFinished();
		}

		mImageSubstractProcess->start(dir.absoluteFilePath("imagesubstract"), args);
	}
}

