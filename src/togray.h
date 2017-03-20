#ifndef TOGRAY_H
#define TOGRAY_H
#include <ArithmeticBase.h>
#include <QImage>

class ToGray : public ArithmeticBase<QImage>
{
public:
    ToGray(); 
    void setParam(const QList<QImage> &dataList, const QList<QString> &paramList); 
private:
    QImage sourceImage; 
    
    virtual void dealData(); 
};

#endif // TOGRAY_H
