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
    QString getCurCmd();             //�õ���ǰδִ�е�cmd��������input>
    void setCurCmd(const QString &cmd);    //��յ�ǰ��cmd�����õ�ǰ��cmd
    void clearCurCmd();                    //��յ�ǰ��cmd
    void execCurCmd();                     //ִ�е�ǰ��cmd
    void addCmd(const QString &cmd);       //�ڵ�ǰcmd֮ǰ�����Ѿ��������ط�ִ�к��cmd
    
signals:
    void hasCmd(const QString &cmd); //����Ҫִ�е�����ʽ�����ź�
    void pickcmdTriggered();         //�����¿�������
    
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
    void resetCursor(const QString &preCmd); //��input>����д�룬���������������Ŀ�ͷ
    void eventOnKeyReturn(QKeyEvent *e); 
    void eventOnKeyTab(QKeyEvent *e); 
    void eventOnKeyOther(QKeyEvent *e);      //���������¼�
    
    const QString Hint;           //"input>"����Ҫ�ı�
    QList<QString> cmdList;       //Ԥ���cmd����
    QList<QString> cmdMatchList;  //��tab������ʾ������
    QList<QString>::iterator iterCmdMatch;  //������
    QClipboard *clipboard;        //ճ����
};

#endif // FINPUTWIDGET_H
