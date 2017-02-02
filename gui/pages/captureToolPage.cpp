/*
 * captureToolPage.cpp
 *
 *  Created on: 31/01/2017
 *      Author: john
 */

#include <gui/pages/captureToolPage.h>

const QString captureToolPage::KROUTECOLORDIRECTORY = tr("");
const QString captureToolPage::KROUTEDEPTHDIRECTORY = tr("");
const QString captureToolPage::KROUTEPCDDIRECTORY = tr("");

captureToolPage::captureToolPage(QWidget *parent) :
QWidget(parent),
mUi(new Ui::captureToolPage){
	mUi->setupUi(this);
	mCaptureToolProcess = new QProcess(this);
	connect(mUi->pbReturnMainPage, SIGNAL(clicked()), this, SLOT(goMainPage()));
	connect(mUi->pbOutput, SIGNAL(clicked()), this, SLOT(setOutputDirectory()));
	connect(mUi->pbCapture, SIGNAL(clicked()), this, SLOT(doCapture()));
	connect(mUi->pbShowColorImage, SIGNAL(clicked()), this, SLOT(showColorImage()));
	connect(mUi->pbShowDepthImage, SIGNAL(clicked()), this, SLOT(showDepthImage()));
	connect(mUi->pbShowPCD, SIGNAL(clicked()), this, SLOT(showPCD()));
	connect(mUi->pbInitTool, SIGNAL(clicked()), this, SLOT(initTool()));
	connect(mCaptureToolProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(closedProcess()));
	connect(mCaptureToolProcess, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(closedProcess()));
}

void captureToolPage::closedProcess(){
	mUi->pbInitTool->setEnabled(true);
	mUi->pbCapture->setEnabled(false);
}

void captureToolPage::goMainPage(){
	gui::goToPage("mainPage");
}

void captureToolPage::setOutputDirectory(){
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setOption(QFileDialog::ShowDirsOnly);
	//dialog.setNameFilter(KOUTPUTIMAGEFORMATS);
	dialog.setViewMode(QFileDialog::Detail);
	if(!mLastOutputDirectory.isEmpty())
		dialog.setDirectory(mLastOutputDirectory);

	QStringList fileNames;
	if (dialog.exec()){
	    fileNames = dialog.selectedFiles();
	    if(!fileNames.at(0).isNull()){
	    	mLastOutputDirectory = fileNames.at(0);
	    	mUi->txtOutputDirectory->setText(fileNames.at(0));
		}
	}
}

void captureToolPage::doCapture(){

}

void captureToolPage::showColorImage(){

}

void captureToolPage::showDepthImage(){

}

void captureToolPage::showPCD(){

}

void captureToolPage::initTool(){
	mUi->pbInitTool->setEnabled(false);
	mUi->pbCapture->setEnabled(true);
	//Check parameters
	if(mUi->txtOutputDirectory->text().isEmpty()){
		//throw window error
		QMessageBox msgBox(this);
		msgBox.setText("You must introduce output directory.");
		msgBox.exec();
	}else{
		QStringList arguments;
		arguments << "--n";
		arguments << mUi->txtOutputDirectory->text();

		if(mCaptureToolProcess->isOpen()){
			mCaptureToolProcess->kill();
			mCaptureToolProcess->waitForFinished();
		}

		mCaptureToolProcess->start(QDir::current().absoluteFilePath("capture"), arguments);
	}
}

captureToolPage::~captureToolPage() {
	delete mUi;
}

