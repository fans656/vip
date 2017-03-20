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

class FMainWindow : public QMainWindow     //��������
{
	Q_OBJECT

public:
    FMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);

private slots:
	void aboutUsSlot();					           //��������
	void aboutVIPSlot();				           //����VIP
	void setViewModeActionTrueSlot();	           //����ͼ��ť��Ϊ�ɰ�
	void setViewModeActionFalseSlot();	           //����ͼ��ť��Ϊ���ɰ�
    
protected:
	void closeEvent(QCloseEvent *event);

private:
    //��ʼ������
    void initWindow(); 
    //��ʼ������̨
    void initConsole(); 
    //���ù���
    void initConnect();   
    //��������
    void createAction();    
    //�����˵�
	void createMenu();   
    //����������
	void createToolBar();  
    //�������봰��
	void createCentralWidget();     
    
    Console *console; 
    
    FArithmeticDialog *arithmeticDialog; 
	FNotDealImageWidget *notDealImageWidget;         //δ����ͼƬ����
	FNotDealGraghWidget *notDealGraghWidget;
	FNotDealVedioWidget *notDealVedioWidget;
	FImageViewWidget *imageViewWidget;               //ͼƬ��ʾ����
	FDealtImageWidget *dealtImageWidget;             //�����ͼƬ����
    FInputOutputWidget *inputOutputWidget; 
    FSetWidget *setWidget; 

//���˵�
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *toolsMenu;
	QMenu *helpMenu;
//�ļ��Ӳ˵�
	QAction *openImageFile;
	QAction *saveImage;
	QAction *saveImageAs;
	QAction *exitVIP;
//�༭�Ӳ˵�
	QAction *maxerImage;
	QAction *minnerImage;
	QAction *originalImage;
	QAction *fitWindow;
//��ͼ�Ӳ˵�
	QAction *biaoQianYeView;
	QAction *duiDieView;
	QAction *bingPaiView;
//�����Ӳ˵�	
	QAction *option; 
//�����Ӳ˵�
	QAction *aboutUs;
	QAction *AboutQt;
	QAction *aboutVIP;
//������
	QToolBar *fileToolBar;
    
//���봰��
	QWidget *centralWidget;
	QTabWidget* topLeftTabWidget;					/**< ���ϲ���ǩҳ�ؼ� */
	QTabWidget* topRightTabWidget;					/**< ���ϲ���ǩҳ�ؼ� */
    QTabWidget* bottomTabWidget;                    /**< �²���ǩҳ�ؼ� */
};

#endif // MAINWINDOW_H
