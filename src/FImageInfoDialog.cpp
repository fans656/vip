#include "FImageInfoDialog.h"

#include <QtGui>

FImageInfoDialog::FImageInfoDialog(QWidget *parent)
    : QDialog(parent)
{
    picName = new QLabel(tr("文件名：     "));
    picNameLine = new QLineEdit;
    picNameLine->setReadOnly(true);
    QHBoxLayout *hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(picName);
    hLayout1->addWidget(picNameLine);

    picPath = new QLabel(tr("路径:        "));
    picPathLine = new QLineEdit;
    picPathLine->setReadOnly(true);
    QHBoxLayout *hLayout2 = new QHBoxLayout;
    hLayout2->addWidget(picPath);
    hLayout2->addWidget(picPathLine);

    picSize = new QLabel(tr("大小:        "));
    picSizeLine = new QLineEdit;
    picSizeLine->setReadOnly(true);
    QHBoxLayout *hLayout3 = new QHBoxLayout;
    hLayout3->addWidget(picSize);
    hLayout3->addWidget(picSizeLine);

    createTime = new QLabel(tr("上次修改时间:"));
    createTimeLine = new QLineEdit;
    createTimeLine->setReadOnly(true);
    QHBoxLayout *hLayout4 = new QHBoxLayout;
    hLayout4->addWidget(createTime);
    hLayout4->addWidget(createTimeLine);

    lastModify = new QLabel(tr("上次修改时间:"));
    lastModifyLine = new QLineEdit;
    lastModifyLine->setReadOnly(true);
    QHBoxLayout*hLayout5 = new QHBoxLayout;
    hLayout5->addWidget(lastModify);
    hLayout5->addWidget(lastModifyLine);

    lastRead = new QLabel(tr("上次访问时间:"));
    lastReadLine = new QLineEdit;
    lastReadLine->setReadOnly(true);
    QHBoxLayout *hLayout6 = new QHBoxLayout;
    hLayout6->addWidget(lastRead);
    hLayout6->addWidget(lastReadLine);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addLayout( hLayout1 );
    vLayout->addLayout( hLayout2 );
    vLayout->addLayout( hLayout3 );
    vLayout->addLayout( hLayout4 );
    vLayout->addLayout( hLayout5 );
    vLayout->addLayout( hLayout6 );
    setLayout(vLayout);

    setWindowTitle(tr("File Information"));
    setFixedSize(400,250);
}

void FImageInfoDialog::getDealtImagePath(QString beforeDealtImageName)
{
	this->beforeDealtImagePath = beforeDealtImageName;
	QFileInfo info(beforeDealtImagePath);
	//获取文件大小
	//在不同的平台上，基本的C++类型如short，char，int，long，long long会有不同的字长。
	//最好把它们转换为qint8，quint8，qint16，quint16，qint32，
	//quint32，qint64，quint64，这些类型能确保字长是不随平台改变的。
	QString pictureName = info.fileName();
	qint64 size = info.size()/1024;
	//读取文件时间信息
	QDateTime created = info.created();
	QDateTime lastModified = info.lastModified();
	QDateTime lastRead = info.lastRead();
	//读取文件属性
	//LineEditSize->setText (QString::number(size));
	//显示文件时间信息
	picNameLine->setText(pictureName);
	picPathLine->setText(beforeDealtImageName);
	picSizeLine->setText(QString::number(size)+" KB");
	createTimeLine->setText (created.toString("yyyy/MM/dd hh:mm:ss"));
	lastModifyLine->setText (lastModified.toString("yyyy/MM/dd hh:mm:ss"));
	lastReadLine->setText (lastRead.toString("yyyy/MM/dd hh:mm:ss"));
    //将光标移动至最前端
    picNameLine->setCursorPosition(0); 
    picPathLine->setCursorPosition(0); 
	picSizeLine->setCursorPosition(0); 
	createTimeLine->setCursorPosition(0); 
	lastModifyLine->setCursorPosition(0); 
	lastReadLine->setCursorPosition(0); 
}
