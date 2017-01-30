/*
 * selectObjectDialog.h
 *
 *  Created on: 30/01/2017
 *      Author: john
 */

#ifndef SELECTOBJECTDIALOG_H_
#define SELECTOBJECTDIALOG_H_

#include <QDialog>
#include <QVBoxLayout>
#include <QListWidget>

class selectObjectDialog : public QDialog{
	Q_OBJECT
public:
	selectObjectDialog();
	virtual ~selectObjectDialog();

private:
	QVBoxLayout *mVerticalLayout;
	QListWidget *mDatasetList;
};

#endif /* SELECTOBJECTDIALOG_H_ */
