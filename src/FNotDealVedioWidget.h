#pragma once
#include <QtGui>

class FNotDealVedioWidget: public QWidget							//δ����ͼƬ��
{
	Q_OBJECT
public:
	FNotDealVedioWidget();
	~FNotDealVedioWidget();
	void notDealVedioWidgetLayout();							    //δ�����ڲ���
	void initNotDealVedioWidget();									//��ʼ��δ������
	//һ�¿ռ䶨��Ϊ����
	QListWidget *notDealVedioList;
	QPushButton *playVedioButton;						 //��ʾͼƬ��ť

private:
	QPushButton *deleteVedioButton;
	QPushButton *dealVedioButton;
};
