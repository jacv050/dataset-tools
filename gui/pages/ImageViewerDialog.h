/*
 * ImageViewerDialog.h
 *
 *  Created on: 02/02/2017
 *      Author: john
 */

#ifndef IMAGEVIEWERDIALOG_H_
#define IMAGEVIEWERDIALOG_H_

#include <QDialog>
#include <QImage>
#include <QImageReader>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>

class ImageViewerDialog : QDialog {
	Q_OBJECT
public:
	ImageViewerDialog();
	void showImage(const QString& path);
	virtual ~ImageViewerDialog();

private:
	QScrollArea *mScrollArea;
	QVBoxLayout *mVboxLayout;
	QLabel *mImageLabel;

};

#endif /* IMAGEVIEWERDIALOG_H_ */
