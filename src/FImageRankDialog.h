#pragma once
#include <QtGui>
#include "stdafx.h"

class FNotDealImageWidget; 

class FImageRankDialog : public QDialog            //ͼƬ����
{
	Q_OBJECT

public:
    FImageRankDialog(QStringList paths);
    const QStringList rearrangedPaths() const;

private slots:
    void upImageSlot();											//ͼƬ����
    void downImageSlot();									    //ͼƬ����

private:
	QListWidget *imageListWidget;
	QPushButton *upButton;
	QPushButton *downButton;
    QPushButton *okButton;

	void initWindow();                          //��ʼ���Ի������
    void initImageData(QStringList paths);
};
