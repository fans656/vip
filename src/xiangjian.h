#ifndef XIANGJIAN_H
#define XIANGJIAN_H
#include <ArithmeticBase.h>
#include <QImage>

class XiangJian : public ArithmeticBase<QImage>
{
public:
	XiangJian(); 
	void setParam(const QList<QImage> &dataList, const QList<QString> &paramList); 
private:
	QImage sourceImageA;
	QImage sourceImageB;


	virtual void dealData(); 
};

#endif // XIANGJIAN_H
