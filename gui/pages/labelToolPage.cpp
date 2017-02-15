/*
 * labelToolPage.cpp
 *
 *  Created on: 28/01/2017
 *      Author: john
 */

#include <gui/pages/labelToolPage.h>

const QString labelToolPage::KIMAGEFORMATS("Images (*.png )");
const QString labelToolPage::KINPUTFORMATS("Images (*.csv *.txt)");
const QString labelToolPage::KOUTPUTIMAGEFORMATS("Images (*.png)");
const char labelToolPage::KSEPARATOR=';';

labelToolPage::labelToolPage(QWidget *parent) :
QWidget(parent),
mUi(new Ui::labelToolPage){
	mUi->setupUi(this);
	mImageViewer = new imageViewerDialog();
	mSelectObjectDialog = new selectObjectDialog();
	mLabelToolProcess = new QProcess(this);
	mLastOutput = QDir::currentPath() + QDir::separator() + tr("pngIndexed.png");
	mUi->txtOutput->setText(mLastOutput);
	mLastDataset = QDir::currentPath() + QDir::separator() + tr("dataset.csv");
	mUi->txtDataset->setText(mLastDataset);
	mSelectObjectDialog->loadDataset(mLastDataset);
	connect(mUi->pbReturnMainPage, SIGNAL(clicked()), this, SLOT(goMainPage()));
	connect(mUi->pbSetDataset, SIGNAL(clicked()), this, SLOT(setDataset()));
	connect(mUi->pbSetOutput, SIGNAL(clicked()), this, SLOT(setOutput()));
	connect(mUi->pbExecute, SIGNAL(clicked()), this, SLOT(labelDataset()));
	connect(mLabelToolProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyOutputMsgProcess()));
	connect(mLabelToolProcess, SIGNAL(readyReadStandardError()), this, SLOT(readyErrorMsgProcess()));
	connect(mUi->pbAddMask, SIGNAL(clicked()), this, SLOT(addMask()));
	connect(mUi->listAddedMasks, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectObjectMask(QListWidgetItem*)));
	connect(mUi->pbDeleteAll, SIGNAL(clicked()), this, SLOT(deleteAllItems()));
	connect(mUi->pbShowOutput, SIGNAL(clicked()), this, SLOT(showOutput()));
}

void labelToolPage::showOutput(){
	mImageViewer->showImage(mUi->txtOutput->text());
}

void labelToolPage::deleteAllItems(){
	mUi->listAddedMasks->clear();
}

void labelToolPage::readyErrorMsgProcess(){
	mUi->pbShowOutput->setEnabled(false);
	QString s("<font color=red>");
	s.append(tr(mLabelToolProcess->readAllStandardError()));
	s.append(tr("</font><br><br>"));
	mUi->txtOutputProcess->insertHtml(s);
}

void labelToolPage::readyOutputMsgProcess(){
	QString s("<font color=black>");
	s.append(tr(mLabelToolProcess->readAllStandardOutput()));
	s.append(tr("</font><br><br>"));
	mUi->txtOutputProcess->insertHtml(s);
	mUi->pbShowOutput->setEnabled(true);
}

void labelToolPage::selectObjectMask(QListWidgetItem* item){
	mSelectObjectDialog->setModal(true);
	QString name = mSelectObjectDialog->selectName(mUi->listAddedMasks, item);
	if(!name.isNull()){
		if(name == selectObjectDialog::KDELETENAME)
			delete mUi->listAddedMasks->takeItem(mUi->listAddedMasks->row(item));
		else{
			int index = item->text().indexOf(KSEPARATOR);
			if(index == -1){
				item->setText(item->text().append(KSEPARATOR).append(name));
			}else{
				mUi->listAddedMasks->removeItemWidget(item);
				item->setText(item->text().left(index).append(KSEPARATOR).append(name));
			}
		}
	}
}

void labelToolPage::setDataset(){
	QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open File"),
			mLastDataset.toLocal8Bit().constData(),
			KINPUTFORMATS);
	if(!fileName.isNull()){
		mLastDataset = fileName;
		mUi->txtDataset->setText(fileName);
		mSelectObjectDialog->loadDataset(fileName);
	}
}

void labelToolPage::setOutput(){
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setNameFilter(KOUTPUTIMAGEFORMATS);
	dialog.setViewMode(QFileDialog::Detail);
	if(!mLastOutput.isEmpty())
		dialog.setDirectory(mLastOutput);

	QStringList fileNames;
	if (dialog.exec()){
	    fileNames = dialog.selectedFiles();
	    if(!fileNames.at(0).isNull()){
	    	mLastOutput = fileNames.at(0);
	    	mUi->txtOutput->setText(fileNames.at(0));
		}
	}
}

void labelToolPage::goMainPage(){
	gui::goToPage("mainPage");
}

labelToolPage::~labelToolPage() {
	delete mUi;
}

void labelToolPage::addMask(){
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setNameFilter(KIMAGEFORMATS);
	dialog.setViewMode(QFileDialog::Detail);
	if(!mLastMask.isEmpty())
		dialog.setDirectory(mLastMask);

	QStringList fileNames;
	if (dialog.exec()){
	    fileNames = dialog.selectedFiles();
	    for(int i=0; i<fileNames.size(); ++i){
		    if(!fileNames.at(i).isNull()){
		    	mLastMask = fileNames.at(i);
		    	mUi->listAddedMasks->addItem(fileNames.at(i));
			}
	    }
	}
}

void labelToolPage::labelDataset(){
	//Check parameters
	if(mUi->txtDataset->text().isEmpty() ||
			mUi->txtOutput->text().isEmpty() ||
			mUi->listAddedMasks->count() == 0){
		//throw window error
		QMessageBox msgBox(this);
		msgBox.setText("You must introduce dataset route, out file and masks.");
		msgBox.exec();
	}else{
		QStringList arguments;
		arguments << QDir::currentPath() + QDir::separator() + "indexPngDataset.py";
		arguments << mUi->txtDataset->text();
		arguments << mUi->txtOutput->text();
		for(int i=0; i<mUi->listAddedMasks->count(); ++i){
			arguments << mUi->listAddedMasks->item(i)->text();
		}

		if(mLabelToolProcess->isOpen()){
			mLabelToolProcess->kill();
			mLabelToolProcess->waitForFinished();
		}

		mLabelToolProcess->start("python", arguments);
	}
}

