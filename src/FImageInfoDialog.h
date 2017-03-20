#ifndef FIMAGEINFODIALOG_H
#define FIMAGEINFODIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;

class FImageInfoDialog : public QDialog
{
	Q_OBJECT

public:
    FImageInfoDialog(QWidget *parent = 0);
    void getDealtImagePath(QString beforeDealtImageName);		//获取处理前图片的路径

private:

	QLabel *picName;
	QLabel *picPath;
	QLabel *picSize;
	QLabel *createTime;
	QLabel *lastModify;
	QLabel *lastRead;
	QLineEdit *picNameLine;
	QLineEdit *picPathLine;
	QLineEdit *picSizeLine;
	QLineEdit *createTimeLine;
	QLineEdit *lastModifyLine;
	QLineEdit *lastReadLine;
	QString beforeDealtImagePath;
};

#endif
