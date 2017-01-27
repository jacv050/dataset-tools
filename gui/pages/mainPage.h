/*
 * wmain.h
 *
 *  Created on: 25/01/2017
 *      Author: john
 */

#ifndef WMAIN_H_
#define WMAIN_H_

#include <QtWidgets>
#include "ui_mainPage.h"
#include "gui/gui.h"

/**
 * Page to select the tool
 */
class mainPage : public QWidget {
	Q_OBJECT
public:
	mainPage(QWidget* parent = 0);
	virtual ~mainPage();

private:
	Ui::mainPage *mUi;

private slots:
	void goSubstracToolPage();
	void goLabelToolPage();
};

#endif /* WMAIN_H_ */
