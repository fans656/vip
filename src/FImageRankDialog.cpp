#include "FImageRankDialog.h"
#include "FNotDealImageWidget.h"
#include "FNotDealImageWidget.h"
#include <QtGui>

FImageRankDialog::FImageRankDialog(QStringList paths)
{
	initWindow();
    initImageData(paths);
}

void FImageRankDialog::initWindow()
{
	imageListWidget = new QListWidget;

	upButton = new QPushButton(tr("上移"));
	connect(upButton,SIGNAL(clicked()),this ,SLOT(upImageSlot()));

	downButton = new QPushButton(tr("下移"));
	connect(downButton,SIGNAL(clicked()),this,SLOT(downImageSlot()));

	okButton = new QPushButton(tr("确定"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

	QVBoxLayout *vlayout1 = new QVBoxLayout;
	vlayout1->addWidget(upButton);
	vlayout1->addStretch();
	vlayout1->addWidget(downButton);
	vlayout1->addStretch();
	vlayout1->addWidget(okButton);
	vlayout1->addStretch();

	QHBoxLayout *hlayout2 = new QHBoxLayout;
	hlayout2->addWidget(imageListWidget);
    hlayout2->addLayout(vlayout1);
    this->setLayout(hlayout2);
    this->resize(350,200);
}

const QStringList FImageRankDialog::rearrangedPaths() const
{
    QStringList paths;
    for (int i = 0; i < imageListWidget->count(); ++i)
    {
        QListWidgetItem *item = imageListWidget->item(i);
        QString path = item->data(Qt::UserRole).toString();
        paths << path;
    }
    return paths;
}

void FImageRankDialog::initImageData(QStringList paths)
{
    foreach (QString path, paths)
    {
        QChar sep = (path.contains('/') ? '/' : '\\');    // "E:/tmp/1.jpg" or "E:\tmp\1.jpg"
        QString name = path.section(sep, -1);
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(name);
        item->setData(Qt::UserRole, path);
        imageListWidget->addItem(item);
    }
}

void FImageRankDialog::upImageSlot()
{
    int row = imageListWidget->currentRow();
    if (row > 0)
    {
        QListWidgetItem *item = imageListWidget->takeItem(row);
        imageListWidget->insertItem(row - 1, item);
        imageListWidget->setCurrentItem(item);
    }
}

void FImageRankDialog::downImageSlot()
{
    int row = imageListWidget->currentRow();
    if (row + 1 < imageListWidget->count())
	{
        QListWidgetItem *item = imageListWidget->takeItem(row);
        imageListWidget->insertItem(row + 1, item);
        imageListWidget->setCurrentItem(item);
	}
}

