#include "FImageViewWidget.h"
#include "FShowImageLabel.h"
#include "FShowImageLabel.h"

FImageViewWidget::FImageViewWidget(QWidget *parent)
	: QMainWindow( parent) , currentViewMode( QMdiArea::SubWindowView)
{
	mainViewWidgetLayout();
}

void FImageViewWidget::showImage(QString path)
{
	bool isImage = false;
    QMdiSubWindow *window = imageSubWindowHash.value(path, 0);
    if(window)
    {
        mdiArea->setActiveSubWindow(window);
    }
    else
    {
        FShowImageLabel *label = new FShowImageLabel;
        isImage = label->showImage(path);
		if(!isImage)
			emit hasOutPut(path + tr("Ϊ��ͼƬ�ļ�����ʾͼƬʧ��"));
        connect(label, SIGNAL(labelCloseSignal(QString)), this, SLOT(closeImageFile(QString)));

        window = new QMdiSubWindow;
        window->setWidget(label);
        window->setAttribute(Qt::WA_DeleteOnClose);
        mdiArea->addSubWindow(window);
        window->show();

        imageSubWindowHash.insert(path, window);
    }
}

void FImageViewWidget::cascadeSubWindows()
{
    mdiArea->cascadeSubWindows();
}

void FImageViewWidget::tileSubWindows()
{
    mdiArea->tileSubWindows();
}

