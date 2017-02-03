/*
 * ImageViewerDialog.cpp
 *
 *  Created on: 02/02/2017
 *      Author: john
 */

#include <gui/pages/ImageViewerDialog.h>

ImageViewerDialog::ImageViewerDialog() {
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

    //resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

void ImageViewerDialog::showImage(const QString& path){
	QImageReader reader(path);
	QImage image = reader.read();
	if(image.isNull()){
		QMessageBox::information(this, "Image not load", tr("Cannot load image."));
	}else{
		mImageLabel->setPixmap(QPixmap::fromImage(image));
		//imageLabel->resize(imageLabel->pixmap()->size());
		show();
	}
}

ImageViewerDialog::~ImageViewerDialog() {
}

