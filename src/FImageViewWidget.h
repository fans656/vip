#pragma once
#include <QtGui>
#include "Image.h"

class FShowImageLabel; 
class FimageClassWidget;

class FImageViewWidget : public QMainWindow
{
	Q_OBJECT
public:
    FImageViewWidget(QWidget *parent = NULL);

public slots:
    void showImage(QString path);
    void cascadeSubWindows();
    void tileSubWindows();
    
signals:
	void setMianWindowActionTrueSignal();
    void setMianWindowActionFalseSignal();
	void hasOutPut(QString message);

private slots:
	void viewModeSlot();				//视图模式
	void maxerImageSlot();				//放大图片
	void minnerImageSlot();				//缩小图片
	void userDefineSizeSlot();			//自定义大小
	void originalImageSlot();			//原始大小
	void fitWindowSlot();				//适合窗口
    void closeImageSlot();				//关闭图片
	void prevImage();					//上一张
	void nextImage();					//下一张
    void closeImageFile(QString path);  //显示图片控件关闭时

    //	void notDealImageItemDoubleClickSlot(QString imagePath,
    //		QHash<QString,QString> *hash);	//双击未处理图片列表条目时的响应
    //	void notDealImageWidgetShowImageButtonClickedSlot(QStringList notDealImageWidgetCheckedItemList,
    //		QHash<QString,QString> *hash);	//点击显示图片按钮多张图片同时显示
    //	void dealtImageItemDoubleClickSlot(Image currentDealtImage,
    //		QHash<QString,QString> *hash);	//双击处理后的图片时
    //	void showImageLabelClosedSlot(QString currentLabelWindowTitle);	//显示图片控件关闭时

private:
    QMdiArea *mdiArea;
	FimageClassWidget *imageClassWidget;
	QDockWidget *dockWidget;

	QToolBar *viewToolBar;
	QAction *maxerImageAction;
	QAction *minnerImageAction;
	QAction *userDefineSizeAction;
	QAction *originalImageAction;
	QAction *fitWindowAction;
	QAction *viewModeAction;
	QAction *duiDieViewAction;
	QAction *bingPaiViewAction;
	QAction *closeImageAction;
	QAction *prevImageAction;
	QAction *nextImageAction;

    enum QMdiArea::ViewMode currentViewMode;	         // 多文档显示模式
    QHash<QString, QMdiSubWindow *> imageSubWindowHash;

    void mainViewWidgetLayout();

//	void imageDisplay(QString currentItemFileName);
//	void imageDisplay(QStringList currentItemFileNameList);
//	void dealtImageDisplay(Image currentDealtImage);
};
