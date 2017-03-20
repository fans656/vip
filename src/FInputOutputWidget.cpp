#include "FInputOutputWidget.h"
#include "FInputWidget.h"
#include <QTextEdit>
#include <QBoxLayout>
#include <QSplitter>
#include <QTime>
#include <QDebug>

FInputOutputWidget::FInputOutputWidget(QWidget *parent) :
    QWidget(parent)
{
    initData(); 
    initWindow(); 
    setConnect(); 
}

void FInputOutputWidget::initData()
{}

void FInputOutputWidget::initWindow()
{
    initInputEdit(); 
    initOutputEdit(); 
    initLayout(); 
}

void FInputOutputWidget::initInputEdit()
{
    inputEdit = new FInputWidget(this); 
}

void FInputOutputWidget::initOutputEdit()
{
    outputEdit = new QTextEdit(this); 
    outputEdit->setReadOnly(true); 
    outputEdit->setFont(QFont("新宋体", 10, 10)); //该字体的字体宽度一致
}

void FInputOutputWidget::initLayout()
{
    QHBoxLayout *layout = new QHBoxLayout(this); 
    QSplitter *splitter = new QSplitter(Qt::Horizontal); 
    splitter->addWidget(inputEdit); 
    splitter->addWidget(outputEdit); 
    layout->addWidget(splitter); 
    this->setLayout(layout); 
}

void FInputOutputWidget::setConnect()
{
    connect(inputEdit, SIGNAL(hasCmd(QString)), 
            this, SIGNAL(hasCmd(QString))); 
    connect(inputEdit, SIGNAL(pickcmdTriggered()), 
            this, SIGNAL(pickcmdTriggered())); 
}

void FInputOutputWidget::addOutput(const QString &text)
{
    outputEdit->append(QTime::currentTime().toString("(hh:mm:ss)") 
                       + "output>" + text); 
}

void FInputOutputWidget::addInputInfo(const QString &text)
{
    inputEdit->append(text); 
}

void FInputOutputWidget::addInputCmd(const QString &cmd)
{
    inputEdit->addCmd(cmd); 
}

void FInputOutputWidget::clearInput()
{
    inputEdit->clear(); 
}

void FInputOutputWidget::clearOutput()
{
    outputEdit->clear(); 
}
