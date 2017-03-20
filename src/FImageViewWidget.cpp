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
			emit hasOutPut(path + tr("为非图片文件，显示图片失败"));
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
	dockWidget = new QDockWidget(tr("图层"));
	dockWidget->setObjectName(tr("dock"));
	dockWidget->setWidget(imageClassWidget);
	dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea,dockWidget);
	dockWidget->hide();*/

	setToolButtonStyle( Qt::ToolButtonTextBesideIcon);
	setWindowTitle( tr("图片显示"));	

	maxerImageAction = new QAction(tr("放大"),this);
	connect(maxerImageAction,SIGNAL(triggered()),this,SLOT(maxerImageSlot()));

	minnerImageAction = new QAction(tr("缩小"),this);
	connect(minnerImageAction,SIGNAL(triggered()),this,SLOT(minnerImageSlot()));
	
	userDefineSizeAction = new QAction(tr("自定义图片大小"),this);
	connect(userDefineSizeAction,SIGNAL(triggered()),this,SLOT(userDefineSizeSlot()));

	originalImageAction = new QAction(tr("原始图片"),this);
	connect(originalImageAction,SIGNAL(triggered()),this,SLOT(originalImageSlot()));

	fitWindowAction = new QAction(tr("适合窗口"),this);
	connect(fitWindowAction,SIGNAL(triggered()),this,SLOT(fitWindowSlot()));

	viewModeAction = new QAction(tr("多文档视图"),this);
	connect(viewModeAction,SIGNAL(triggered()),this,SLOT(viewModeSlot()));

	duiDieViewAction = new QAction(tr("堆叠显示"),this);
	connect(duiDieViewAction, SIGNAL(triggered()),mdiArea, SLOT(cascadeSubWindows()));

	bingPaiViewAction = new QAction(tr("并排显示"),this);
	connect(bingPaiViewAction, SIGNAL(triggered()),mdiArea, SLOT(tileSubWindows()));

	prevImageAction = new QAction(tr("上一张"),this);
	connect(prevImageAction,SIGNAL(triggered()),this,SLOT(prevImage()));

	nextImageAction = new QAction(tr("下一张"),this);
	connect(nextImageAction,SIGNAL(triggered()),this,SLOT(nextImage()));

	closeImageAction = new QAction(tr("关闭"),this);
	connect(closeImageAction,SIGNAL(triggered()),this,SLOT(closeImageSlot()));

	viewToolBar = addToolBar(tr("视图"));
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
			viewModeAction->setText( tr("多文档模式"));
			currentViewMode=QMdiArea::TabbedView;
			bingPaiViewAction->setEnabled( false);
			duiDieViewAction->setEnabled( false);
			emit setMianWindowActionFalseSignal();
			break;
		}
	case QMdiArea::TabbedView :
		{
			mdiArea->setViewMode(QMdiArea::SubWindowView);
			viewModeAction->setText( tr("标签页模式"));
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

// 显示图片的signal由MainWindow转发，slot为showImage(QString path)，以下函数不再需要

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
//    // 图片未打开
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
//    // 图片已打开
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
