#ifndef FINPUTOUTPUTWIDGET_H
#define FINPUTOUTPUTWIDGET_H

#include <QWidget>

class QTextEdit; 
class FInputWidget; 

class FInputOutputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FInputOutputWidget(QWidget *parent = 0);
    
    FInputWidget *inputEdit; 
    QTextEdit *outputEdit; 
signals:
    void hasCmd(const QString &cmd); //����Ҫִ�е�����ʽ�����ź�
    void pickcmdTriggered();         //�����¿�������
public slots:
    void addOutput(const QString &text); 
    void addInputInfo(const QString &text);   //���������������ִ�����
    void addInputCmd(const QString &cmd);     //������ʾ�Ѿ�ִ�к������
    void clearInput();                        //���input
    void clearOutput();                       //���output
private:
    void initData();
    void initWindow();
    void initInputEdit(); 
    void initOutputEdit();
    void initLayout();
    void setConnect();
};

#endif // FINPUTOUTPUTWIDGET_H
