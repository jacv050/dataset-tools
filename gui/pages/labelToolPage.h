/*
 * labelToolPage.h
 *
 *  Created on: 28/01/2017
 *      Author: john
 */

#ifndef LABELTOOLPAGE_H_
#define LABELTOOLPAGE_H_

#include "ui_labelToolPage.h"
#include "gui/gui.h"
#include <QWidget>
#include <QProcess>
#include <QStringList>
#include <QFileDialog>
#include <QDialog>
#include "selectObjectDialog.h"

class labelToolPage : public QWidget{
	Q_OBJECT
public:
	labelToolPage(QWidget *parent = 0);
	virtual ~labelToolPage();

public slots:
	//Button event to return main page
	void goMainPage();
	//Button event to label scene
	void labelDataset();
	//Button event to select dataset
	void setDataset();
	//Button event to select output
	void setOutput();
	//Update message
	void readyOutputMsgProcess();
	//Update error message
	void readyErrorMsgProcess();
	//Add mask
	void addMask();
	//Select object in list widget
	void selectObjectMask(QListWidgetItem* item);
	//Delete all item in listWidget
	void deleteAllItems();

private:
	Ui::labelToolPage *mUi;
	QProcess *mLabelToolProcess;
	selectObjectDialog *mSelectObjectDialog;

	QString mLastDataset;
	QString mLastOutput;
	QString mLastMask;

	static const QString KIMAGEFORMATS;
	static const QString KINPUTFORMATS;
	static const QString KOUTPUTIMAGEFORMATS;
	static const char KSEPARATOR;
};

#endif /* LABELTOOLPAGE_H_ */
