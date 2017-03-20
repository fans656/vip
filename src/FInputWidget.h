#ifndef FINPUTWIDGET_H
#define FINPUTWIDGET_H

#include <QTextEdit>

class QFont; 
class QClipboard; 

class FInputWidget : public QTextEdit
{
    Q_OBJECT
public:
    explicit FInputWidget(QWidget *parent = 0);
    QString getCurCmd();             //得到当前未执行的cmd，不包括input>
    void setCurCmd(const QString &cmd);    //清空当前的cmd后设置当前的cmd
    void clearCurCmd();                    //清空当前的cmd
    void execCurCmd();                     //执行当前的cmd
    void addCmd(const QString &cmd);       //在当前cmd之前加入已经由其他地方执行后的cmd
    
signals:
    void hasCmd(const QString &cmd); //当有要执行的命令式发出信号
    void pickcmdTriggered();         //当按下拷贝所有
    
public slots:
    
protected:
    void mousePressEvent(QMouseEvent *e); 
    void mouseDoubleClickEvent(QMouseEvent *e); 
    void contextMenuEvent(QContextMenuEvent *e); 
    void keyPressEvent(QKeyEvent *e); 
    
private:
    void initData(); 
    void initCmdList(); 
    void initWindow(); 
    void makeCmdMatchList(const QString &cmd); 
    void resetCursor(const QString &preCmd); //将input>重新写入，并将光标置于命令的开头
    void eventOnKeyReturn(QKeyEvent *e); 
    void eventOnKeyTab(QKeyEvent *e); 
    void eventOnKeyOther(QKeyEvent *e);      //其他按键事件
    
    const QString Hint;           //"input>"，不要改变
    QList<QString> cmdList;       //预存的cmd命令
    QList<QString> cmdMatchList;  //按tab建会提示的命令
    QList<QString>::iterator iterCmdMatch;  //迭代器
    QClipboard *clipboard;        //粘贴板
};

#endif // FINPUTWIDGET_H
