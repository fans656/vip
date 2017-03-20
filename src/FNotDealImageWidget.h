#pragma once
#include <QtGui>
#include "Image.h"
#include "stdafx.h"
#include "Thread.h"

class FImageRankDialog; 

class FNotDealImageWidget : public QWidget							//未处理图片类
{
    Q_OBJECT
public:
    FNotDealImageWidget();
    
public slots:
    void openImage(QList<QString> pathList);
    void openImage(); 
    
signals:
    void imageOpened(QList<QString> pathList);
    void hasOutput(QString message);
    void hasImageToShow(QString path);
    
    void dealImageButtonClicked(DataType datatype, const QList<QList<QString> > &pathListList);
    
    //    void showImageButtonClickedSignal(QStringList notDealImageWidgetCheckedItemList,
    //        QHash<QString,QString> *hash);							//发射多张图片显示信号  f6: -> ImageView
    //    void notDealWidgetItemDoubleClickedSignal(QString imagePath,
    //        QHash<QString,QString> *hash);							//未处理图片双击时发射信号   f6: -> ImageView
    //    void sendDealtImageToDealtImageWidgetSignal(Image image);	//将处理后的图片发射至处理后窗口
    //    void sendHashToDealtImageWidget(QHash<QString,QString> *hash);	//将Hash数据发射至处理后窗口   f6: -> DealtImage
    
private slots:
    void setIcon(QListWidgetItem *item, const QImage &image);    
    void dealImage();										//处理图片响应
    void notDealImageItemClickSlot(QListWidgetItem *item);		//单击未处理图片列表响应
    void notDealMoreImageItemClickSlot(QListWidgetItem *item);
    void selectAll();							//全选
    void deleteImage();								//删除
    void showImage();							//图片显示
    void onItemDoubleClicked(QListWidgetItem *item);	//未处理图片双击时
    void notDealMoreWidgetItemDoubleClickedSlot(QListWidgetItem *item);	//未处理图片双击时
    void toolBoxCurrentChangedSlot(int index);
    void judgeCR();
    
private:
    QList<QList<QString> > pathGroups;   //存放打勾的条目对应的路径
    QPushButton *deleteImageButton;
    QPushButton *dealImageButton;
    QToolBox *notDealImageToolBox;
    QListWidget *notDealImageList;						 //图片列表
    QListWidget *notDealMoreImageList;
    QPushButton *selectAllButton;						 //全选
    QPushButton *showImageButton;						 //显示图片按钮
    QPushButton *combineOrRankButton;
    QHash<QString,QString> *hash;                            //映射图片名称—图片路径
    QSet<QString> openedImages;
    QList<QListWidgetItem *> picGroup;                      //添加组图片列表
    int GroupNum;
    int picSum;
    
    bool alreadyOpened(QString path); 
    bool judgePicSize(QList<QListWidgetItem *> picGroup);       //判断图片大小
    QPixmap setIconPic(QList<QString> paths);         //生成XP风格文件夹预览图标
    bool isContinueOpenWhenExist(const QString &imageName);         //当打开数据文件出现错误时，是否继续打开
    void notDealImageWidgetLayout();							    //未处理窗口布局
    void initNotDealImageWidget();									//初始化未处理窗口
    void picCombine();
    void picRank();
    
    class Worker;
};

class FNotDealImageWidget::Worker : public ThreadWorker
{
    Q_OBJECT
    
public:
    Worker(QListWidgetItem *_item, QString _path)
        : item(_item), path(_path)
    {
    }
    
    void run()
    {
        QImage image;
        if (!image.load(path))
        {
            image.load("error.png");
        }
        
        emit iconCreated(item, image.scaled(QSize(W_ICONSIZE, H_ICONSIZE)));
    }
    
signals:
    void iconCreated(QListWidgetItem *item, QImage image);
    
private:
    QListWidgetItem *item;
    QString path;
};
