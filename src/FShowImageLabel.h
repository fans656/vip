#pragma once
#include "FSetImageSizeDialog.h"
#include "Image.h"
#include "stdafx.h"

class FSetImageSizeDialog;
class QMenu;

class FShowImageLabel : public QWidget          //定义label显示图片类
{
	Q_OBJECT

public:
    FShowImageLabel(void);

public slots:
    void maxerImageSlot();						//放大图片
    void minnerImageSlot();						//缩小图片
    void userDefineSizeSlot();					//自定义图片大小
    void originalImageSlot();					//原始图片
    void fitWindowSlot();						//适合窗口
    void surePushButtonClickSlot();				//确认按钮
    bool showImage(QString path);

signals:
    void labelCloseSignal(QString currentLabelWindowTitle);		//显示图片控件关闭时发射信号

protected:
	void closeEvent(QCloseEvent *event);				//显示图片空间关闭时
	void contextMenuEvent( QContextMenuEvent* event);	//鼠标右键菜单

private:
    void initShowImageLabel();					//初始化显示图片的控件
    void initAction();							//初始化动作
    void initMenu();							//初始化菜单
    void setSize();								//设置图片大小

    Image image;
    QString imagePath;
	FSetImageSizeDialog *setImageSizeDialog;
	QLabel *showImageLabel;                  //显示图片
	QSize currentSize;				         //图片当前大小 
	QScrollArea* scrollArea;		         //滚动区域 
	int userDefineImageWidth;
	int userDefineImageHeight;
	
	QMenu *menu;
	QAction* maxerImageAction;			     //放大动作 
	QAction* minnerImageAction;			     //缩小动作 
	QAction* userDefineSizeAction;           //自定义大小
	QAction* fitWindowAction;		         //调整到适合窗口动作 
	QAction* originalImageAction;		     //调整到正常大小动作 

    //	void showNotDealImage(QString currentItemFileName,
    //		QHash<QString,QString> *hash);			//显示未处理图片
    //    void showDealtImage(Image currentDealtImage);			//显示处理后图片
};
