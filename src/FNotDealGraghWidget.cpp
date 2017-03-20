#include "FNotDealGraghWidget.h"
#include <QCursor>
#include <QMenu>
#include "stdafx.h"

FNotDealGraghWidget::FNotDealGraghWidget()
{
	notDealGraghWidgetLayout();
	initNotDealGraghWidget();
}

FNotDealGraghWidget::~FNotDealGraghWidget()
{
}
void FNotDealGraghWidget::notDealGraghWidgetLayout()
{
	showGraghButton = new QPushButton;
	showGraghButton->setFixedSize(70,50);
	showGraghButton->setIconSize(QSize(65,45));
	QIcon icon1(":/icon/selectAllButton.png");
	showGraghButton->setIcon(icon1);   //初始化

	deleteGraghButton = new QPushButton;
	deleteGraghButton->setFixedSize(70,50);
	deleteGraghButton->setIconSize(QSize(65,45));
	QIcon icon2(":/icon/deleteButton.png");
	deleteGraghButton->setIcon(icon2);   //初始化

	dealGraghButton = new QPushButton;
	dealGraghButton->setFixedSize(70,50);
	dealGraghButton->setIconSize(QSize(65,45));
	QIcon icon3(":/icon/dealImageButton.png");
	dealGraghButton->setIcon(icon3);   //初始化
	
	notDealGraghList = new QListWidget;
	//notDealGraghList->setFixedSize(297,340);
	notDealGraghList->setMovement(QListView::Static);

	QHBoxLayout *hlayout1 = new QHBoxLayout;
	hlayout1->addWidget(deleteGraghButton);
	hlayout1->addWidget(showGraghButton);
	hlayout1->addWidget(dealGraghButton);
	hlayout1->addStretch();


	QVBoxLayout *vlayout3 = new QVBoxLayout;
	vlayout3->addLayout(hlayout1);
	vlayout3->addWidget(notDealGraghList);
	vlayout3->addStretch();

	this->setLayout(vlayout3);
}
void FNotDealGraghWidget::initNotDealGraghWidget()
{
	/*isAllSelected = false;
	isArithmeticDialogNew = false;
	notDealImageList->setSelectionMode(QAbstractItemView::MultiSelection);
	selectAllButton->setEnabled(false);
	deleteImageButton->setEnabled(false);
	dealImageButton->setEnabled(false);
	showImageButton->setEnabled(false);*/
}
