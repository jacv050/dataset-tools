/*
 * imageViewerDialog.cpp
 *
 *  Created on: 02/02/2017
 *      Author: john
 */

#include <gui/pages/imageViewerDialog.h>

imageViewerDialog::imageViewerDialog() {
	mVboxLayout = new QVBoxLayout(this);
	mScrollArea = new QScrollArea(this);
	mImageLabel = new QLabel(this);

	mVboxLayout->addWidget(mScrollArea);

    mImageLabel->setBackgroundRole(QPalette::Base);
    mImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    mImageLabel->setScaledContents(true);

    mScrollArea->setBackgroundRole(QPalette::Dark);
    mScrollArea->setWidget(mImageLabel);
    mScrollArea->setVisible(true);
    mScrollArea->setWidgetResizable(true);

}

void imageViewerDialog::showImage(const QString& path){
	QImageReader reader(path);
	QImage image = reader.read();
	if(image.isNull()){
		QMessageBox::information(this, "Image not load", tr("Cannot load image."));
	}else{
		mImageLabel->setPixmap(QPixmap::fromImage(image));
		show();
	}
}

imageViewerDialog::~imageViewerDialog() {
}

