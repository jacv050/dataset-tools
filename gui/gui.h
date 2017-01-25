/*
 * gui.h
 *
 *  Created on: 25/01/2017
 *      Author: john
 */

#ifndef GUI_H_
#define GUI_H_

#include <qmainwindow.h>
#include <qstackedwidget.h>

class gui : public QMainWindow {
	Q_OBJECT
public:
	gui();
	virtual ~gui();
private:
	QWidget *centralWidget;
	QStackedWidget *stackedWidget;
};

#endif /* GUI_H_ */
