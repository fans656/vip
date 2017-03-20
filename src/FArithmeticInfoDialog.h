#ifndef FARITHMETICINFODIALOG_H
#define FARITHMETICINFODIALOG_H
#include <QtGui>

class FArithmeticInfoDialog : public QDialog
{
    Q_OBJECT
public:
    FArithmeticInfoDialog(QString aName);

    bool isParamNeedSet(); //返回值表示此算法的参数个数是否不为0,即需不需要弹出此对话框
    QList<QString> getParamList(); //返回用户输入的参数序列
    
private:
    QString name; 
    int paramInfoNumber; 
    QList<QString> paramDescriptionList; 
    QList<QString> paramTypeList; 
    QList<QString> paramMinList; 
    QList<QString> paramMaxList; 
    QList<QString> paramList; 
    
    QList<QLineEdit *> lineEditList; //用来当用户点击确定时，逐一提取参数信息
    QPushButton *pushButton; 
    
    void readInfo(); //从数据库中提取信息
    void setParamInfoList(QList<QString> &paramList, 
                          const QString &paramString); 
    void initWindow(); 
    void setConnect(); 
    void setParamList(); 
    
private slots:
    void onPushButtonSlot(); 
};

#endif // FARITHMETICINFODIALOG_H
