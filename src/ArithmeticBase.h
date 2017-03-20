#ifndef ARITHMETIC_H
#define ARITHMETIC_H
#include <QList>
#include <QString>
#include <QObject>

template <typename T> //T表示数据的种类
class ArithmeticBase
{
public:
    QList<T> getFinalDataList()
    {
        finalDataList.clear(); 
        dealData(); 
        return finalDataList; 
    }
    QString getArithmeticName()
    {
        return arithName; 
    }
    virtual void setParam(const QList<T> &dataList, 
                          const QList<QString> &paramList) = 0; 
protected:
    QList<T> finalDataList; 
    QString arithName;    
    virtual void dealData() = 0; 
};

#endif // ARITHMETIC_H
