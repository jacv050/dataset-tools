/*
 * captureToolPage.cpp
 *
 *  Created on: 31/01/2017
 *      Author: john
 */

#include <gui/pages/captureToolPage.h>

const QString captureToolPage::KROUTECOLORDIRECTORY = QString(QDir::separator()).append("color_images").append(QDir::separator()).append("scan_");
const QString captureToolPage::KROUTEDEPTHDIRECTORY = QString(QDir::separator()).append("depth_images").append(QDir::separator()).append("scan_");
const QString captureToolPage::KROUTEPCDDIRECTORY = QString(QDir::separator()).append("original_clouds").append(QDir::separator()).append("pcd").append(QDir::separator()).append("scan_");

captureToolPage::captureToolPage(QWidget *parent) :
QWidget(parent),
mUi(new Ui::captureToolPage){
	mUi->setupUi(this);
	mImgColorViewer = new imageViewerDialog();
	mImgDepthViewer = new imageViewerDialog();
	m3dViewer = new QProcess(this);
	mCaptureToolProcess = new QProcess(this);
	connect(mUi->pbReturnMainPage, SIGNAL(clicked()), this, SLOT(goMainPage()));
	connect(mUi->pbOutput, SIGNAL(clicked()), this, SLOT(setOutputDirectory()));
	connect(mUi->pbCapture, SIGNAL(clicked()), this, SLOT(doCapture()));
	connect(mUi->pbShowColorImage, SIGNAL(clicked()), this, SLOT(showColorImage()));
	connect(mUi->pbShowDepthImage, SIGNAL(clicked()), this, SLOT(showDepthImage()));
	connect(mUi->pbShowPCD, SIGNAL(clicked()), this, SLOT(showPCD()));
	connect(mUi->pbInitTool, SIGNAL(clicked()), this, SLOT(initTool()));
	connect(mCaptureToolProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(closedProcess()));
	connect(mCaptureToolProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyOutputMsgProcess()));
}

void captureToolPage::readyOutputMsgProcess(){
	QString s = tr(mCaptureToolProcess->readAllStandardOutput());
	if(s.contains("recorded")){
		mUi->pbShowColorImage->setEnabled(true);
		mUi->pbShowDepthImage->setEnabled(true);
		mUi->pbShowPCD->setEnabled(true);
	}
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
	if(mUi->txtCapture->text().isEmpty()){
		//throw window error
		QMessageBox msgBox(this);
		msgBox.setText("You must introduce a name for capture.");
		msgBox.exec();
	}
	mCaptureToolProcess->write(mUi->txtCapture->text().append("\n").toLocal8Bit().constData());
}

void captureToolPage::showColorImage(){
	mImgColorViewer->showImage(mUi->txtOutputDirectory->text().append(KROUTECOLORDIRECTORY).append(mUi->txtCapture->text()).append(".png"));
}

void captureToolPage::showDepthImage(){
	mImgDepthViewer->showImage(mUi->txtOutputDirectory->text().append(KROUTEDEPTHDIRECTORY).append(mUi->txtCapture->text()).append(".png"));
}

void captureToolPage::showPCD(){
	QStringList arguments;
	arguments << "--n";
	arguments << mUi->txtOutputDirectory->text().append(KROUTEPCDDIRECTORY).append(mUi->txtCapture->text()).append(".pcd");

	m3dViewer->start(QDir::current().absoluteFilePath("viewer"), arguments);
	m3dViewer->waitForFinished();
	if(m3dViewer->readAllStandardError().length() > 0){
		QMessageBox::information(this, "Error", "Cannot open pcd file");
	}
}

void captureToolPage::initTool(){
	//Check parameters
	if(mUi->txtOutputDirectory->text().isEmpty()){
		//throw window error
		QMessageBox msgBox(this);
		msgBox.setText("You must introduce output directory.");
		msgBox.exec();
	}else{
		mUi->pbInitTool->setEnabled(false);
		mUi->pbCapture->setEnabled(true);
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

