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
#include "gui/pages/mainPage.h"

class gui : public QMainWindow {
	Q_OBJECT

public:
	static void addPage(QWidget*);
	static QWidget* getParentPages();
	static void initClass();
	static void destroyClass();
	static void show();
	static void goToPage(const std::string&);
	static void setMinimumConstraint(int width, int height);
	static void setMaximumConstraint(int width, int height);
	static void resize(int width, int heihgt);

private:
	//static gui *mGui;
	static QMainWindow *mGui;
	gui();
	virtual ~gui();

	static QWidget *mCentralWidget;
	static QStackedWidget *mStackedWidget;

	static const int KINITPAGE = 0;
	static const int KINITWIDT = 600;
	static const int KINITHEIGHT = 400;

private slots:
	void indexChanged(int);

};

#endif /* GUI_H_ */
