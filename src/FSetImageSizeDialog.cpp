#include "FSetImageSizeDialog.h"

FSetImageSizeDialog::FSetImageSizeDialog(void)
{
	createSetImageSizeDialog();
}

FSetImageSizeDialog::~FSetImageSizeDialog(void)
{
}
void FSetImageSizeDialog::createSetImageSizeDialog()
{
	widthLabel = new QLabel(tr("图片宽度："));
	heightLabel = new QLabel(tr("图片高度："));
	widthLineEdit = new QLineEdit;
	heightLineEdit = new QLineEdit;
	surePushButton = new QPushButton(tr("确定"));
	cancelPushButton = new QPushButton(tr("取消"));

	QHBoxLayout *hlayout1 = new QHBoxLayout;
	hlayout1->addWidget(widthLabel);
	hlayout1->addWidget(widthLineEdit);
	QHBoxLayout *hlayout2 = new QHBoxLayout;
	hlayout2->addWidget(heightLabel);
	hlayout2->addWidget(heightLineEdit);
	QHBoxLayout *hlayout3 = new QHBoxLayout;
	hlayout3->addWidget(surePushButton);
	hlayout3->addWidget(cancelPushButton);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(hlayout1);
	mainLayout->addLayout(hlayout2);
	mainLayout->addLayout(hlayout3);
	this->setLayout(mainLayout);
	connect(cancelPushButton,SIGNAL(clicked()),this,SLOT(close()));
}
