#pragma once
#include <QtGui>

class FArithmeticListManageDialog: public QDialog
{
	Q_OBJECT
public:
	FArithmeticListManageDialog();
	~FArithmeticListManageDialog();
	void createDialog();
	QLineEdit *nameLineEdit;
private:
	QPushButton *cancelButton;
	QPushButton *saveButton;
	QWidget *arithmeticListSaveWidget;
	QString ArithmeticName;
	QLabel * nameLabel;

private slots:	
	void ArithmeticListSave();			//确认保存算法序列
	void ArithmeticListCancel();		//取消保存算法序列
};
