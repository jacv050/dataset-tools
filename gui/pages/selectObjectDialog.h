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
#include <QPushButton>

class selectObjectDialog : public QDialog{
	Q_OBJECT
public:
	selectObjectDialog();
	void loadDataset(const QString& filePath);
	QString selectName(QListWidget*, QListWidgetItem*);
	virtual ~selectObjectDialog();
	static const QString KDELETENAME;

public slots:
	void nameSelected(QListWidgetItem* item);
	//Event delete mask
	void deleteSelectedMask();


private:
	QListWidget *mListMaskAux;
	QListWidgetItem *mListMaskItemAux;

	bool mDelete;
	QVBoxLayout *mVerticalLayout;
	QListWidget *mDatasetList;
	QPushButton *mPbDelete;
	QString mNameSelected;
	static const char KSEPARATOR;
};

#endif /* SELECTOBJECTDIALOG_H_ */
