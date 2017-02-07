/*
 * substractToolPage.cpp
 *
 *  Created on: 26/01/2017
 *      Author: john
 */

#include <gui/pages/substractToolPage.h>

const QString substractToolPage::KIMAGEFORMATS("Images (*.png *.jpg)");
const QString substractToolPage::KOUTPUTIMAGEFORMATS("Images (*.png)");

substractToolPage::substractToolPage(QWidget *parent) :
QWidget(parent),
mUi(new Ui::substractToolPage){
	mUi->setupUi(this);
	mImageViewer = new imageViewerDialog();
	mUi->sbHsvchannel->setEnabled(mUi->cbSubstractmode->isChecked());
	mImageSubstractProcess = new QProcess(this);
	mLastMaskDestiny = QDir::currentPath() + QDir::separator() + tr("finalimage.png");
	mUi->txtMaskDestiny->setText(mLastMaskDestiny);
	connect(mUi->pbReturnMainPage, SIGNAL(clicked()), this, SLOT(goMainPage()));
	connect(mUi->pbBackground, SIGNAL(clicked()), this, SLOT(setBackgroundImage()));
	connect(mUi->pbObject, SIGNAL(clicked()), this, SLOT(setObjectImage()));
	connect(mUi->pbMaskDestiny, SIGNAL(clicked()), this, SLOT(setMaskNameDestiny()));
	connect(mUi->pbSubstractTool, SIGNAL(clicked()), this, SLOT(substractObject()));
	connect(mUi->cbSubstractmode, SIGNAL(clicked()), this, SLOT(substractModeChanged()));
	connect(mUi->pbNewObject, SIGNAL(clicked()), this, SLOT(setNewObject()));
	connect(mImageSubstractProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyOutputMsgProcess()));
	connect(mUi->pbShowOutput, SIGNAL(clicked()), this, SLOT(showOutput()));
}

void substractToolPage::showOutput(){
	mImageViewer->showImage(mUi->txtMaskDestiny->text());
}

void substractToolPage::setNewObject(){
	QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open File"),
			mLastPageNewObject.toLocal8Bit().constData(),
			KIMAGEFORMATS);
	if(!fileName.isNull()){
		mUi->txtBackground->setText(mUi->txtObject->text());
		mLastPageNewObject = fileName;
		mUi->txtObject->setText(fileName);
	}
}

void substractToolPage::goMainPage(){
	gui::goToPage("mainPage");
}

substractToolPage::~substractToolPage() {
	delete mUi;
}

void substractToolPage::substractModeChanged(){
	mUi->sbHsvchannel->setEnabled(mUi->cbSubstractmode->isChecked());
}

void substractToolPage::setBackgroundImage(){
	QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open File"),
			mLastPageBackground.toLocal8Bit().constData(),
			KIMAGEFORMATS);
	if(!fileName.isNull()){
		mLastPageBackground = fileName;
		mUi->txtBackground->setText(fileName);
	}
}

void substractToolPage::setObjectImage(){
	QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open File"),
			mLastPageObject.toLocal8Bit().constData(),
			KIMAGEFORMATS);
	if(!fileName.isNull()){
		mLastPageObject = fileName;
		mUi->txtObject->setText(fileName);
	}
}

void substractToolPage::setMaskNameDestiny(){
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setNameFilter(KOUTPUTIMAGEFORMATS);
	dialog.setViewMode(QFileDialog::Detail);
	if(!mLastMaskDestiny.isEmpty())
		dialog.setDirectory(mLastMaskDestiny);

	QStringList fileNames;
	if (dialog.exec()){
	    fileNames = dialog.selectedFiles();
	    if(!fileNames.at(0).isNull()){
	    	mLastMaskDestiny = fileNames.at(0);
	    	mUi->txtMaskDestiny->setText(fileNames.at(0));
		}
	}
}

void substractToolPage::readyOutputMsgProcess(){
	QString s = tr(mImageSubstractProcess->readAllStandardOutput());
	if(s.contains("saved"))
		mUi->pbShowOutput->setEnabled(true);
	mUi->txtOutputProcess->appendPlainText(s);
}

void substractToolPage::substractObject(){
	if(!mUi->pbNewObject->isEnabled()){
		mUi->pbNewObject->setEnabled(true);
	}
	//Execute application to substract
	if(mUi->txtBackground->text().isEmpty() ||
			mUi->txtObject->text().isEmpty() ||
			mUi->txtMaskDestiny->text().isEmpty()){
		//throw window error
		QMessageBox msgBox(this);
		msgBox.setText("You must introduce the background image, object image and mask destiny.");
		msgBox.exec();
	}else{
		//Arguments
		QStringList args;
		args << "--background";
		args << mUi->txtBackground->text();
		args << "--object";
		args << mUi->txtObject->text();
		args << "--outputimage";
		args << mUi->txtMaskDestiny->text();
		args << "--gaussianiterations";
		args << mUi->sbGaussianiterations->text();
		args << "--windowratio";
		args << mUi->sbWindowratio->text();
		args << "--threshold";
		args << mUi->sbThreshold->text();
		if(mUi->cbSubstractmode->isChecked()){
			args << "--substractmode";
			args << "1";
			args << "--hsvchannel";
			args << mUi->sbHsvchannel->text();
		}
		if(mUi->cbPrbfgb->isChecked()){
			args << "--prbfgb";
			args << "1";
		}

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

