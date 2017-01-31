/*
 * labelToolPage.cpp
 *
 *  Created on: 28/01/2017
 *      Author: john
 */

#include <gui/pages/labelToolPage.h>

const QString labelToolPage::KIMAGEFORMATS = tr("Images (*.png )");
const QString labelToolPage::KINPUTFORMATS = tr("Images (*.csv *.txt)");
const QString labelToolPage::KOUTPUTIMAGEFORMATS = tr("Images (*.png)");
const char labelToolPage::KSEPARATOR=':';

labelToolPage::labelToolPage(QWidget *parent) :
QWidget(parent),
mUi(new Ui::labelToolPage){
	mUi->setupUi(this);
	mSelectObjectDialog = new selectObjectDialog();
	mLabelToolProcess = new QProcess(this);
	connect(mUi->pbReturnMainPage, SIGNAL(clicked()), this, SLOT(goMainPage()));
	connect(mUi->pbSetDataset, SIGNAL(clicked()), this, SLOT(setDataset()));
	connect(mUi->pbSetOutput, SIGNAL(clicked()), this, SLOT(setOutput()));
	connect(mUi->pbExecute, SIGNAL(clicked()), this, SLOT(labelDataset()));
	connect(mLabelToolProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyOutputMsgProcess()));
	connect(mLabelToolProcess, SIGNAL(readyReadStandardError()), this, SLOT(readyErrorMsgProcess()));
	connect(mUi->pbAddMask, SIGNAL(clicked()), this, SLOT(addMask()));
	connect(mUi->listAddedMasks, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectObjectMask(QListWidgetItem*)));
}

void labelToolPage::readyErrorMsgProcess(){
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
}

void labelToolPage::selectObjectMask(QListWidgetItem* item){
	mSelectObjectDialog->setModal(true);
	QString name = mSelectObjectDialog->selectName(mUi->listAddedMasks, item);
	if(!name.isNull()){
		int index = item->text().indexOf(KSEPARATOR);
		if(index == -1)
			item->setText(item->text().append(KSEPARATOR).append(name));
		else if(name != selectObjectDialog::KDELETENAME){
			mUi->listAddedMasks->removeItemWidget(item);
			item->setText(item->text().left(index).append(KSEPARATOR).append(name));
		}else{
			delete mUi->listAddedMasks->takeItem(mUi->listAddedMasks->row(item));
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
	QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open File"),
			mLastOutput.toLocal8Bit().constData(),
			KOUTPUTIMAGEFORMATS);
	if(!fileName.isNull()){
		mLastOutput = fileName;
		mUi->txtOutput->setText(fileName);
	}
}

void labelToolPage::goMainPage(){
	gui::goToPage("mainPage");
}

labelToolPage::~labelToolPage() {
	delete mUi;
}

void labelToolPage::addMask(){
	QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open File"),
			mLastMask.toLocal8Bit().constData(),
			KIMAGEFORMATS);
	if(!fileName.isNull()){
		mLastMask = fileName;
		new QListWidgetItem(fileName, mUi->listAddedMasks);
		//mUi->listAddedMasks->addItem(fileName);
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

