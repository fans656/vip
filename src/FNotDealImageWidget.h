#pragma once
#include <QtGui>
#include "Image.h"
#include "stdafx.h"
#include "Thread.h"

class FImageRankDialog; 

class FNotDealImageWidget : public QWidget							//δ����ͼƬ��
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
    //        QHash<QString,QString> *hash);							//�������ͼƬ��ʾ�ź�  f6: -> ImageView
    //    void notDealWidgetItemDoubleClickedSignal(QString imagePath,
    //        QHash<QString,QString> *hash);							//δ����ͼƬ˫��ʱ�����ź�   f6: -> ImageView
    //    void sendDealtImageToDealtImageWidgetSignal(Image image);	//��������ͼƬ����������󴰿�
    //    void sendHashToDealtImageWidget(QHash<QString,QString> *hash);	//��Hash���ݷ���������󴰿�   f6: -> DealtImage
    
private slots:
    void setIcon(QListWidgetItem *item, const QImage &image);    
    void dealImage();										//����ͼƬ��Ӧ
    void notDealImageItemClickSlot(QListWidgetItem *item);		//����δ����ͼƬ�б���Ӧ
    void notDealMoreImageItemClickSlot(QListWidgetItem *item);
    void selectAll();							//ȫѡ
    void deleteImage();								//ɾ��
    void showImage();							//ͼƬ��ʾ
    void onItemDoubleClicked(QListWidgetItem *item);	//δ����ͼƬ˫��ʱ
    void notDealMoreWidgetItemDoubleClickedSlot(QListWidgetItem *item);	//δ����ͼƬ˫��ʱ
    void toolBoxCurrentChangedSlot(int index);
    void judgeCR();
    
private:
    QList<QList<QString> > pathGroups;   //��Ŵ򹴵���Ŀ��Ӧ��·��
    QPushButton *deleteImageButton;
    QPushButton *dealImageButton;
    QToolBox *notDealImageToolBox;
    QListWidget *notDealImageList;						 //ͼƬ�б�
    QListWidget *notDealMoreImageList;
    QPushButton *selectAllButton;						 //ȫѡ
    QPushButton *showImageButton;						 //��ʾͼƬ��ť
    QPushButton *combineOrRankButton;
    QHash<QString,QString> *hash;                            //ӳ��ͼƬ���ơ�ͼƬ·��
    QSet<QString> openedImages;
    QList<QListWidgetItem *> picGroup;                      //�����ͼƬ�б�
    int GroupNum;
    int picSum;
    
    bool alreadyOpened(QString path); 
    bool judgePicSize(QList<QListWidgetItem *> picGroup);       //�ж�ͼƬ��С
    QPixmap setIconPic(QList<QString> paths);         //����XP����ļ���Ԥ��ͼ��
    bool isContinueOpenWhenExist(const QString &imageName);         //���������ļ����ִ���ʱ���Ƿ������
    void notDealImageWidgetLayout();							    //δ�����ڲ���
    void initNotDealImageWidget();									//��ʼ��δ������
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
