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
    setWindowTitle(tr("VIP��Versatile Image Platform"));
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
    //�ļ���
    connect(openImageFile,SIGNAL(triggered()),notDealImageWidget,SLOT(openImage()));
    connect(saveImage,SIGNAL(triggered()),dealtImageWidget,SLOT(saveImage()));
    connect(saveImageAs,SIGNAL(triggered()),dealtImageWidget,SLOT(saveImageAs()));
    connect(exitVIP,SIGNAL(triggered()),this,SLOT(close()));
    //�༭��
    connect(maxerImage,SIGNAL(triggered()),imageViewWidget,SLOT(maxerImageSlot()));
    connect(minnerImage,SIGNAL(triggered()),imageViewWidget,SLOT(minnerImageSlot()));
    connect(originalImage,SIGNAL(triggered()),imageViewWidget,SLOT(originalImageSlot()));
    connect(fitWindow,SIGNAL(triggered()),imageViewWidget,SLOT(fitWindowSlot()));
    //��ͼ��
    connect(biaoQianYeView,SIGNAL(triggered()),imageViewWidget,SLOT(viewModeSlot()));
    connect(duiDieView, SIGNAL(triggered()),imageViewWidget, SLOT(cascadeSubWindows()));
    connect(bingPaiView, SIGNAL(triggered()),imageViewWidget, SLOT(tileSubWindows()));
    connect(imageViewWidget,SIGNAL(setMianWindowActionTrueSignal()),this,SLOT(setViewModeActionTrueSlot()));
    connect(imageViewWidget,SIGNAL(setMianWindowActionFalseSignal()),this,SLOT(setViewModeActionFalseSlot()));
    //������
    connect(option, SIGNAL(triggered()), setWidget, SLOT(setRequiredSlot())); 
    //������
    connect(AboutQt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    connect(aboutUs,SIGNAL(triggered()),this,SLOT(aboutUsSlot()));
    connect(aboutVIP,SIGNAL(triggered()),this,SLOT(aboutVIPSlot()));
    
    //�ؼ�
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
    //�ؼ���Ҫ������ַ���
    connect(notDealImageWidget, SIGNAL(hasOutput(QString)),
            inputOutputWidget, SLOT(addOutput(QString)));
    connect(arithmeticDialog, SIGNAL(hasOutput(QString)),
            inputOutputWidget, SLOT(addOutput(QString)));
    connect(imageViewWidget, SIGNAL(hasOutPut(QString)), 
            inputOutputWidget, SLOT(addOutput(QString))); 
    connect(dealtImageWidget, SIGNAL(hasOutput(QString)), 
            inputOutputWidget, SLOT(addOutput(QString))); 
    //�ؼ���Ҫ���������
    connect(notDealImageWidget, SIGNAL(imageOpened(QList<QString>)), 
            console, SLOT(displayOpenImageCmd(QList<QString>))); 
    connect(arithmeticDialog, SIGNAL(imageDealt(QList<QList<QString> >,QList<QString>)), 
            console, SLOT(displayDealImageCmd(QList<QList<QString> >,QList<QString>))); 
}

void FMainWindow::createAction()
{
    fileMenu = new QMenu(tr("�ļ�"));
    openImageFile = new QAction(tr("��ͼƬ"),this);
    openImageFile->setIcon(QIcon(":/icon/open.png"));
    
    saveImage = new QAction(tr("����"),this);
    saveImage->setIcon(QIcon(":/icon/save.png"));
    saveImageAs = new QAction(tr("���Ϊ"),this);
    exitVIP = new QAction(tr("�˳�"),this);
    
    editMenu = new QMenu(tr("�༭"));
    maxerImage = new QAction(tr("�Ŵ�"),this);
    minnerImage = new QAction(tr("��С"),this);
    originalImage = new QAction(tr("ԭʼ��С"),this);
    fitWindow = new QAction(tr("�ʺϴ���"),this);
    viewMenu = new QMenu(tr("��ͼ"));
    biaoQianYeView = new QAction(tr("��ǩҳ/���ĵ�"),this);
    duiDieView = new QAction(tr("�ѵ���ʾ"),this);
    duiDieView->setEnabled(false);
    bingPaiView = new QAction(tr("������ʾ"),this);
    bingPaiView->setEnabled(false);
    
    toolsMenu = new QMenu(tr("����"));
    option = new QAction(tr("ѡ��"), this); 
    
    helpMenu = new QMenu(tr("����"));
    aboutUs = new QAction(tr("��������"),this);
    AboutQt = new QAction(tr("����Qt"),this);
    aboutVIP = new QAction(tr("����VIP"),this);
}
void FMainWindow::createMenu()
{
    fileMenu = menuBar()->addMenu(tr("�ļ�"));
    fileMenu->addAction(openImageFile);
    fileMenu->addAction(saveImage);
    fileMenu->addAction(saveImageAs);
    fileMenu->addAction(exitVIP);
    
    editMenu = menuBar()->addMenu(tr("�༭"));
    editMenu->addAction(maxerImage);
    editMenu->addAction(minnerImage);
    editMenu->addAction(originalImage);
    editMenu->addAction(fitWindow);
    
    editMenu = menuBar()->addMenu(tr("��ͼ"));
    editMenu->addAction(biaoQianYeView);
    editMenu->addAction(duiDieView);
    editMenu->addAction(bingPaiView);
    
    toolsMenu = menuBar()->addMenu(tr("����"));
    toolsMenu->addAction(option); 
    
    helpMenu = menuBar()->addMenu(tr("����"));
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
    topLeftTabWidget->addTab( notDealImageWidget,tr("δ����ͼƬ"));
    topLeftTabWidget->addTab( notDealVedioWidget,tr("δ������Ƶ"));
    topLeftTabWidget->addTab( notDealGraghWidget,tr("δ����ͼ��"));
    
    topRightTabWidget = new QTabWidget;
    topRightTabWidget->addTab( imageViewWidget, imageViewWidget->windowTitle());
    
    bottomTabWidget = new QTabWidget;
    bottomTabWidget->addTab(inputOutputWidget, tr("����̨")); 
    bottomTabWidget->addTab(dealtImageWidget, tr("�����ͼƬ"));
    
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
        QMessageBox::warning(this, tr("�Ф�"), tr("��ǹ��ɱ������"));
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
                       tr("<h1><font color = darkblue>ָ����ʦ����Ԩ</font></h1>"
                          "<h3>��Ա����ԣ��ɳ���Ѧ���ɣ����������Ŵ���</h3>"
                          "<p>-------------ͼ��ͼ����ʵ����-------------"));
}
void FMainWindow::aboutVIPSlot()
{
    QMessageBox::about(this, tr("About VIP"),
                       tr("<h1><font color = green>VIPͼ����ƽ̨ 1.0</font></h1>"
                          "<h3>---Versatitle Image Platform---</h3>"
                          "<h3>-----Vision Image Platform-----</h3>"
                          "<p>......................................................2012.7.26</p>"));
}