void FImageViewWidget::mainViewWidgetLayout()
{
	mdiArea = new QMdiArea(NULL);
	setCentralWidget(mdiArea);
	/*imageClassWidget = new FimageClassWidget();
	dockWidget = new QDockWidget(tr("ͼ��"));
	dockWidget->setObjectName(tr("dock"));
	dockWidget->setWidget(imageClassWidget);
	dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea,dockWidget);
	dockWidget->hide();*/

	setToolButtonStyle( Qt::ToolButtonTextBesideIcon);
	setWindowTitle( tr("ͼƬ��ʾ"));	

	maxerImageAction = new QAction(tr("�Ŵ�"),this);
	connect(maxerImageAction,SIGNAL(triggered()),this,SLOT(maxerImageSlot()));

	minnerImageAction = new QAction(tr("��С"),this);
	connect(minnerImageAction,SIGNAL(triggered()),this,SLOT(minnerImageSlot()));
	
	userDefineSizeAction = new QAction(tr("�Զ���ͼƬ��С"),this);
	connect(userDefineSizeAction,SIGNAL(triggered()),this,SLOT(userDefineSizeSlot()));

	originalImageAction = new QAction(tr("ԭʼͼƬ"),this);
	connect(originalImageAction,SIGNAL(triggered()),this,SLOT(originalImageSlot()));

	fitWindowAction = new QAction(tr("�ʺϴ���"),this);
	connect(fitWindowAction,SIGNAL(triggered()),this,SLOT(fitWindowSlot()));

	viewModeAction = new QAction(tr("���ĵ���ͼ"),this);
	connect(viewModeAction,SIGNAL(triggered()),this,SLOT(viewModeSlot()));

	duiDieViewAction = new QAction(tr("�ѵ���ʾ"),this);
	connect(duiDieViewAction, SIGNAL(triggered()),mdiArea, SLOT(cascadeSubWindows()));

	bingPaiViewAction = new QAction(tr("������ʾ"),this);
	connect(bingPaiViewAction, SIGNAL(triggered()),mdiArea, SLOT(tileSubWindows()));

	prevImageAction = new QAction(tr("��һ��"),this);
	connect(prevImageAction,SIGNAL(triggered()),this,SLOT(prevImage()));

	nextImageAction = new QAction(tr("��һ��"),this);
	connect(nextImageAction,SIGNAL(triggered()),this,SLOT(nextImage()));

	closeImageAction = new QAction(tr("�ر�"),this);
	connect(closeImageAction,SIGNAL(triggered()),this,SLOT(closeImageSlot()));

	viewToolBar = addToolBar(tr("��ͼ"));
	viewToolBar->setMovable(true);
	viewToolBar->setIconSize( QSize(16,16));
	viewToolBar->addAction(maxerImageAction);
	viewToolBar->addAction(minnerImageAction);
	viewToolBar->addAction(userDefineSizeAction);
	viewToolBar->addAction(originalImageAction);
	viewToolBar->addAction(fitWindowAction);
	viewToolBar->addAction(viewModeAction);
	viewToolBar->addAction(duiDieViewAction);
	viewToolBar->addAction(bingPaiViewAction);
	viewToolBar->addAction(prevImageAction);
	viewToolBar->addAction(nextImageAction);
	viewToolBar->addAction(closeImageAction);

	viewModeSlot();
}
void FImageViewWidget::viewModeSlot()
{
	switch( currentViewMode)
	{
	case QMdiArea::SubWindowView :
		{
			mdiArea->setViewMode(QMdiArea::TabbedView);
			viewModeAction->setText( tr("���ĵ�ģʽ"));
			currentViewMode=QMdiArea::TabbedView;
			bingPaiViewAction->setEnabled( false);
			duiDieViewAction->setEnabled( false);
			emit setMianWindowActionFalseSignal();
			break;
		}
	case QMdiArea::TabbedView :
		{
			mdiArea->setViewMode(QMdiArea::SubWindowView);
			viewModeAction->setText( tr("��ǩҳģʽ"));
			currentViewMode=QMdiArea::SubWindowView;
			bingPaiViewAction->setEnabled( true);
			duiDieViewAction->setEnabled( true);
			emit setMianWindowActionTrueSignal();
			break;
		}
	}
}
void FImageViewWidget::maxerImageSlot()
{
	if(mdiArea->activeSubWindow () != NULL)
    {
		QMdiSubWindow* subWindow = mdiArea->activeSubWindow();
		Q_ASSERT( subWindow );
		qobject_cast<FShowImageLabel*>(subWindow->widget())->maxerImageSlot();
	}
}
void FImageViewWidget::minnerImageSlot()
{
	if(mdiArea->activeSubWindow () != NULL)
	{
		QMdiSubWindow* subWindow = mdiArea->activeSubWindow();
		Q_ASSERT( subWindow );
		qobject_cast<FShowImageLabel*>(subWindow->widget())->minnerImageSlot();
	}
}
void FImageViewWidget::userDefineSizeSlot()
{
	if(mdiArea->activeSubWindow () != NULL)
	{
		QMdiSubWindow* subWindow = mdiArea->activeSubWindow();
		Q_ASSERT( subWindow );
		qobject_cast<FShowImageLabel*>(subWindow->widget())->userDefineSizeSlot();
	}
}
void FImageViewWidget::originalImageSlot()
{
	if(mdiArea->activeSubWindow () != NULL)
	{
		QMdiSubWindow* subWindow = mdiArea->activeSubWindow();
		Q_ASSERT( subWindow );
		qobject_cast<FShowImageLabel*>(subWindow->widget())->originalImageSlot();
	}
}
void FImageViewWidget::fitWindowSlot()
{
	if(mdiArea->activeSubWindow () != NULL)
	{
		QMdiSubWindow* subWindow = mdiArea->activeSubWindow();
		Q_ASSERT( subWindow );
		qobject_cast<FShowImageLabel*>(subWindow->widget())->fitWindowSlot();
	}
}
void FImageViewWidget::closeImageSlot()
{
	if(mdiArea->activeSubWindow () != NULL)
	{
		QMdiSubWindow* subWindow = mdiArea->activeSubWindow();
		Q_ASSERT( subWindow );
		mdiArea->activatePreviousSubWindow();
		if ( subWindow == mdiArea->activeSubWindow() )
		{
			mdiArea->activatePreviousSubWindow();
		}
        subWindow->close();
	}
}

void FImageViewWidget::prevImage()
{
	if(mdiArea->activeSubWindow () != NULL)
	{
		QMdiSubWindow* subWindow = mdiArea->activeSubWindow();
		Q_ASSERT( subWindow );
		mdiArea->activatePreviousSubWindow();
	}
}
void FImageViewWidget::nextImage()
{
	if(mdiArea->activeSubWindow () != NULL)
	{
		QMdiSubWindow* subWindow = mdiArea->activeSubWindow();
		Q_ASSERT( subWindow );
		mdiArea->activateNextSubWindow();
	}
}
void FImageViewWidget::closeImageFile(QString path)
{
    imageSubWindowHash.remove(path);
}

