#include "FDealtImageWidget.h"
#include "FImageInfoDialog.h"
#include "stdafx.h"
#include "thread.h"

FDealtImageWidget::FDealtImageWidget(void)
{
    initData(); 
	initDealtImageWidget();
}

void FDealtImageWidget::initData()
{
	isImageSending = false;
    imageInfo = new FImageInfoDialog(this);
}

void FDealtImageWidget::initDealtImageWidget()
{
	selectAllButton = new QPushButton(tr("ȫѡ"));
    connect(selectAllButton,SIGNAL(clicked()),this,SLOT(selectAll()));

	saveButton = new QPushButton(tr("����"));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(saveImage()));

	saveAsButton = new QPushButton(tr("���Ϊ"));
    connect(saveAsButton,SIGNAL(clicked()),this,SLOT(saveImageAs()));

	deleteButton = new QPushButton(tr("ɾ��"));
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(deleteImage()));

	imageInformationButton = new QPushButton(tr("ͼƬ��Ϣ"));
	//imageInformationButton->setEnabled(false);
    connect(imageInformationButton,SIGNAL(clicked()),this,SLOT(showImageInfo()));

	continueToDealButton = new QPushButton(tr("��������"));
	//continueToDealButton->setEnabled(false);
	connect(continueToDealButton,SIGNAL(clicked()),this,SLOT(continueToDealSlot()));

    imageListWidget = new QListWidget;
    connect(imageListWidget,SIGNAL(itemClicked(QListWidgetItem*)),
        this,SLOT(dealtImageItemClickSlot(QListWidgetItem *)));
    connect(imageListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),
        this,SLOT(dealtImageItemDoubleClickSlot(QListWidgetItem *)));

	QHBoxLayout *hlayout1 = new QHBoxLayout;
	hlayout1->addWidget(selectAllButton);
	hlayout1->addWidget(deleteButton);
	hlayout1->addWidget(saveButton);
	hlayout1->addWidget(saveAsButton);
	hlayout1->addWidget(imageInformationButton);
	hlayout1->addWidget(continueToDealButton);
	hlayout1->addStretch();

	QVBoxLayout *vlayout = new QVBoxLayout;
	vlayout->addLayout(hlayout1);
    vlayout->addWidget(imageListWidget);

	this->setLayout(vlayout);
    
    imageListWidget->setIconSize(QSize(W_ICONSIZE+20, H_ICONSIZE+20));
    imageListWidget->setResizeMode(QListView::Adjust);
    imageListWidget->setViewMode(QListView::IconMode);  //����QListWidget����ʾģʽ
    imageListWidget->setMovement(QListView::Static);    //����QListWidget�еĵ�Ԫ��ɱ��϶�
    imageListWidget->setSpacing(10);
    imageListWidget->setSelectionMode( QAbstractItemView::ExtendedSelection); //��ѡ
}

void FDealtImageWidget::addImage(Image image)
{
    // ����������item����
    rename(image);
    // ������ʱ�ļ���
    QString fileName = image.imageName;
    QString tmpSavePath = QDir::currentPath() + "/dealtData/" + image.imageName;
    // ��������ͼ + ������ʱ�ļ�
    Worker *worker = new Worker(fileName, image.image, tmpSavePath);
    connect(worker, SIGNAL(iconMade(QString,QString,QImage)), this, SLOT(addItem(QString,QString,QImage)));
    Thread::run(worker, Thread::Uninterruptible);
}

void FDealtImageWidget::addItem(QString name, QString path, QImage image)
{
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(QSize(W_ITEMSIZE,H_ITEMSIZE - 25));
    item->setText(name);
    item->setData(Qt::UserRole, path);
    imageListWidget->addItem(item);
    item->setIcon(QIcon(QPixmap::fromImage(image)));
	//if (!isImageSending)                   
	//{
	//	emit taskRunning(false);     //�ݴ���ɺ�����ͼƬ������ʱ�����ź���-1��
	//}
}

FDealtImageWidget::Worker::Worker(QString _name, QImage _image, QString _path)
    : image(_image), name(_name), path(_path)
{
}

