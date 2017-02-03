/*
 * captureToolPage.h
 *
 *  Created on: 31/01/2017
 *      Author: john
 */

#ifndef CAPTURETOOLPAGE_H_
#define CAPTURETOOLPAGE_H_

#include "ui_captureToolPage.h"
#include <QtWidgets>
#include <QProcess>
#include <QPixmap>
#include "gui/gui.h"
#include "gui/pages/ImageViewerDialog.h"

class captureToolPage : public QWidget {
	Q_OBJECT
public:
	captureToolPage(QWidget *parent = 0);
	virtual ~captureToolPage();

public slots:
	void goMainPage();
	void setOutputDirectory();
	void doCapture();
	void showColorImage();
	void showDepthImage();
	void showPCD();
	void initTool();

	void closedProcess();
	void readyOutputMsgProcess();

private:

	Ui::captureToolPage *mUi;
	QString mRouteColorImage;
	QString mRouteDepthImage;
	QString mRoutePCD;
	QString mLastOutputDirectory;

	QProcess *mCaptureToolProcess;

	ImageViewerDialog *mImgColorViewer;
	ImageViewerDialog *mImgDepthViewer;

	static const QString KROUTECOLORDIRECTORY;
	static const QString KROUTEDEPTHDIRECTORY;
	static const QString KROUTEPCDDIRECTORY;
};

#endif /* CAPTURETOOLPAGE_H_ */
