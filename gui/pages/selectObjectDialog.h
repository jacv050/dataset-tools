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
	QString selectName();
	virtual ~selectObjectDialog();

public slots:
	void nameSelected(QListWidgetItem* item);

private:
	QVBoxLayout *mVerticalLayout;
	QListWidget *mDatasetList;
	QString mNameSelected;
	static const char KSEPARATOR;
};

#endif /* SELECTOBJECTDIALOG_H_ */
