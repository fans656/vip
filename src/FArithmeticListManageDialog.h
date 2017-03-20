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
	void ArithmeticListSave();			//ȷ�ϱ����㷨����
	void ArithmeticListCancel();		//ȡ�������㷨����
};
