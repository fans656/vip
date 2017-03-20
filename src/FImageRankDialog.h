#pragma once
#include <QtGui>
#include "stdafx.h"

class FNotDealImageWidget; 

class FImageRankDialog : public QDialog            //图片排序
{
	Q_OBJECT

public:
    FImageRankDialog(QStringList paths);
    const QStringList rearrangedPaths() const;

private slots:
    void upImageSlot();											//图片上移
    void downImageSlot();									    //图片下移

private:
	QListWidget *imageListWidget;
	QPushButton *upButton;
	QPushButton *downButton;
    QPushButton *okButton;

	void initWindow();                          //初始化对话框界面
    void initImageData(QStringList paths);
};
