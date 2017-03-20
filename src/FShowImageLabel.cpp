#include "FShowImageLabel.h"

FShowImageLabel::FShowImageLabel(void)
{
	initAction();
	initMenu();
	initShowImageLabel();
}

void FShowImageLabel::initShowImageLabel()
{
	showImageLabel = new QLabel( NULL);
	showImageLabel->setFrameShadow( QFrame::Sunken);
	showImageLabel->setBackgroundRole( QPalette::Dark);
	showImageLabel->setAutoFillBackground( true);
	showImageLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored);
	showImageLabel->setScaledContents(true);

	scrollArea = new QScrollArea( NULL);
	scrollArea->setWidget( showImageLabel);
	QHBoxLayout* layout = new QHBoxLayout( NULL);
	layout->addWidget( scrollArea);
	setLayout( layout);

	scrollArea->setWidgetResizable( true);
	scrollArea->setWidgetResizable( false);

	setAttribute( Qt::WA_DeleteOnClose);
}

bool FShowImageLabel::showImage(QString path)
{
    // 提取文件名
    QChar sep = (path.contains('/') ? '/' : '\\');  // 分隔符不同，NotDealImage -> ("E:/tmp/1.jpg")，DealtImage -> ("E:\tmp\1.jpg")
    QString name = path.section(sep, -1);
    setWindowTitle(name);
    // 加载图片
    QImage img;
	if(img.load(path))
	{
		showImageLabel->setPixmap(QPixmap::fromImage(img));
		// 调整显示大小
		QSize originalSize = img.size();
		currentSize = scrollArea->size() - QSize(2,2);
		if (originalSize.width() < currentSize.width() && (originalSize.height() < currentSize.height()))
		{
			currentSize = img.size();
			showImageLabel->resize(currentSize);
		}
		else
		{
			currentSize = img.size();
			currentSize.scale(scrollArea->size() - QSize(2,2), Qt::KeepAspectRatio);
			showImageLabel->resize(currentSize);
		}
		// 保留图片内容和路径
		image.image = img;
		imagePath = path;
		return true;
	}
	else return false;
}

void FShowImageLabel::closeEvent(QCloseEvent *event)
{
    emit labelCloseSignal(imagePath);
}

void FShowImageLabel::initAction()
{
	maxerImageAction = new QAction(tr("放大"),this);
	connect(maxerImageAction,SIGNAL(triggered()),this,SLOT(maxerImageSlot()));

	minnerImageAction = new QAction(tr("缩小"),this);
	connect(minnerImageAction,SIGNAL(triggered()),this,SLOT(minnerImageSlot()));

	userDefineSizeAction = new QAction(tr("自定义大小"),this);
	connect(userDefineSizeAction,SIGNAL(triggered()),this,SLOT(userDefineSizeSlot()));

	originalImageAction = new QAction(tr("原始大小"),this);
	connect(originalImageAction,SIGNAL(triggered()),this,SLOT(originalImageSlot()));

	fitWindowAction = new QAction(tr("适合窗口"),this);
	connect(fitWindowAction,SIGNAL(triggered()),this,SLOT(fitWindowSlot()));
}
void FShowImageLabel::initMenu()
{
	menu = new QMenu(this);
	menu->addAction(maxerImageAction);
	menu->addAction(minnerImageAction);
	menu->addAction(userDefineSizeAction);
	menu->addAction(originalImageAction);
	menu->addAction(fitWindowAction);
}
void FShowImageLabel::contextMenuEvent(QContextMenuEvent* event)
{
	menu->exec( event->globalPos());
	QWidget::contextMenuEvent( event);
}
void FShowImageLabel::maxerImageSlot()
{
	currentSize *= maxerFactor;
	setSize();
}
void FShowImageLabel::minnerImageSlot()
{
	currentSize *= minnerFactor;
	setSize();
}
void FShowImageLabel::userDefineSizeSlot()
{
	setImageSizeDialog = new FSetImageSizeDialog;
	setImageSizeDialog->show();
	connect(setImageSizeDialog->surePushButton,SIGNAL(clicked()),this,SLOT(surePushButtonClickSlot()));
}
void FShowImageLabel::surePushButtonClickSlot()
{
	userDefineImageWidth = setImageSizeDialog->widthLineEdit->text().toInt();
	userDefineImageHeight = setImageSizeDialog->heightLineEdit->text().toInt();
	if(userDefineImageWidth>0 && userDefineImageWidth<5000 
		&& userDefineImageHeight>0 && userDefineImageHeight<5000)
	{	
		currentSize.setWidth(userDefineImageWidth);
		currentSize.setHeight(userDefineImageHeight);
		setSize();
		setImageSizeDialog->close();
	}
	else
	{
		 QMessageBox::warning(this,tr("警告"),tr("输入不合法"),QMessageBox::Yes);
		 QApplication::setActiveWindow(setImageSizeDialog);
	}
}
void FShowImageLabel::originalImageSlot()
{
	currentSize = image.image.size();
	setSize();
}
void FShowImageLabel::fitWindowSlot()
{
	currentSize = image.image.size();
	currentSize.scale( scrollArea->size()-QSize(2,2), Qt::KeepAspectRatio);
	setSize();
}
void FShowImageLabel::setSize()
{
	showImageLabel->resize( currentSize);
	showImageLabel->setToolTip( QString("%1 * %2")
		.arg( QString::number( currentSize.width()))
		.arg( QString::number( currentSize.height())));
}

//void FShowImageLabel::showNotDealImage(QString currentItemFileName,QHash<QString,QString> *hash)
//{
//	if(!image.image.load(hash->value(currentItemFileName)))
//	{
//		QMessageBox notImageMessage;
//		notImageMessage.setText(tr("打开的不是图片文件"));
//		return;
//	}
//	this->setWindowTitle(currentItemFileName);
//	showImageLabel->setPixmap(QPixmap(hash->value(currentItemFileName)));
//	QSize imageNormalSize = image.image.size();
//	currentSize = scrollArea->size() - QSize(2,2);
//	if( imageNormalSize.width() < currentSize.width() && ( imageNormalSize.height() < currentSize.height() ))
//	{
//		currentSize = image.image.size();
//		showImageLabel->resize( currentSize);
//	}
//	else
//	{
//		currentSize = image.image.size();
//		currentSize.scale( scrollArea->size()-QSize(2,2), Qt::KeepAspectRatio);
//		showImageLabel->resize( currentSize);
//	}
//}
//void FShowImageLabel::showDealtImage(Image currentDealtImage)
//{
//	image = currentDealtImage;
//	QString currentDealtImageName;
//	currentDealtImageName.append(currentDealtImage.imageName);
//	this->setWindowTitle(currentDealtImageName);
//	showImageLabel->setPixmap( QPixmap::fromImage(currentDealtImage.image));
//	QSize imageNormalSize = currentDealtImage.image.size();
//	currentSize = scrollArea->size() - QSize(2,2);
//	if( imageNormalSize.width() < currentSize.width() && ( imageNormalSize.height() < currentSize.height() ))
//	{
//		currentSize = currentDealtImage.image.size();
//		showImageLabel->resize( currentSize);
//	}
//	else
//	{
//		currentSize = currentDealtImage.image.size();
//		currentSize.scale( scrollArea->size()-QSize(2,2), Qt::KeepAspectRatio);
//		showImageLabel->resize( currentSize);
//	}
//}
