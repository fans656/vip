#pragma once
#include<QtGui>

class FSetImageSizeDialog : public QDialog
{
public:
	FSetImageSizeDialog(void);
	~FSetImageSizeDialog(void);
	void createSetImageSizeDialog();
	QLineEdit *widthLineEdit;
	QLineEdit *heightLineEdit;
	QPushButton *surePushButton;
signals:
	
private:
	QLabel *widthLabel;
	QLabel *heightLabel;
	
	
	QPushButton *cancelPushButton;
};
