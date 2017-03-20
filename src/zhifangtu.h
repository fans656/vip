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
    int EqualMod; //ֱ��ͼ������㷨����
    double MValue; //����ǿ�������о���
    QImage sourceImage; 
    
    virtual void dealData(); 
};

#endif // ZHIFANGTU_H
