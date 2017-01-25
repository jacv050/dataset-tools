/*
 * wmain.h
 *
 *  Created on: 25/01/2017
 *      Author: john
 */

#ifndef WMAIN_H_
#define WMAIN_H_

#include <QtWidgets>
#include "ui_wmain.h"

class wmain : public QWidget {
public:
	wmain();
	virtual ~wmain();

private:
	Ui::DatasetTool ui;
};

#endif /* WMAIN_H_ */
