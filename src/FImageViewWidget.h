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
	void viewModeSlot();				//��ͼģʽ
	void maxerImageSlot();				//�Ŵ�ͼƬ
	void minnerImageSlot();				//��СͼƬ
	void userDefineSizeSlot();			//�Զ����С
	void originalImageSlot();			//ԭʼ��С
	void fitWindowSlot();				//�ʺϴ���
    void closeImageSlot();				//�ر�ͼƬ
	void prevImage();					//��һ��
	void nextImage();					//��һ��
    void closeImageFile(QString path);  //��ʾͼƬ�ؼ��ر�ʱ

    //	void notDealImageItemDoubleClickSlot(QString imagePath,
    //		QHash<QString,QString> *hash);	//˫��δ����ͼƬ�б���Ŀʱ����Ӧ
    //	void notDealImageWidgetShowImageButtonClickedSlot(QStringList notDealImageWidgetCheckedItemList,
    //		QHash<QString,QString> *hash);	//�����ʾͼƬ��ť����ͼƬͬʱ��ʾ
    //	void dealtImageItemDoubleClickSlot(Image currentDealtImage,
    //		QHash<QString,QString> *hash);	//˫��������ͼƬʱ
    //	void showImageLabelClosedSlot(QString currentLabelWindowTitle);	//��ʾͼƬ�ؼ��ر�ʱ

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

    enum QMdiArea::ViewMode currentViewMode;	         // ���ĵ���ʾģʽ
    QHash<QString, QMdiSubWindow *> imageSubWindowHash;

    void mainViewWidgetLayout();

//	void imageDisplay(QString currentItemFileName);
//	void imageDisplay(QStringList currentItemFileNameList);
//	void dealtImageDisplay(Image currentDealtImage);
};
