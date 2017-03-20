#include "FMainWindow.h"
#include "Image.h"
#include "ArithmeticManager.h"
#include "Thread.h"
#include <QtGui/QApplication>

void setCode()
{
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
	//QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    /***************����***************/
    setCode();
    qRegisterMetaType<Image>("Image"); //�߳�Ҫ�õ�

    ArithmeticManager arithmeticManager;  // ��������ȡ���ݿ�
    ThreadPool threadPool;
    Thread thread;
    /***************����***************/
    
    /***************����***************/
    bool ok;
    FMainWindow mainwindow;
    mainwindow.showMaximized();
    ok = a.exec(); 
    /***************����***************/
 
    /***************����***************/
    /***************����***************/
    
	return ok;
}