// ��ʾͼƬ��signal��MainWindowת����slotΪshowImage(QString path)�����º���������Ҫ

//void FImageViewWidget::notDealImageItemDoubleClickSlot(QString imagePath,QHash<QString,QString> *hash)
//{
//	this->hashForNotDealImage = hash;
//	imageDisplay(imagePath);
//}
//void FImageViewWidget::dealtImageItemDoubleClickSlot(Image currentDealtImage,QHash<QString,QString> *hash)
//{
//	this->hashForDealtImage = hash;
//	dealtImageDisplay(currentDealtImage);
//}
//void FImageViewWidget::dealtImageDisplay(Image currentDealtImage)
//{
//	QString currentDealtImageName = currentDealtImage.imageName;
//	//QString currentDealtImageName = "dealt_";
//	//currentDealtImageName.append(currentDealtImage.imageName);
//	if(!hashForOpenImage->contains(currentDealtImageName))
//	{
//		FShowImageLabel *showImageLabel = new FShowImageLabel;
//        showImageLabel->showDealtImage(currentDealtImage);
//		connect(showImageLabel,SIGNAL(labelCloseSignal(QString)),
//			this,SLOT(showImageLabelClosedSlot(QString)));
//		QMdiSubWindow *subWindow = new QMdiSubWindow;
//		subWindow->setWidget(showImageLabel);
//		subWindow->setAttribute(Qt::WA_DeleteOnClose);
//		mdiArea->addSubWindow(subWindow);
//		subWindow->show();
//		hashForOpenImage->insert(currentDealtImageName,subWindow);
//	}
//	else
//	{
//		mdiArea->setActiveSubWindow( hashForOpenImage->value(currentDealtImageName));
//	}
//}
//void FImageViewWidget::imageDisplay(QString currentItemFileName)
//{
//    // ͼƬδ��
//	if(!hashForOpenImage->contains(currentItemFileName))
//	{
//		FShowImageLabel *showImageLabel = new FShowImageLabel;
//		showImageLabel->showNotDealImage(currentItemFileName,hashForNotDealImage);
//		connect(showImageLabel,SIGNAL(labelCloseSignal(QString)),
//			this,SLOT(showImageLabelClosedSlot(QString)));
//		QMdiSubWindow *subWindow = new QMdiSubWindow;
//		subWindow->setWidget(showImageLabel);
//		subWindow->setAttribute(Qt::WA_DeleteOnClose);
//		mdiArea->addSubWindow(subWindow);
//		subWindow->show();
//		hashForOpenImage->insert(currentItemFileName,subWindow);
//	}
//    // ͼƬ�Ѵ�
//	else
//	{
//		mdiArea->setActiveSubWindow( hashForOpenImage->value(currentItemFileName));
//	}
//}
//void FImageViewWidget::notDealImageWidgetShowImageButtonClickedSlot(
//	QStringList notDealImageWidgetCheckedItemList,QHash<QString,QString> *hash)
//{
//	this->hashForNotDealImage = hash;
//	imageDisplay(notDealImageWidgetCheckedItemList);
//}
//void FImageViewWidget::imageDisplay(QStringList currentItemFileNameList)
//{
//	for(int i = 0;i<currentItemFileNameList.size();i++)
//	{
//		if(!hashForOpenImage->contains(currentItemFileNameList[i]))
//		{
//			FShowImageLabel *showImageLabel = new FShowImageLabel;
//			showImageLabel->showNotDealImage(currentItemFileNameList[i],hashForNotDealImage);
//			connect(showImageLabel,SIGNAL(labelCloseSignal(QString)),
//				this,SLOT(showImageLabelClosedSlot(QString)));
//			QMdiSubWindow *subWindow = new QMdiSubWindow;
//			subWindow->setWidget(showImageLabel);
//			subWindow->setAttribute(Qt::WA_DeleteOnClose);
//			mdiArea->addSubWindow(subWindow);
//			subWindow->show();
//			hashForOpenImage->insert(currentItemFileNameList[i],subWindow);
//		}
//	}
//}
//void FImageViewWidget::showImageLabelClosedSlot(QString currentLabelWindowTitle)
//{
//	hashForOpenImage->remove(currentLabelWindowTitle);
//}
