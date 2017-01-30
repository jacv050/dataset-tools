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
#include <QString>
#include <fstream>
#include <vector>

class selectObjectDialog : public QDialog{
	Q_OBJECT
public:
	selectObjectDialog();
	void loadDataset(const QString& filePath);
	virtual ~selectObjectDialog();

private:
	QVBoxLayout *mVerticalLayout;
	QListWidget *mDatasetList;
	static const char KSEPARATOR;
};

#endif /* SELECTOBJECTDIALOG_H_ */
