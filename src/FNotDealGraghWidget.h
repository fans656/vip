#pragma once
#include <QtGui>

class FNotDealGraghWidget: public QWidget							//未处理图片类
{
	Q_OBJECT
public:
	FNotDealGraghWidget();
	~FNotDealGraghWidget();
	void notDealGraghWidgetLayout();							    //未处理窗口布局
	void initNotDealGraghWidget();									//初始化未处理窗口
	//一下空间定义为公有
	QListWidget *notDealGraghList;
	QPushButton *showGraghButton;						 //显示图片按钮

private:
	QPushButton *deleteGraghButton;
	QPushButton *dealGraghButton;
};
