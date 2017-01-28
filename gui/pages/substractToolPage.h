/*
 * substractToolPage.h
 *
 *  Created on: 26/01/2017
 *      Author: john
 */

#ifndef SUBSTRACTTOOLPAGE_H_
#define SUBSTRACTTOOLPAGE_H_

#include <QtWidgets>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>

#include <iostream>

#include "ui_substractToolPage.h"
#include "gui/gui.h"
#include <vector>

class substractToolPage : public QWidget {
	Q_OBJECT
public:
	substractToolPage(QWidget *parent=0);
	virtual ~substractToolPage();

public slots:
	//Button event to return main page
	void goMainPage();
	//Button event to set the background image
	void setBackgroundImage();
	//Button event to set the object image
	void setObjectImage();
	//Button evet to set the mask name destiny
	void setMaskNameDestiny();
	//Button event to execute the tool to substract the image
	void substractObject();
	//Update message
	void readyOutputMsgProcess();
	//Event checkbox substract mode
	void substractModeChanged();

private:
	Ui::substractToolPage *mUi;
	QProcess *mImageSubstractProcess;

	QString mLastPageBackground;
	QString mLastPageObject;
	QString mLastMaskDestiny;

	static const QString KIMAGEFORMATS;
	static const QString KOUTPUTIMAGEFORMATS;
};

#endif /* SUBSTRACTTOOLPAGE_H_ */
