#pragma once
#include "FSetImageSizeDialog.h"
#include "Image.h"
#include "stdafx.h"

class FSetImageSizeDialog;
class QMenu;

class FShowImageLabel : public QWidget          //����label��ʾͼƬ��
{
	Q_OBJECT

public:
    FShowImageLabel(void);

public slots:
    void maxerImageSlot();						//�Ŵ�ͼƬ
    void minnerImageSlot();						//��СͼƬ
    void userDefineSizeSlot();					//�Զ���ͼƬ��С
    void originalImageSlot();					//ԭʼͼƬ
    void fitWindowSlot();						//�ʺϴ���
    void surePushButtonClickSlot();				//ȷ�ϰ�ť
    bool showImage(QString path);

signals:
    void labelCloseSignal(QString currentLabelWindowTitle);		//��ʾͼƬ�ؼ��ر�ʱ�����ź�

protected:
	void closeEvent(QCloseEvent *event);				//��ʾͼƬ�ռ�ر�ʱ
	void contextMenuEvent( QContextMenuEvent* event);	//����Ҽ��˵�

private:
    void initShowImageLabel();					//��ʼ����ʾͼƬ�Ŀؼ�
    void initAction();							//��ʼ������
    void initMenu();							//��ʼ���˵�
    void setSize();								//����ͼƬ��С

    Image image;
    QString imagePath;
	FSetImageSizeDialog *setImageSizeDialog;
	QLabel *showImageLabel;                  //��ʾͼƬ
	QSize currentSize;				         //ͼƬ��ǰ��С 
	QScrollArea* scrollArea;		         //�������� 
	int userDefineImageWidth;
	int userDefineImageHeight;
	
	QMenu *menu;
	QAction* maxerImageAction;			     //�Ŵ��� 
	QAction* minnerImageAction;			     //��С���� 
	QAction* userDefineSizeAction;           //�Զ����С
	QAction* fitWindowAction;		         //�������ʺϴ��ڶ��� 
	QAction* originalImageAction;		     //������������С���� 

    //	void showNotDealImage(QString currentItemFileName,
    //		QHash<QString,QString> *hash);			//��ʾδ����ͼƬ
    //    void showDealtImage(Image currentDealtImage);			//��ʾ�����ͼƬ
};
