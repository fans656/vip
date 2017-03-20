#pragma once
#include <QtGui>
#include "Image.h"
#include "Thread.h"
#include "stdafx.h"

class FImageInfoDialog; 

class FDealtImageWidget : public QWidget          //�����ͼƬ��
{
	Q_OBJECT

public:
    FDealtImageWidget();

public slots:
    void addImage(Image image);	//��ȡ������ͼƬ

signals:
    void hasImageToShow(QString path);
	void continueToDeal(QList<QString> imageListPath);
	void hasOutput(QString message); //��Ϣ��ʾ
	//void taskRunning(bool isTaskRunning);

private slots:
    void selectAll();										//ȫѡ
    void deleteImage();								//ɾ��
    void saveImage();									//����
    void saveImageAs();								//���Ϊ
    void showImageInfo();								//ͼƬ��Ϣ
    void dealtImageItemClickSlot(QListWidgetItem *item);		//���������ͼƬʱ
    void dealtImageItemDoubleClickSlot(QListWidgetItem *item);	//˫��������ͼƬʱ

    void addItem(QString name, QString path, QImage image);     // �����̴߳��ص����ź��image
	void continueToDealSlot();                                  //��������
	void saveFinished();
	void isImageSendingSLot(bool isSending);

private:
//dealtImageWidget������
    QListWidget *imageListWidget;
	QPushButton *selectAllButton;
	QPushButton *saveButton;
	QPushButton *saveAsButton;
	QPushButton *deleteButton;
	QPushButton *imageInformationButton;
	QPushButton *continueToDealButton;

	QString lastOpenDirPath;					//�ϴδ�ͼƬ�ļ���·�� 
	//QString lastSaveDirPath;					//�ϴα���ͼƬ�ļ���·�� 
	FImageInfoDialog *imageInfo;
	bool isImageSending;
 
    void initData();                  //��ʼ������
    void initDealtImageWidget();       //���������ͼƬ����
    void rename(Image &image);                      // ������ͼ������������item���ֳ�ͻ
    //void tempSaveDealtImage(Image &image);		  //���洦����ͼƬ

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