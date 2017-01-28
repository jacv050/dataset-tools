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

class labelToolPage : public QWidget{
	Q_OBJECT
public:
	labelToolPage(QWidget *parent = 0);
	virtual ~labelToolPage();

public slots:
	//Button event to return main page
	void goMainPage();

private:
	Ui::labelToolPage *mUi;
};

#endif /* LABELTOOLPAGE_H_ */