void FDealtImageWidget::Worker::run()
{
	QFileInfo *info = new QFileInfo(path);
	QString tempSavePath = info->absoluteDir().absolutePath();
	QDir *temp = new QDir(tempSavePath); 
	bool exist = false;
	exist = temp->exists();
	if (!exist)
	{
		temp->mkpath(tempSavePath); 
	}
    image.save(path);
    emit iconMade(name, path, image.scaled(QSize(180,150)));
}
void FDealtImageWidget::rename(Image &image)
{
    int j = 0;                       //��¼�����ͼ������
    QString newImageName = "dealt=";
    QString temp = "dealt=";
    temp.append(image.imageName);
    for (int i = 0; i< imageListWidget->count();i++)
    {
        QString otherName = imageListWidget->item(i)->data(0).toString();
        if (QString::compare(temp,otherName) == 0)
        {
            j++;
            newImageName = "dealt.";
            newImageName.append(QString::number(j,10));
            newImageName.append("=");
            temp = newImageName;
            temp.append(image.imageName);
            i = 0;
        }
    }
    newImageName.append(image.imageName);
    image.imageName = newImageName;
}

//void FDealtImageWidget::tempSaveDealtImage(Image &image)
//{
//    QString fileName = image.imageName;
//    QString tmpSavePath = QDir::currentPath() + "/dealtData/" + image.imageName;
//    image.image.save(tmpSavePath);
//}

void FDealtImageWidget::selectAll()
{
    imageListWidget->selectAll();
}

