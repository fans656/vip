#pragma once
#include <QtGui>
#include "Image.h"
#include "Thread.h"
#include "stdafx.h"

class FImageInfoDialog; 

class FDealtImageWidget : public QWidget          //处理后图片类
{
	Q_OBJECT

public:
    FDealtImageWidget();

public slots:
    void addImage(Image image);	//获取处理后的图片

signals:
    void hasImageToShow(QString path);
	void continueToDeal(QList<QString> imageListPath);
	void hasOutput(QString message); //消息提示
	//void taskRunning(bool isTaskRunning);

private slots:
    void selectAll();										//全选
    void deleteImage();								//删除
    void saveImage();									//保存
    void saveImageAs();								//另存为
    void showImageInfo();								//图片信息
    void dealtImageItemClickSlot(QListWidgetItem *item);		//点击处理后的图片时
    void dealtImageItemDoubleClickSlot(QListWidgetItem *item);	//双击处理后的图片时

    void addItem(QString name, QString path, QImage image);     // 接受线程传回的缩放后的image
	void continueToDealSlot();                                  //继续处理
	void saveFinished();
	void isImageSendingSLot(bool isSending);

private:
//dealtImageWidget的属性
    QListWidget *imageListWidget;
	QPushButton *selectAllButton;
	QPushButton *saveButton;
	QPushButton *saveAsButton;
	QPushButton *deleteButton;
	QPushButton *imageInformationButton;
	QPushButton *continueToDealButton;

	QString lastOpenDirPath;					//上次打开图片文件夹路径 
	//QString lastSaveDirPath;					//上次保存图片文件夹路径 
	FImageInfoDialog *imageInfo;
	bool isImageSending;
 
    void initData();                  //初始化数据
    void initDealtImageWidget();       //创建处理后图片窗口
    void rename(Image &image);                      // 重命名图像以免与已有item名字冲突
    //void tempSaveDealtImage(Image &image);		  //缓存处理后的图片

    class Worker;
	class SaveWorker;
};

class FDealtImageWidget::Worker : public ThreadWorker
{
    Q_OBJECT

public:
    Worker(QString _name, QImage _image, QString _path);
    void run();

signals:
    void iconMade(QString name, QString path, QImage image);

private:
    QImage image;
    QString name;
    QString path;
};

class FDealtImageWidget::SaveWorker : public ThreadWorker
{
	Q_OBJECT

public:
	SaveWorker(QString imagePath, QString saveAsPath);
	void run();

signals:
	//void taskFinished();

private:
	QString imagePath;
	QString saveAsFolderPath;
};