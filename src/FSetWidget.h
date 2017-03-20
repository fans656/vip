/*************************************************
  Author: 成超      Version:1.2        Date:2012.12.27 
  Description:留出接口给用户提供权限，对VIP程序中的某些自定义属性进行更改或设置。    
  Interface:
    1. public:
       (1)...
    2. public slots:
       (1)...
    3. signal:
       (1)...
  History:        
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#pragma once
#include <QtGui>

class FSetWidget : public QWidget
{
	Q_OBJECT
public:
	FSetWidget();         
	~FSetWidget(void);
private:

	class BasicSetWidget;
	class OtherSetWidget;

	void createSetWidget();
	void layoutSetWidget();
	void initElement();
	bool checkSettings();
	bool checkPath(QString path);
	QListWidget *setListWidget;
	QStackedWidget *setStackWidget;
	BasicSetWidget *basicSetWidget;
	OtherSetWidget *otherSetWidget;
	QPushButton *sureButton;
	QPushButton *cancelButton;
	QPushButton *useButton;
signals:
	void deleteFileNow();
private slots:
	void setWidgetIndexChanged(int index);
	void setRequiredSlot();
	void sureButtonClicked();
	void cancelButtonClicked();
	bool useButtonClicked();

};
class FSetWidget::BasicSetWidget : public QWidget
{
	Q_OBJECT
public:
	BasicSetWidget();
	QString getDefaultSavePath()
	{
		return defaultPathLineEdit->text();
	};
	QString getDeleteFileCycleDay()
	{
		return deleteFileCycleDay;
	};
	void initDefaultSavePath()
	{
		QSettings settings("config.ini", QSettings::IniFormat);
		if (!settings.contains("defaultSavePath"))
		{
			settings.setValue("defaultSavePath",defaultSavePath);
		}
		defaultSavePath = settings.value("defaultSavePath").toString();

		QDir *temp = new QDir(defaultSavePath);
		bool exist = temp->exists();
		if(!exist || defaultSavePath == "")
		{
			bool abso = temp->isAbsolute();
			if (!abso)
			{	
				defaultSavePath = QDir::currentPath() + "/User Data";
				settings.setValue("defaultSavePath",defaultSavePath);
			}
			else
			{
				temp->mkpath(defaultSavePath);    //创建文件夹
			}
		}
		defaultPathLineEdit->setText(defaultSavePath);
	};
	void initDeleteFileCycle()
	{
		QSettings settings("config.ini", QSettings::IniFormat);
		if (!settings.contains("deleteFileCycle"))
		{
			settings.setValue("deleteFileCycle","-1");
			deleteFileComboBox->setCurrentIndex(6);
		}
		deleteFileCycleDay = settings.value("deleteFileCycle").toString();
		if (deleteFileCycleDay == "-1" || deleteFileCycleDay == "0" || deleteFileCycleDay == "1"
			|| deleteFileCycleDay == "2" || deleteFileCycleDay == "3" || deleteFileCycleDay == "7"
			|| deleteFileCycleDay == "30")
		{
			deleteFileComboBox->setCurrentIndex(deleteFileComBoxHash.value(deleteFileCycleDay));
		}
		else
		{
			settings.setValue("deleteFileCycle","-1");
			deleteFileComboBox->setCurrentIndex(6);
		}
		deleteFileCycleDay = settings.value("deleteFileCycle").toString();
	};
private:
	void createWidget();
	void layoutWidget();
	void initData();
	QHash<QString ,int> deleteFileComBoxHash;

	QString defaultSavePath;
	QString deleteFileCycleDay;
	QLabel *defaultPathLabel;
	QGroupBox *saveGroupBox;
	QLineEdit *defaultPathLineEdit;
	QPushButton *scanDefaultPathButton;

	QGroupBox *deleteFileGroupBox;
	QLabel *deleteFileLabel;
	QLabel *deleteFileCycleLabel;
	QComboBox *deleteFileComboBox;
	QPushButton *deleteFileNowButton;
signals:
	void deleteFileNowButtonClicked();
private slots:
	void scanDefaultPathButtonClick();
	void deleteFileComboBoxIndexChanged(int index);

};
class FSetWidget::OtherSetWidget : public QWidget
{
	Q_OBJECT
public:
	OtherSetWidget();
private:
};
