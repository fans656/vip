#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

class FArithmeticDialog; 
class FNotDealImageWidget; 
class FImageViewWidget; 
class FDealtImageWidget; 
class FNotDealGraghWidget; 
class FNotDealVedioWidget; 
class FInputOutputWidget; 
class FSetWidget; 
class Console;

class FMainWindow : public QMainWindow     //主窗口类
{
	Q_OBJECT

public:
    FMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);

private slots:
	void aboutUsSlot();					           //关于我们
	void aboutVIPSlot();				           //关于VIP
	void setViewModeActionTrueSlot();	           //将视图按钮设为可按
	void setViewModeActionFalseSlot();	           //将视图按钮设为不可按
    
protected:
	void closeEvent(QCloseEvent *event);

private:
    //初始化窗口
    void initWindow(); 
    //初始化控制台
    void initConsole(); 
    //设置关联
    void initConnect();   
    //创建动作
    void createAction();    
    //创建菜单
	void createMenu();   
    //创建工具栏
	void createToolBar();  
    //创建中央窗口
	void createCentralWidget();     
    
    Console *console; 
    
    FArithmeticDialog *arithmeticDialog; 
	FNotDealImageWidget *notDealImageWidget;         //未处理图片窗口
	FNotDealGraghWidget *notDealGraghWidget;
	FNotDealVedioWidget *notDealVedioWidget;
	FImageViewWidget *imageViewWidget;               //图片显示窗口
	FDealtImageWidget *dealtImageWidget;             //处理后图片窗口
    FInputOutputWidget *inputOutputWidget; 
    FSetWidget *setWidget; 

//主菜单
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *toolsMenu;
	QMenu *helpMenu;
//文件子菜单
	QAction *openImageFile;
	QAction *saveImage;
	QAction *saveImageAs;
	QAction *exitVIP;
//编辑子菜单
	QAction *maxerImage;
	QAction *minnerImage;
	QAction *originalImage;
	QAction *fitWindow;
//视图子菜单
	QAction *biaoQianYeView;
	QAction *duiDieView;
	QAction *bingPaiView;
//工具子菜单	
	QAction *option; 
//帮助子菜单
	QAction *aboutUs;
	QAction *AboutQt;
	QAction *aboutVIP;
//工具栏
	QToolBar *fileToolBar;
    
//中央窗口
	QWidget *centralWidget;
	QTabWidget* topLeftTabWidget;					/**< 左上部标签页控件 */
	QTabWidget* topRightTabWidget;					/**< 右上部标签页控件 */
    QTabWidget* bottomTabWidget;                    /**< 下部标签页控件 */
};

#endif // MAINWINDOW_H
