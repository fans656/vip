#include "FSetWidget.h"

FSetWidget::FSetWidget()			
{
	createSetWidget();
	layoutSetWidget();
	initElement();
	this->setWindowTitle(tr("设置"));
}

FSetWidget::~FSetWidget(void)
{
}
void FSetWidget::createSetWidget()
{
	setListWidget = new QListWidget();
	setListWidget->setSpacing(5);
	setListWidget->setMovement(QListView::Static);
	setListWidget->setViewMode(QListView::ListMode);
	setListWidget->setMaximumWidth(100);
	connect(setListWidget,SIGNAL(currentRowChanged(int)),this,SLOT(setWidgetIndexChanged(int)));

	QListWidgetItem *basicSetItem = new QListWidgetItem(tr("基本设置"));
	QListWidgetItem *otherSetItem = new QListWidgetItem(tr("其它设置"));
	setListWidget->addItem(basicSetItem);
	setListWidget->addItem(otherSetItem);

	basicSetWidget = new BasicSetWidget();
	otherSetWidget = new OtherSetWidget();
	setStackWidget = new QStackedWidget();
	setStackWidget->addWidget(basicSetWidget);
	setStackWidget->addWidget(otherSetWidget);

	sureButton = new QPushButton(tr("确定"));
	connect(sureButton,SIGNAL(clicked()),this,SLOT(sureButtonClicked()));
	cancelButton  = new QPushButton(tr("取消"));
	connect(cancelButton,SIGNAL(clicked()),this,SLOT(cancelButtonClicked()));
	useButton = new QPushButton(tr("应用"));
	connect(useButton,SIGNAL(clicked()),this,SLOT(useButtonClicked()));
	connect(basicSetWidget,SIGNAL(deleteFileNowButtonClicked()),this,SIGNAL(deleteFileNow()));
}
void FSetWidget::layoutSetWidget()
{
	QHBoxLayout *hlayout1 = new QHBoxLayout();
	hlayout1->addWidget(setListWidget);
	hlayout1->addWidget(setStackWidget);
	
	QHBoxLayout *hlayout2 = new QHBoxLayout;
	hlayout2->addStretch();
	hlayout2->addWidget(sureButton);
	hlayout2->addWidget(cancelButton);
	hlayout2->addWidget(useButton);
	
	QVBoxLayout *vlayout = new QVBoxLayout;
	vlayout->addLayout(hlayout1);
	vlayout->addLayout(hlayout2);
	this->setLayout(vlayout);
	this->setFixedSize(500,400);
}
void FSetWidget::setWidgetIndexChanged(int index)
{
	setStackWidget->setCurrentIndex(index);
}
void FSetWidget::setRequiredSlot()
{
	this->show();
	initElement();
}
void FSetWidget::sureButtonClicked()
{
	if(useButtonClicked())
		this->hide();
}
bool FSetWidget::useButtonClicked()
{
	bool isSettingRight = checkSettings();
	if (!isSettingRight)
	{
		QMessageBox::about(this,tr("提示"),tr("设置参数有误"));
		return false;
	}
	QSettings settings("config.ini", QSettings::IniFormat);
	settings.setValue("defaultSavePath", basicSetWidget->getDefaultSavePath());
	settings.setValue("deleteFileCycle",basicSetWidget->getDeleteFileCycleDay());
	return true;
}
void FSetWidget::cancelButtonClicked()
{
	this->hide();
}
void FSetWidget::initElement()
{
	basicSetWidget->initDefaultSavePath();
	basicSetWidget->initDeleteFileCycle();
}
bool FSetWidget::checkSettings()
{
	bool isSettingRight = true;
	QString path = basicSetWidget->getDefaultSavePath();
	if(!checkPath(path))
	{
		isSettingRight = false;
		return isSettingRight;
	}
	return isSettingRight;
}
bool FSetWidget::checkPath(QString path)
{
	QDir *temp = new QDir(path);
	if(path == "")
		return false;
	bool abso = temp->isAbsolute();
	if (!abso)
	{
		return false;
	}
	else
	{
		return true;
	}
}
FSetWidget::BasicSetWidget::BasicSetWidget()
{
	defaultSavePath = QDir::currentPath() + "/User Data";
	createWidget();
	initData();
	layoutWidget();
}
void FSetWidget::BasicSetWidget::createWidget()
{
	defaultPathLabel = new QLabel(tr("默认把保存的图片保存在以下路径:"));
	saveGroupBox = new QGroupBox(tr("保存"));
	defaultPathLineEdit = new QLineEdit;
	defaultPathLineEdit->setText(defaultSavePath);
	scanDefaultPathButton = new QPushButton(tr("更改路径"));
	connect(scanDefaultPathButton,SIGNAL(clicked()),this,SLOT(scanDefaultPathButtonClick()));

	deleteFileGroupBox = new QGroupBox(tr("文件清理"));
	deleteFileLabel = new QLabel(tr("定期进行文件清理可以有效的节省空间"));
	deleteFileNowButton = new QPushButton(tr("立即清理"));
	connect(deleteFileNowButton,SIGNAL(clicked()),this,SIGNAL(deleteFileNowButtonClicked()));
	deleteFileCycleLabel = new QLabel(tr("定时清理文件的周期:"));
	deleteFileComboBox = new QComboBox();
	connect(deleteFileComboBox,SIGNAL(currentIndexChanged(int )),
		this,SLOT(deleteFileComboBoxIndexChanged(int)));
}
void FSetWidget::BasicSetWidget::initData()
{
	deleteFileComboBox->addItem(tr("一天"));
	deleteFileComboBox->addItem(tr("两天"));
	deleteFileComboBox->addItem(tr("三天"));
	deleteFileComboBox->addItem(tr("一周"));
	deleteFileComboBox->addItem(tr("一个月"));
	deleteFileComboBox->addItem(tr("从不清理"));
	deleteFileComboBox->addItem(tr("每次启动时"));

	deleteFileComBoxHash.insert("1",0);
	deleteFileComBoxHash.insert("2",1);
	deleteFileComBoxHash.insert("3",2);
	deleteFileComBoxHash.insert("7",3);
	deleteFileComBoxHash.insert("30",4);
	deleteFileComBoxHash.insert("0",5);
	deleteFileComBoxHash.insert("-1",6);
}
void FSetWidget::BasicSetWidget::layoutWidget()
{
	QHBoxLayout *hlayout1 = new QHBoxLayout;
	hlayout1->addWidget(defaultPathLineEdit);
	hlayout1->addWidget(scanDefaultPathButton);

	QVBoxLayout *vlayout1 = new QVBoxLayout;
	vlayout1->addWidget(defaultPathLabel);
	vlayout1->addLayout(hlayout1);
	saveGroupBox->setLayout(vlayout1);
	
	QHBoxLayout *hlayout2 = new QHBoxLayout;
	hlayout2->addWidget(deleteFileLabel);
	hlayout2->addStretch();
	hlayout2->addWidget(deleteFileNowButton);

	QHBoxLayout *hlayout3 = new QHBoxLayout;
	hlayout3->addWidget(deleteFileCycleLabel);
	hlayout3->addWidget(deleteFileComboBox);

	QVBoxLayout *vlayout2 = new QVBoxLayout;
	vlayout2->addLayout(hlayout2);
	vlayout2->addLayout(hlayout3);
	deleteFileGroupBox->setLayout(vlayout2);

	QVBoxLayout *vlayout3 = new QVBoxLayout;
	vlayout3->addWidget(saveGroupBox);
	vlayout3->addStretch();
	vlayout3->addWidget(deleteFileGroupBox);
	vlayout3->addStretch();
	this->setLayout(vlayout3);
}
void FSetWidget::BasicSetWidget::scanDefaultPathButtonClick()
{
	const QString dirPath = QFileDialog::getExistingDirectory( NULL, tr("保存图片路径"), defaultSavePath);
	if( !QFileInfo( dirPath).exists())
	{
		return;
	}
	defaultSavePath = dirPath;
	defaultPathLineEdit->setText(dirPath);
}
void FSetWidget::BasicSetWidget::deleteFileComboBoxIndexChanged(int index)
{
	switch(index)
	{
	case 0:deleteFileCycleDay = "1";break;
	case 1:deleteFileCycleDay = "2";break;
	case 2:deleteFileCycleDay = "3";break;
	case 3:deleteFileCycleDay = "7";break;
	case 4:deleteFileCycleDay = "30";break;
	case 5:deleteFileCycleDay = "0";break;
	case 6:deleteFileCycleDay = "-1";break;
	default:
		break;
	}
}
FSetWidget::OtherSetWidget::OtherSetWidget()
{
	
}