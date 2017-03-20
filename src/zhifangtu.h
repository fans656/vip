#ifndef ZHIFANGTU_H
#define ZHIFANGTU_H
#include "ArithmeticBase.h"
#include <QImage>
#include <QObject>
#include "HistoGram.h"

class Zhifangtu : public ArithmeticBase<QImage>
{
public:
    Zhifangtu(); 
    void setParam(const QList<QImage> &dataList, const QList<QString> &paramList); 
private:
    int EqualMod; //直方图均衡的算法种类
    double MValue; //根据强度来进行均衡
    QImage sourceImage; 
    
    virtual void dealData(); 
};

#endif // ZHIFANGTU_H
