#include "FArithmeticInfoDialog.h"
#include <QSqlQuery>

FArithmeticInfoDialog::FArithmeticInfoDialog(QString aName)
    : name(aName)
{
    readInfo(); 
    initWindow(); 
    setConnect(); 
}

bool FArithmeticInfoDialog::isParamNeedSet()
{
    return (bool)paramInfoNumber; 
}

QList<QString> FArithmeticInfoDialog::getParamList()
{
    return paramList; 
}

void FArithmeticInfoDialog::readInfo()
{
    QSqlQuery query; 
    QString sql; 
    
    sql += "select A_paramNumber, A_paramDescription, A_paramType, "; 
    sql += "A_paramMin, A_paramMax from Arithmetic where A_name = '"; 
    sql += this->name + "'"; 
    
    query.exec(sql); 
    if (query.next())
    {
        paramInfoNumber = query.value(0).toInt(); 
        if (paramInfoNumber > 0)
        {
            setParamInfoList(paramDescriptionList, query.value(1).toString()); 
            setParamInfoList(paramTypeList, query.value(2).toString()); 
            setParamInfoList(paramMinList, query.value(3).toString()); 
            setParamInfoList(paramMaxList, query.value(4).toString()); 
        }
    }
    query.clear(); 
}

void FArithmeticInfoDialog::setParamInfoList(QList<QString> &paramList, 
                                             const QString &paramString)
{
    for (int i = 0; i < paramInfoNumber; i++)
    {   
        paramList.push_back(paramString.section(';', i, i)); 
    }
}

void FArithmeticInfoDialog::initWindow()
{
    pushButton = new QPushButton(tr("È·¶¨")); 
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    for (QList<QString>::iterator iter = paramDescriptionList.begin(); 
         iter != paramDescriptionList.end(); iter++)
    {
        QLabel *label = new QLabel(*iter); 
        QLineEdit *lineEdit = new QLineEdit(); 
        
        QHBoxLayout *hLayout = new QHBoxLayout; 
        hLayout->addWidget(label); 
        hLayout->addWidget(lineEdit); 
        
        mainLayout->addLayout(hLayout); 
        
        lineEditList.push_back(lineEdit); 
    }
    mainLayout->addWidget(pushButton); 
    
    this->setLayout(mainLayout); 
}

void FArithmeticInfoDialog::setConnect()
{
    connect(pushButton, SIGNAL(clicked()), 
            this, SLOT(onPushButtonSlot())); 
}

void FArithmeticInfoDialog::onPushButtonSlot()
{
    setParamList(); 
    this->accept(); 
}

void FArithmeticInfoDialog::setParamList()
{
    for (QList<QLineEdit *>::iterator iter = lineEditList.begin(); 
         iter != lineEditList.end(); iter++)
    {
        paramList.push_back((*iter)->text()); 
    }
}
