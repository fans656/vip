#ifndef FARITHMETICINFODIALOG_H
#define FARITHMETICINFODIALOG_H
#include <QtGui>

class FArithmeticInfoDialog : public QDialog
{
    Q_OBJECT
public:
    FArithmeticInfoDialog(QString aName);

    bool isParamNeedSet(); //����ֵ��ʾ���㷨�Ĳ��������Ƿ�Ϊ0,���費��Ҫ�����˶Ի���
    QList<QString> getParamList(); //�����û�����Ĳ�������
    
private:
    QString name; 
    int paramInfoNumber; 
    QList<QString> paramDescriptionList; 
    QList<QString> paramTypeList; 
    QList<QString> paramMinList; 
    QList<QString> paramMaxList; 
    QList<QString> paramList; 
    
    QList<QLineEdit *> lineEditList; //�������û����ȷ��ʱ����һ��ȡ������Ϣ
    QPushButton *pushButton; 
    
    void readInfo(); //�����ݿ�����ȡ��Ϣ
    void setParamInfoList(QList<QString> &paramList, 
                          const QString &paramString); 
    void initWindow(); 
    void setConnect(); 
    void setParamList(); 
    
private slots:
    void onPushButtonSlot(); 
};

#endif // FARITHMETICINFODIALOG_H
