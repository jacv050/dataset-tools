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
	connect(ui->pbReturnMainPage, SIGNAL(clicked()), this, SLOT(goMainPage()));
	connect(ui->pbBackground, SIGNAL(clicked()), this, SLOT(setBackgroundImage()));
	connect(ui->pbObject, SIGNAL(clicked()), this, SLOT(setObjectImage()));
	connect(ui->pbSubstractTool, SIGNAL(clicked()), this, SLOT(substractObject()));
	imageSubstractProcess = new QProcess(this);
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
		ui->txtBackground->setPlainText(fileName);
	}
}

void substractToolPage::setObjectImage(){
	QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open File"),
			mLastPageObject.toLocal8Bit().constData(),
			imageFormats);
	if(!fileName.isNull()){
		mLastPageObject = fileName;
		ui->txtObject->setPlainText(fileName);
	}
}

void substractToolPage::substractObject(){
	//imageSubstractProcess->start("nautilus", QStringList() << ".");
	bool isBackgroundImage = true;
	//Execute application to substract
	if(ui->txtBackground->toPlainText().isEmpty() ||
			ui->txtObject->toPlainText().isEmpty()){
		//throw window error
		QMessageBox msgBox(this);
		msgBox.setText("You must introduce the background and object images.");
		msgBox.exec();
	}else{
		QStringList args;
		args << "--background";
		args << ui->txtBackground->toPlainText();
		args << "--object";
		args << ui->txtObject->toPlainText();

		//QDir::currentPath().append("/imagesubstract") //QDir::separator
		QDir dir(".");
		imageSubstractProcess->start(dir.absoluteFilePath("imagesubstract"), args);
		if (imageSubstractProcess->waitForStarted(-1)) {
			while(imageSubstractProcess->waitForReadyRead(-1)) {
				QString s = QString(imageSubstractProcess->readAllStandardOutput());
				std::cout << s.toStdString();
			}
		}

	}
}