void FDealtImageWidget::deleteImage()
{
	bool isAnyNeedDelete = false;
	int lastDelete = 1;
    for (int i = imageListWidget->count() - 1;i >= 0 ;i--)
	{
        if (imageListWidget->item(i)->isSelected())
		{
			//dealtImageListListWidget->takeItem(i);
            delete imageListWidget->item(i);
			lastDelete = i;
			isAnyNeedDelete = true;
		}
	}
	if (!isAnyNeedDelete)
		return;
	if (lastDelete > 0)
	{
        imageListWidget->item(lastDelete - 1)->setSelected(true);
	}
    else if (imageListWidget->count() > 0)
	{
        imageListWidget->item(0)->setSelected(true);
	}
    //if (imageListWidget->count() == 0)
	//{
	//	imageInformationButton->setEnabled(false);
	//	continueToDealButton->setEnabled(false);
	//}

}
void FDealtImageWidget::saveImage()
{
	// item.data(Qt::UserRole)�б��������ʱ�ļ��ĵ�������ַ
    // ���豣��ɽ���ʱ�ļ�������+�ƶ�
	bool isAnyNeedSave = false;
	QList<QString> saveImagePathList;
	for (int i = 0;i < imageListWidget->count();i++)
	{
		if(imageListWidget->item(i)->isSelected())
		{
			saveImagePathList.append(imageListWidget->item(i)->data(Qt::UserRole).toString());
			isAnyNeedSave = true;
		}
	}
	if (!isAnyNeedSave)
		return;

	QString defaultSaveDirPath = QDir::currentPath() + "/User Data";
	QSettings settings("config.ini", QSettings::IniFormat);
	if (!settings.contains("defaultSaveDirPath"))
	{
		settings.setValue("defaultSaveDirPath",defaultSaveDirPath);
	}
	defaultSaveDirPath = settings.value("defaultSavePath").toString();
	
	QDir *temp = new QDir(defaultSaveDirPath);
	bool exist = temp->exists();
	if(!exist || defaultSaveDirPath == "")
	{
		bool abso = temp->isAbsolute();
		if (!abso)
		{	
			defaultSaveDirPath = QDir::currentPath() + "/User Data";
			settings.setValue("defaultSaveDirPath",defaultSaveDirPath);
		}
		else
		{
			temp->mkpath(defaultSaveDirPath);    //�����ļ���
		}
	}
	emit hasOutput(tr("ͼƬ��������Ŀ¼�£�") + defaultSaveDirPath);
	//emit taskRunning(true);		             //��ʼ����ͼƬʱ�����ź���+1
	foreach(QString path,saveImagePathList)
	{
		SaveWorker *worker = new SaveWorker(path,defaultSaveDirPath);
		//connect(worker,SIGNAL(taskFinished()),this,SLOT(saveFinished()));
		Thread::run(worker);
	}
	
}
void FDealtImageWidget::saveImageAs()
{
    // ʹ��QImage�� load() + save() �ڴ������ͼƬʱ�Ὣ�����濨��
    // item.data(Qt::UserRole)�б��������ʱ�ļ���������ַ
    // �������Ϊ�ɽ���ʱ�ļ�������+�ƶ�
	bool isAnyImageNeedSave = false;
	QList<QString> saveAsImagePathList;
	for (int i = 0;i < imageListWidget->count();i++)
	{
		if(imageListWidget->item(i)->isSelected())
		{
			saveAsImagePathList.append(imageListWidget->item(i)->data(Qt::UserRole).toString());
			isAnyImageNeedSave = true;
		}
	}
	if(!isAnyImageNeedSave)
		return;

	QSettings settings("config.ini", QSettings::IniFormat);
	if(!QFileInfo(settings.value("saveAsLastOpenDirPath").toString()).exists())
	{
		settings.setValue("saveAsLastOpenDirPath", QDir::currentPath());
	}
	QString lastSaveDirPath = settings.value("saveAsLastOpenDirPath").toString();

	QString savedFormat( "png");
	static const QString saveDialogName( tr("����ͼƬ"));
	
	const QString dirPath = QFileDialog::getExistingDirectory( NULL, saveDialogName, lastSaveDirPath);
	if( !QFileInfo( dirPath).exists())
	{
		return;
	}
	settings.setValue("saveAsLastOpenDirPath", dirPath);

	//emit taskRunning(true);   //��ʼ���ͼƬʱ�������ź���+1
	foreach(QString path,saveAsImagePathList)
	{
		SaveWorker *worker = new SaveWorker(path,dirPath);
		//connect(worker,SIGNAL(taskFinished()),this,SLOT(saveFinished()));
		Thread::run(worker);
	}
}
FDealtImageWidget::SaveWorker::SaveWorker(QString imagePath, QString saveAsFolderPath)
{
	this->imagePath = imagePath;
	this->saveAsFolderPath = saveAsFolderPath;
}
void FDealtImageWidget::SaveWorker::run()
{
	QString savedFormat( "png");
	QImage image;
	QFileInfo info(imagePath);
	QString fileName = info.fileName();
	QString saveAsPath = saveAsFolderPath + "\\" + fileName;
	image.load(imagePath);
	image.save(saveAsPath, savedFormat.toAscii());
//	emit taskFinished();
}
void FDealtImageWidget::saveFinished()
{
	//emit taskRunning(false);   //�����������ʱ�������ź���-1
}
void FDealtImageWidget::dealtImageItemClickSlot(QListWidgetItem *item)
{
	//imageInformationButton->setEnabled(true);
	//continueToDealButton->setEnabled(true);
}
void FDealtImageWidget::dealtImageItemDoubleClickSlot(QListWidgetItem *item)
{
    QString path = item->data(Qt::UserRole).toString();
    emit hasImageToShow(path);
}
void FDealtImageWidget::showImageInfo()
{
	QListWidgetItem *item = imageListWidget->currentItem();
	if (item == NULL)
	{
		return; 
	}
	QString path = item->data(Qt::UserRole).toString();
	// ÿ�ζ�new������ڴ�й¶������ֻ��initData()��newһ��
	//imageInfo = new FImageInfoDialog;
	imageInfo->getDealtImagePath(path);
	imageInfo->show();
}
void FDealtImageWidget::continueToDealSlot()
{
	bool isAnyNeedDeal = false;
	QList<QString> continueToDealImageList;
	for (int i = 0;i < imageListWidget->count();i++)
	{
		if(imageListWidget->item(i)->isSelected())
		{
			continueToDealImageList.push_back(imageListWidget->item(i)->data(Qt::UserRole).toString()); 
			isAnyNeedDeal = true;
		}
	}
	if (!isAnyNeedDeal)
		return;
	emit continueToDeal(continueToDealImageList);
}
void FDealtImageWidget::isImageSendingSLot(bool isSending)
{
	//isImageSending = isSending;
	//if (isSending)            
	//{
	//	emit taskRunning(true);    //����ͼƬ��������ʱ�򣬷����ź���+1
	//}
}
