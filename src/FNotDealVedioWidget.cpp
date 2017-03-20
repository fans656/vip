#include "FNotDealVedioWidget.h"
#include <QCursor>
#include <QMenu>
#include "stdafx.h"

FNotDealVedioWidget::FNotDealVedioWidget()
{
	notDealVedioWidgetLayout();
	initNotDealVedioWidget();
}

FNotDealVedioWidget::~FNotDealVedioWidget()
{
}
void FNotDealVedioWidget::notDealVedioWidgetLayout()
{
	playVedioButton = new QPushButton;
	playVedioButton->setFixedSize(70,50);
	playVedioButton->setIconSize(QSize(65,45));
	QIcon icon1(":/icon/selectAllButton.png");
	playVedioButton->setIcon(icon1);   //初始化

	deleteVedioButton = new QPushButton;
	deleteVedioButton->setFixedSize(70,50);
	deleteVedioButton->setIconSize(QSize(65,45));
	QIcon icon2(":/icon/deleteButton.png");
	deleteVedioButton->setIcon(icon2);   //初始化

	dealVedioButton = new QPushButton;
	dealVedioButton->setFixedSize(70,50);
	dealVedioButton->setIconSize(QSize(65,45));
	QIcon icon3(":/icon/dealImageButton.png");
	dealVedioButton->setIcon(icon3);   //初始化
	
	notDealVedioList = new QListWidget;
	//notDealVedioList->setFixedSize(297,340);
	notDealVedioList->setMovement(QListView::Static);

	QHBoxLayout *hlayout1 = new QHBoxLayout;
	hlayout1->addWidget(deleteVedioButton);
	hlayout1->addWidget(playVedioButton);
	hlayout1->addWidget(dealVedioButton);
	hlayout1->addStretch();


	QVBoxLayout *vlayout3 = new QVBoxLayout;
	vlayout3->addLayout(hlayout1);
	vlayout3->addWidget(notDealVedioList);
	vlayout3->addStretch();

	this->setLayout(vlayout3);
}
void FNotDealVedioWidget::initNotDealVedioWidget()
{
	/*isAllSelected = false;
	isArithmeticDialogNew = false;
	notDealImageList->setSelectionMode(QAbstractItemView::MultiSelection);
	selectAllButton->setEnabled(false);
	deleteImageButton->setEnabled(false);
	dealImageButton->setEnabled(false);
	showImageButton->setEnabled(false);*/
}
