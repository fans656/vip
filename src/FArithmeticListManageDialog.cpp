#include "FArithmeticListManageDialog.h"
#include "FArithmeticDialog.h"

FArithmeticListManageDialog::FArithmeticListManageDialog()
{
	createDialog();
	this->setWindowTitle(tr("�㷨����"));
	this->setFixedSize(200,150);
}
FArithmeticListManageDialog::~FArithmeticListManageDialog()
{
}

void FArithmeticListManageDialog::createDialog()
{
	arithmeticListSaveWidget = new QWidget;
	nameLineEdit = new QLineEdit;
	nameLabel = new QLabel(tr("  �㷨��������:"));

	saveButton = new QPushButton(tr("����"));
	connect(saveButton,SIGNAL(clicked()),this,SLOT(ArithmeticListSave()));

	cancelButton = new QPushButton(tr("ȡ��"));
	connect(cancelButton,SIGNAL(clicked()),this,SLOT(ArithmeticListCancel()));

	QHBoxLayout *hlayout1 = new QHBoxLayout;
	hlayout1->addWidget(saveButton);
	hlayout1->addWidget(cancelButton);

	QHBoxLayout *hlayout2 = new QHBoxLayout;
	hlayout2->addWidget(nameLabel);
	hlayout2->addWidget(nameLineEdit);

	QVBoxLayout *vlayout1 = new QVBoxLayout;
	vlayout1->addLayout(hlayout2);
	vlayout1->addLayout(hlayout1);
	arithmeticListSaveWidget->setLayout(hlayout1);

	this->setLayout(vlayout1);
}

void FArithmeticListManageDialog::ArithmeticListSave()
{
		if(nameLineEdit->text().isEmpty())
		{
		   QMessageBox::warning(this,tr("error"),tr("���Ʋ���Ϊ�գ�����������"),QMessageBox::Yes);
		}
		else
			this->accept();
}

void FArithmeticListManageDialog::ArithmeticListCancel()
{
	this->reject();
}
