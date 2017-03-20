#include "FInputWidget.h"
#include <QKeyEvent>
#include <QMenu>
#include <QClipboard>
#include <QTextBlock>
#include <QDebug>

FInputWidget::FInputWidget(QWidget *parent) :
    QTextEdit(parent), 
    Hint("input>")
{
    initData(); 
    initWindow(); 
}


void FInputWidget::initData()
{
    initCmdList(); 
} 

void FInputWidget::initCmdList()
{
    cmdList.push_back("open image "); 
    cmdList.push_back("deal image "); 
    cmdList.push_back("clear input"); 
    cmdList.push_back("clear output"); 
    cmdList.push_back("exit"); 
}

void FInputWidget::initWindow()
{
    this->setFont(QFont("新宋体", 10, 100)); //该字体的字体宽度一致
    this->setCursorWidth(7); 
    this->append(Hint); 
}

void FInputWidget::mousePressEvent(QMouseEvent *e)
{
    if (this->isReadOnly())
    {
        QTextEdit::mousePressEvent(e); 
    }
}

void FInputWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (this->isReadOnly())
    {
        QTextEdit::mousePressEvent(e); 
    }
}

void FInputWidget::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu *menu = new QMenu(this); 
    QAction *action_copy = new QAction(tr("复制"), this); 
    QAction *action_paste = new QAction(tr("粘贴"), this); 
    QAction *action_edit = new QAction(tr("编辑模式"), this); 
    QAction *action_cmd = new QAction(tr("命令模式"), this); 
    QAction *action_pickcmd = new QAction(tr("提取所有命令"), this); 
    menu->addAction(action_copy); 
    menu->addAction(action_paste); 
    menu->addSeparator(); 
    menu->addAction(action_edit); 
    menu->addAction(action_cmd); 
    menu->addSeparator(); 
    menu->addAction(action_pickcmd); 
    
    if (this->isReadOnly())
    {
        action_paste->setDisabled(true);  
    }
    else
    {
        action_copy->setDisabled(true); 
    }
    
    QAction *action = menu->exec(QCursor::pos()); 
    if (action == action_copy)
    {
        clipboard->setText(this->textCursor().selectedText()); 
    }
    else if (action == action_paste)
    {
        moveCursor(QTextCursor::End, QTextCursor::MoveAnchor); 
        //粘贴时把回车符换成跳格符，并将文件斜杠格式统一
        insertPlainText(clipboard->text().replace('\n', '\t').replace('\\', '/')); 
    }
    else if (action == action_edit)
    {
        this->setReadOnly(true); 
    }
    else if (action == action_cmd)
    {
        this->setReadOnly(false); 
        moveCursor(QTextCursor::End, QTextCursor::MoveAnchor); 
    }
    else if (action == action_pickcmd)
    {
        emit pickcmdTriggered();    
    }
    else
    {}
    
    delete action_copy; 
    delete action_paste; 
    delete action_edit; 
    delete action_cmd; 
    delete menu; 
}

void FInputWidget::keyPressEvent(QKeyEvent *e)
{
    QString preCmd = getCurCmd(); 
    QTextBlock preBlock = this->textCursor().block(); 
    
    switch(e->key())
    {
    case Qt::Key_Return: eventOnKeyReturn(e); break; 
    case Qt::Key_Enter: eventOnKeyReturn(e); break;   
    case Qt::Key_Tab: eventOnKeyTab(e); break; 
    default: 
    {
        eventOnKeyOther(e); 
        QTextCursor curCursor= this->textCursor(); 
        QTextBlock curBlock = curCursor.block(); 
        //如果光标的位置非法，则重置光标位置
        if (curBlock.blockNumber() != preBlock.blockNumber()
                || curCursor.positionInBlock() - Hint.length() < 0)
        {
            resetCursor(preCmd); 
        }
        break; 
    }
    }
}

void FInputWidget::eventOnKeyReturn(QKeyEvent *e)
{
    execCurCmd(); 
}

void FInputWidget::eventOnKeyTab(QKeyEvent *e)
{
    static bool isMatched = false; //上次匹配是否成功
    
    if (isMatched && getCurCmd() == *iterCmdMatch) //上次成功匹配并且上次匹配后没有更改命令
    {
        if (++iterCmdMatch == cmdMatchList.end())
        {
            iterCmdMatch = cmdMatchList.begin(); 
        }
        setCurCmd(*iterCmdMatch); 
        isMatched = true; 
    }
    else
    {
        makeCmdMatchList(getCurCmd()); 
        if (!cmdMatchList.isEmpty())  //上次匹配失败并且本次匹配成功
        {
            setCurCmd(*iterCmdMatch); 
            isMatched = true; 
        }
        else                          //上次匹配失败并且本次匹配失败
        {
            isMatched = false; 
        }
    }
}

void FInputWidget::eventOnKeyOther(QKeyEvent *e)
{
    if (  e->matches(QKeySequence::Cut)
          || e->matches(QKeySequence::Undo)
          || e->matches(QKeySequence::Redo)
          || e->matches(QKeySequence::SelectAll))
    {
        ;
    }
    else if (e->matches(QKeySequence::Paste)
             && !this->isReadOnly())
    {
        //粘贴时把回车符换成跳格符，并将文件斜杠格式统一
        insertPlainText(clipboard->text().replace('\n', '\t').replace('\\', '/')); 
    }
    else
    {
        QTextEdit::keyPressEvent(e); 
    }
}

void FInputWidget::clearCurCmd()
{
    QTextCursor cursor = this->textCursor(); 
    cursor.select(QTextCursor::BlockUnderCursor); 
    cursor.removeSelectedText(); 
    this->setTextCursor(cursor); 
    this->append(Hint); 
}

QString FInputWidget::getCurCmd()
{
    return this->textCursor().block().text().mid(Hint.length()); 
}

void FInputWidget::setCurCmd(const QString &cmd)
{
    clearCurCmd(); 
    this->insertPlainText(cmd); 
}

void FInputWidget::execCurCmd()
{
    QString curCmd = getCurCmd(); 
    moveCursor(QTextCursor::End, QTextCursor::MoveAnchor); 
    emit hasCmd(curCmd); 
    this->append(Hint); 
}

void FInputWidget::addCmd(const QString &cmd)
{
    QString oldCmd = getCurCmd(); 
    clearCurCmd(); 
    this->insertPlainText(cmd); 
    this->append(Hint + oldCmd); 
}

void FInputWidget::makeCmdMatchList(const QString &cmd)
{
    cmdMatchList.clear();     
    for (QList<QString>::iterator iter = cmdList.begin(); 
         iter != cmdList.end(); iter++)
    {
        if (iter->startsWith(cmd, Qt::CaseInsensitive))
        {
            cmdMatchList.push_back(*iter); 
        }
    }
    
    iterCmdMatch = cmdMatchList.begin(); 
}

void FInputWidget::resetCursor(const QString &preCmd)
{
    this->moveCursor(QTextCursor::End); 
    
    setCurCmd(preCmd); 
    QTextCursor cursor = this->textCursor(); 
    cursor.movePosition(QTextCursor::StartOfBlock); 
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, Hint.length()); 
    this->setTextCursor(cursor); 
}
