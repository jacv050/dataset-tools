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
#include "gui/gui.h"

class captureToolPage : public QWidget {
	Q_OBJECT
public:
	captureToolPage(QWidget *parent = 0);
	virtual ~captureToolPage();

public slots:
	void goMainPage();

private:
	Ui::captureToolPage *mUi;
};

#endif /* CAPTURETOOLPAGE_H_ */
