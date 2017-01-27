/*
 * substractToolPage.h
 *
 *  Created on: 26/01/2017
 *      Author: john
 */

#ifndef SUBSTRACTTOOLPAGE_H_
#define SUBSTRACTTOOLPAGE_H_

#include <QtWidgets>
#include "ui_substractToolPage.h"
#include "gui/gui.h"

class substractToolPage : public QWidget {
	Q_OBJECT
public:
	substractToolPage(QWidget *parent=0);
	virtual ~substractToolPage();

public slots:
	void goMainPage();
	void setBackgroundImage();
	void setObjectImage();
	void substractObject();

private:
	Ui::substractToolPage *ui;
};

#endif /* SUBSTRACTTOOLPAGE_H_ */
