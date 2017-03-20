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
    void hasCmd(const QString &cmd); //当有要执行的命令式发出信号
    void pickcmdTriggered();         //当按下拷贝所有
public slots:
    void addOutput(const QString &text); 
    void addInputInfo(const QString &text);   //用于输出本次命令执行情况
    void addInputCmd(const QString &cmd);     //用于显示已经执行后的命令
    void clearInput();                        //清空input
    void clearOutput();                       //清空output
private:
    void initData();
    void initWindow();
    void initInputEdit(); 
    void initOutputEdit();
    void initLayout();
    void setConnect();
};

#endif // FINPUTOUTPUTWIDGET_H
