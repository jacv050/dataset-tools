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
	void addPage(QWidget *);
	QWidget* getParentPages() const;

	virtual ~gui();
private:
	QWidget *mCentralWidget;
	QStackedWidget *mStackedWidget;

	static const int KINITPAGE = 0;
	static const int KINITWIDT = 600;
	static const int KINITHEIGHT = 400;
};

#endif /* GUI_H_ */
