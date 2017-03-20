#include "FMainWindow.h"
#include "FArithmeticDialog.h"
#include "FNotDealImageWidget.h"
#include "FImageViewWidget.h"
#include "FDealtImageWidget.h"
#include "FNotDealGraghWidget.h"
#include "FNotDealVedioWidget.h"
#include "FInputOutputWidget.h"
#include "FSetWidget.h"
#include "console.h"
#include "stdafx.h"

FMainWindow::FMainWindow(QWidget *parent, Qt::WFlags flags)
    :QMainWindow(parent,flags)
{
    initWindow(); 
    initConsole(); 
    initConnect();  
}

void FMainWindow::initWindow()
{
    createAction();
    createMenu(); 
    createToolBar(); 
    createCentralWidget();
    setWindowTitle(tr("VIP—Versatile Image Platform"));
}

void FMainWindow::initConsole()
{
    console = new Console(arithmeticDialog, 
                          notDealImageWidget, 
                          inputOutputWidget, 
                          this); 
}

void FMainWindow::initConnect()
{
    //文件栏
    connect(openImageFile,SIGNAL(triggered()),notDealImageWidget,SLOT(openImage()));
    connect(saveImage,SIGNAL(triggered()),dealtImageWidget,SLOT(saveImage()));
    connect(saveImageAs,SIGNAL(triggered()),dealtImageWidget,SLOT(saveImageAs()));
    connect(exitVIP,SIGNAL(triggered()),this,SLOT(close()));
    //编辑栏
    connect(maxerImage,SIGNAL(triggered()),imageViewWidget,SLOT(maxerImageSlot()));
    connect(minnerImage,SIGNAL(triggered()),imageViewWidget,SLOT(minnerImageSlot()));
    connect(originalImage,SIGNAL(triggered()),imageViewWidget,SLOT(originalImageSlot()));
    connect(fitWindow,SIGNAL(triggered()),imageViewWidget,SLOT(fitWindowSlot()));
    //视图栏
    connect(biaoQianYeView,SIGNAL(triggered()),imageViewWidget,SLOT(viewModeSlot()));
    connect(duiDieView, SIGNAL(triggered()),imageViewWidget, SLOT(cascadeSubWindows()));
    connect(bingPaiView, SIGNAL(triggered()),imageViewWidget, SLOT(tileSubWindows()));
    connect(imageViewWidget,SIGNAL(setMianWindowActionTrueSignal()),this,SLOT(setViewModeActionTrueSlot()));
    connect(imageViewWidget,SIGNAL(setMianWindowActionFalseSignal()),this,SLOT(setViewModeActionFalseSlot()));
    //工具栏
    connect(option, SIGNAL(triggered()), setWidget, SLOT(setRequiredSlot())); 
    //关于栏
    connect(AboutQt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    connect(aboutUs,SIGNAL(triggered()),this,SLOT(aboutUsSlot()));
    connect(aboutVIP,SIGNAL(triggered()),this,SLOT(aboutVIPSlot()));
    
    //控件
    connect(arithmeticDialog, SIGNAL(hasDealtImage(Image)),
            dealtImageWidget, SLOT(addImage(Image)));
    connect(notDealImageWidget, SIGNAL(dealImageButtonClicked(DataType,QList<QList<QString> >)),  
            arithmeticDialog, SLOT(dealData(DataType,QList<QList<QString> >)));
    connect(notDealImageWidget, SIGNAL(hasImageToShow(QString)),
            imageViewWidget, SLOT(showImage(QString)));
    connect(dealtImageWidget, SIGNAL(hasImageToShow(QString)),
            imageViewWidget, SLOT(showImage(QString)));
    connect(dealtImageWidget, SIGNAL(continueToDeal(QList<QString>)), 
            notDealImageWidget, SLOT(openImage(QList<QString>))); 
    //控件有要输出的字符串
    connect(notDealImageWidget, SIGNAL(hasOutput(QString)),
            inputOutputWidget, SLOT(addOutput(QString)));
    connect(arithmeticDialog, SIGNAL(hasOutput(QString)),
            inputOutputWidget, SLOT(addOutput(QString)));
    connect(imageViewWidget, SIGNAL(hasOutPut(QString)), 
            inputOutputWidget, SLOT(addOutput(QString))); 
    connect(dealtImageWidget, SIGNAL(hasOutput(QString)), 
            inputOutputWidget, SLOT(addOutput(QString))); 
    //控件有要输出的命令
    connect(notDealImageWidget, SIGNAL(imageOpened(QList<QString>)), 
            console, SLOT(displayOpenImageCmd(QList<QString>))); 
    connect(arithmeticDialog, SIGNAL(imageDealt(QList<QList<QString> >,QList<QString>)), 
            console, SLOT(displayDealImageCmd(QList<QList<QString> >,QList<QString>))); 
}

void FMainWindow::createAction()
{
    fileMenu = new QMenu(tr("文件"));
    openImageFile = new QAction(tr("打开图片"),this);
    openImageFile->setIcon(QIcon(":/icon/open.png"));
    
    saveImage = new QAction(tr("保存"),this);
    saveImage->setIcon(QIcon(":/icon/save.png"));
    saveImageAs = new QAction(tr("另存为"),this);
    exitVIP = new QAction(tr("退出"),this);
    
    editMenu = new QMenu(tr("编辑"));
    maxerImage = new QAction(tr("放大"),this);
    minnerImage = new QAction(tr("缩小"),this);
    originalImage = new QAction(tr("原始大小"),this);
    fitWindow = new QAction(tr("适合窗口"),this);
    viewMenu = new QMenu(tr("视图"));
    biaoQianYeView = new QAction(tr("标签页/多文档"),this);
    duiDieView = new QAction(tr("堆叠显示"),this);
    duiDieView->setEnabled(false);
    bingPaiView = new QAction(tr("并排显示"),this);
    bingPaiView->setEnabled(false);
    
    toolsMenu = new QMenu(tr("工具"));
    option = new QAction(tr("选项"), this); 
    
    helpMenu = new QMenu(tr("帮助"));
    aboutUs = new QAction(tr("关于我们"),this);
    AboutQt = new QAction(tr("关于Qt"),this);
    aboutVIP = new QAction(tr("关于VIP"),this);
}
void FMainWindow::createMenu()
{
    fileMenu = menuBar()->addMenu(tr("文件"));
    fileMenu->addAction(openImageFile);
    fileMenu->addAction(saveImage);
    fileMenu->addAction(saveImageAs);
    fileMenu->addAction(exitVIP);
    
    editMenu = menuBar()->addMenu(tr("编辑"));
    editMenu->addAction(maxerImage);
    editMenu->addAction(minnerImage);
    editMenu->addAction(originalImage);
    editMenu->addAction(fitWindow);
    
    editMenu = menuBar()->addMenu(tr("视图"));
    editMenu->addAction(biaoQianYeView);
    editMenu->addAction(duiDieView);
    editMenu->addAction(bingPaiView);
    
    toolsMenu = menuBar()->addMenu(tr("工具"));
    toolsMenu->addAction(option); 
    
    helpMenu = menuBar()->addMenu(tr("帮助"));
    helpMenu->addAction(aboutUs);
    helpMenu->addAction(AboutQt);
    helpMenu->addAction(aboutVIP);
}
void FMainWindow::createToolBar()
{
    fileToolBar = new QToolBar(this); 
    addToolBar(fileToolBar); 
    fileToolBar->addAction(openImageFile);
    fileToolBar->addAction(saveImage);
}

void FMainWindow::createCentralWidget()
{
    inputOutputWidget = new FInputOutputWidget; 
    notDealImageWidget = new FNotDealImageWidget;	
    imageViewWidget = new FImageViewWidget;	
    dealtImageWidget = new FDealtImageWidget;
    notDealVedioWidget = new FNotDealVedioWidget;
    notDealGraghWidget = new FNotDealGraghWidget;
    arithmeticDialog = new FArithmeticDialog;
    setWidget = new FSetWidget; 
    
    topLeftTabWidget = new QTabWidget;
    topLeftTabWidget->addTab( notDealImageWidget,tr("未处理图片"));
    topLeftTabWidget->addTab( notDealVedioWidget,tr("未处理视频"));
    topLeftTabWidget->addTab( notDealGraghWidget,tr("未处理图形"));
    
    topRightTabWidget = new QTabWidget;
    topRightTabWidget->addTab( imageViewWidget, imageViewWidget->windowTitle());
    
    bottomTabWidget = new QTabWidget;
    bottomTabWidget->addTab(inputOutputWidget, tr("控制台")); 
    bottomTabWidget->addTab(dealtImageWidget, tr("处理后图片"));
    
    QPalette pal;
    pal.setColor( QPalette::Window, QColor( 200, 200, 200));
    
    QSplitter* topSplitter = new QSplitter( Qt::Horizontal);
    topSplitter->setHandleWidth(1);
    topSplitter->addWidget( topLeftTabWidget);
    topSplitter->setHandleWidth(10);
    topSplitter->addWidget( topRightTabWidget);
    topSplitter->handle(1)->setPalette( pal);
    
    QSplitter* mainSplitter = new QSplitter( Qt::Vertical);
    mainSplitter->setHandleWidth(1);
    mainSplitter->addWidget( topSplitter);
    mainSplitter->addWidget( bottomTabWidget);
    mainSplitter->handle(1)->setPalette( pal);
    
    setCentralWidget( mainSplitter);
}

void FMainWindow::setViewModeActionFalseSlot()
{
    bingPaiView->setEnabled(false);
    duiDieView->setEnabled(false);
}

void FMainWindow::setViewModeActionTrueSlot()
{
    bingPaiView->setEnabled(true);
    duiDieView->setEnabled(true);
}

void FMainWindow::closeEvent(QCloseEvent *event)
{
    if (Thread::isBusy())
    {
        QMessageBox::warning(this, tr("ばか"), tr("缴枪不杀！！！"));
        event->ignore();
    }
    else
    {
        QApplication::exit();
    }
}

void FMainWindow::aboutUsSlot()
{
    QMessageBox::about(this, tr("About Us"),
                       tr("<h1><font color = darkblue>指导老师：贾渊</font></h1>"
                          "<h3>成员：李春辉，成超，薛鹏飞，刘文龙，张翠香</h3>"
                          "<p>-------------图形图像处理实验室-------------"));
}
void FMainWindow::aboutVIPSlot()
{
    QMessageBox::about(this, tr("About VIP"),
                       tr("<h1><font color = green>VIP图像处理平台 1.0</font></h1>"
                          "<h3>---Versatitle Image Platform---</h3>"
                          "<h3>-----Vision Image Platform-----</h3>"
                          "<p>......................................................2012.7.26</p>"));
}
