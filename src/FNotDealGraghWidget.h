#pragma once
#include <QtGui>

class FNotDealGraghWidget: public QWidget							//δ����ͼƬ��
{
	Q_OBJECT
public:
	FNotDealGraghWidget();
	~FNotDealGraghWidget();
	void notDealGraghWidgetLayout();							    //δ�����ڲ���
	void initNotDealGraghWidget();									//��ʼ��δ������
	//һ�¿ռ䶨��Ϊ����
	QListWidget *notDealGraghList;
	QPushButton *showGraghButton;						 //��ʾͼƬ��ť

private:
	QPushButton *deleteGraghButton;
	QPushButton *dealGraghButton;
};
