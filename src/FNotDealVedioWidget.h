#pragma once
#include <QtGui>

class FNotDealVedioWidget: public QWidget							//未处理图片类
{
	Q_OBJECT
public:
	FNotDealVedioWidget();
	~FNotDealVedioWidget();
	void notDealVedioWidgetLayout();							    //未处理窗口布局
	void initNotDealVedioWidget();									//初始化未处理窗口
	//一下空间定义为公有
	QListWidget *notDealVedioList;
	QPushButton *playVedioButton;						 //显示图片按钮

private:
	QPushButton *deleteVedioButton;
	QPushButton *dealVedioButton;
};
