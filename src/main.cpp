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
    
    /***************构造***************/
    setCode();
    qRegisterMetaType<Image>("Image"); //线程要用到

    ArithmeticManager arithmeticManager;  // 创建、读取数据库
    ThreadPool threadPool;
    Thread thread;
    /***************构造***************/
    
    /***************运行***************/
    bool ok;
    FMainWindow mainwindow;
    mainwindow.showMaximized();
    ok = a.exec(); 
    /***************运行***************/
 
    /***************析构***************/
    /***************析构***************/
    
	return ok;
}
