#include "Image.h"
#include "FArithmeticDialog.h"
#include "ArithmeticManager.h"

#include <QtGui>

FArithmeticDialog::FArithmeticDialog(QWidget *parent) :
    QDialog(parent)
{
    createWidgets();
    layoutWidgets();
    connectWidgets();

    onArithSelectionChanged();
    onCurrentArithSelectionChanged();
    onCurrentArithSequenceChanged();
}

void FArithmeticDialog::dealData(DataType _type, QList<QList<QString> > _pathGroups)
{
    qDebug() << "in";
    // ���㷨������dealData()����Ҫ�õ�
    type = _type;
    pathGroups = _pathGroups;
    // �жϷ������������Ƿ�һ��
    if (!isGroupSizeEqual(_pathGroups))
    {
        QMessageBox::warning(this, tr("����"), tr("����ϴ�С��һ�£��޷�����"), QMessageBox::Ok);
        return;
    }
    // ��һ�£���ȡ������������
    objNumber = pathGroups.first().size();
    // ��ȡ���� ��������+������ ���㷨��
    ArithRoot root = ArithmeticManager::tree(type, objNumber);

    //
    refreshArithSequences();
    // ��treeWidget���item
    arithTreeWidget->clear();
    arithTreeWidget->setHeaderLabel(root.name());
    foreach (ArithLeaf leaf, root.leaves())
    {
        QList<Arithmetic> ariths = leaf.ariths();
        if (ariths.isEmpty())
        {
            continue;
        }
        QTreeWidgetItem *leafItem = new QTreeWidgetItem(QStringList(leaf.name()));
        arithTreeWidget->addTopLevelItem(leafItem);
        foreach (Arithmetic arith, ariths)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(leafItem, QStringList(arith.name()));
            item->setData(0, Qt::WhatsThisRole, arith.intro());
            item->setData(0, Qt::ToolTipRole, arith.intro());
            item->setData(0, Qt::UserRole, arith.name());
        }
    }
    arithTreeWidget->expandAll();
    arithListWidget->clear();
    onCurrentArithSequenceChanged();
    // ��ʾ��������ǰ
    show();
    activateWindow();
    raise();
}

void FArithmeticDialog::dealData(DataType type, QList<QList<QString> > pathGroups, QList<QString> arithSequence)
{
    if (!isGroupSizeEqual(pathGroups))
    {
        emit hasOutput(tr("������Ԫ�ظ�����һ�£��޷�����"));
        return;
    }
    switch (type)
    {
    case ImageType:
        foreach (QList<QString> group, pathGroups)
        {
            // �ļ���
            QStringList imageNames;
            foreach (QString path, group)
            {
                imageNames << QFileInfo(path).fileName();
            }
            // �㷨(������)
            QList<ArithWithParams<QImage> > ariths;
            foreach (QString arithNameAndParams, arithSequence)
            {
                QString name;
                QStringList paramList;
                if (arithParsed(arithNameAndParams, name, paramList))
                {
                    ArithmeticBase<QImage> *arith = ArithmeticManager::imageArithInstanceByName(name);
                    ariths << ArithWithParams<QImage>(arith, paramList);
                }
                else
                {
                    emit hasOutput(tr("�㷨'") + name + tr("'����������ȥӦ�ø��㷨"));
                }
            }
            // �����߳̽��д���
            Worker *worker = new Worker(group, ariths, imageNames);
            connect(worker, SIGNAL(imageDealt(Image)), this, SIGNAL(hasDealtImage(Image)));
            Thread::run(worker, Thread::Uninterruptible, Thread::Special);
        }
        emit imageDealt(pathGroups, arithSequence);
        break;
    default:
        break;
    }
}

void FArithmeticDialog::searchArith(QString text)
{
    static QColor white(255, 255, 255);
    static QColor parentColor(240, 100, 100);
    static QColor arithColor(255, 50, 50);

    // ����item����ɫ
    QTreeWidgetItemIterator it(arithTreeWidget);
    while (*it)
    {
        (*it)->setBackground(0, white);
        ++it;
    }
    if (text.isEmpty())
    {
        return;
    }
    // ����item����ɫ
    QList<QTreeWidgetItem *> matchedItems = arithTreeWidget->findItems(text, Qt::MatchContains | Qt::MatchRecursive);
    foreach (QTreeWidgetItem *item, matchedItems)
    {
        if (item->childCount() == 0)
        {
            if (item == matchedItems.first())
            {
                arithTreeWidget->setCurrentItem(item);
            }
            item->setBackground(0, arithColor);
            while (item->parent())
            {
                item = item->parent();
                item->setBackground(0, parentColor);
            }
        }
    }
}

void FArithmeticDialog::onArithSelectionChanged()
{
    bool arithSelected = arithTreeWidget->selectedItems().size();
    addButton->setEnabled(arithSelected);
}
void FArithmeticDialog::onCurrentArithSelectionChanged()
{
    bool selected = (arithListWidget->currentRow() != -1);
    upButton->setEnabled(selected);
    downButton->setEnabled(selected);
    deleteButton->setEnabled(selected);
}

void FArithmeticDialog::onCurrentArithSequenceChanged()
{
    bool hasAriths = hasCurrentArithSequence();
    runButton->setEnabled(hasAriths);
    saveButton->setEnabled(hasAriths);
    clearButton->setEnabled(hasAriths);
}

void FArithmeticDialog::onAddButtonClicked()
{
    QTreeWidgetItem *item = arithTreeWidget->currentItem();
    if (item && item->childCount() == 0)    // ѡ�е����㷨item
    {
        QString name = item->data(0, Qt::UserRole).toString();
        QStringList params;
        Arithmetic arith = ArithmeticManager::arithmeticByName(name);
        if (arith.paramNumber())
        {
            ArithmeticParamDialog dialog(arith);
            if (dialog.exec() == QDialog::Rejected)
            {
                return;
            }
            else
            {
                params = dialog.params();
            }
        }
        addArith(arith, params);
    }
}
void FArithmeticDialog::addArith(Arithmetic arith, QStringList params)
{
    QString text = arith.name();
    // ����㷨����
    foreach (QString param, params)
    {
        text += '/' + param;
    }

    new QListWidgetItem(text, arithListWidget);
    onCurrentArithSequenceChanged();    // ��ǰ�㷨�б����仯�����¡����С���ť״̬(enabled/disabled)
}

void FArithmeticDialog::addAriths(QStringList ariths)
{
    arithListWidget->clear();
    foreach (QString arith, ariths)
    {
        new QListWidgetItem(arith, arithListWidget);
    }
    onCurrentArithSequenceChanged();    // ��ǰ�㷨�б����仯�����¡����С���ť״̬(enabled/disabled)
}

void FArithmeticDialog::upArith()
{
    int row = arithListWidget->currentRow();
    if (row > 0)
    {
        arithListWidget->insertItem(row - 1, arithListWidget->takeItem(row));
        arithListWidget->setCurrentRow(row - 1);
    }
}

void FArithmeticDialog::downArith()
{
    int row = arithListWidget->currentRow();
    int count = arithListWidget->count();
    if (row + 1 < count)
    {
        arithListWidget->insertItem(row + 1, arithListWidget->takeItem(row));
        arithListWidget->setCurrentRow(row + 1);
    }
}

void FArithmeticDialog::deleteArith()
{
    delete arithListWidget->currentItem();
    onCurrentArithSequenceChanged();
}

void FArithmeticDialog::clearArith()
{
    arithListWidget->clear();
    onCurrentArithSequenceChanged();
    onCurrentArithSelectionChanged();
}

void FArithmeticDialog::saveArithSequence()
{
    ArithmeticSequenceNameDialog dialog;
    if (dialog.exec() == QDialog::Accepted)
    {
        QString name = dialog.name();
        QStringList ariths = currentArithSequence();
        ArithmeticManager::saveArithSequence(name, ariths, ArithSequence::Saved);

        refreshArithSequences();
    }
}

void FArithmeticDialog::renameSequence()
{
    QListWidgetItem *item = savedListWidget->currentItem();
    QString oldName = item->text();
    ArithmeticSequenceNameDialog dialog(oldName);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString newName = dialog.name();
        item->setText(newName);
        ArithmeticManager::renameArithmeticSequence(oldName, newName);
    }
}

void FArithmeticDialog::deleteSequence()
{
    QListWidgetItem *item = savedListWidget->currentItem();
    QString name = item->text();
    delete item;
    ArithmeticManager::deleteArithmeticSequence(name);
}

void FArithmeticDialog::clearSequences()
{
    recentListWidget->clear();
    ArithmeticManager::clearRecentArithmeticSequences();
}

void FArithmeticDialog::run()
{
    QStringList ariths = currentArithSequence();
    saveRecentArithSequence(ariths);
    dealData(type, pathGroups, ariths);
}

bool FArithmeticDialog::isGroupSizeEqual(QList<Group> groups)
{        
    int size = groups.first().size();
    for (int i = 1; i < groups.size(); ++i)
    {
        if (groups[i].size() != size)
        {
            return false;
        }
    }
    return true;
}

bool FArithmeticDialog::hasCurrentArithSequence()
{
    return arithListWidget->item(0);
}
QStringList FArithmeticDialog::currentArithSequence()
{
    QStringList ariths;
    for (int i = 0; i < arithListWidget->count(); ++i)
    {
        QListWidgetItem *item = arithListWidget->item(i);
        ariths << item->text();
    }
    return ariths;
}

bool FArithmeticDialog::arithParsed(QString nameAndParams, QString &name, QStringList &params)
{
    QStringList parts = nameAndParams.split('/');
    if (parts.isEmpty())
    {
        return false;
    }
    name = parts[0];
    for (int i = 1; i < parts.size(); ++i)
    {
        params << parts[i];
    }
    return true;
}

void FArithmeticDialog::refreshArithSequences()
{
    refreshArithSequences(ArithmeticManager::savedArithSequences(), savedListWidget);
    refreshArithSequences(ArithmeticManager::recentArithSequences(), recentListWidget);
}

void FArithmeticDialog::refreshArithSequences(QList<ArithSequence> seqs, QListWidget *widget)
{
    widget->clear();
    foreach (ArithSequence seq, seqs)
    {
        // ��ø��㷨���пɴ����������
        QString firstArithName = seq.ariths.first().section('/', 0, 0);
        int curObjNumber = ArithmeticManager::arithmeticByName(firstArithName).objNumber();
        if (curObjNumber == objNumber)
        {
            QListWidgetItem *item = new QListWidgetItem(seq.name);
            item->setData(Qt::UserRole, seq.ariths);
            widget->addItem(item);
        }
    }
}

void FArithmeticDialog::saveRecentArithSequence(QStringList ariths)
{
    QString name = QDateTime::currentDateTime().toString();
    ArithmeticManager::saveArithSequence(name, ariths, ArithSequence::Recent);
    refreshArithSequences();
}

void FArithmeticDialog::createWidgets()
{
    // �㷨ѡ��
    arithTreeWidget = new QTreeWidget;
    searchLineEdit = new QLineEdit;
    searchLabel = new QLabel(tr("����"));
    searchLabel->setBuddy(searchLineEdit);
    addButton = new QPushButton(tr("���"));

    // �㷨����ѡ��
    savedListWidget = new SeqListWidget;
    deleteSeqButton = new QPushButton(tr("ɾ��"));
    deleteSeqButton->setDisabled(true);
    renameSeqButton = new QPushButton(tr("������"));
    renameSeqButton->setDisabled(true);
    recentListWidget = new SeqListWidget;
    clearSeqsButton = new QPushButton(tr("���"));

    arithSeqToolBox = new QToolBox;

    //
    arithWidget = new QWidget;
    arithSeqWidget = new QWidget;
    tabWidget = new QTabWidget;

    // ��ǰ�㷨����
    arithListWidget = new QListWidget;
    upButton = new QPushButton(tr("����"));
    downButton = new QPushButton(tr("����"));
    deleteButton = new QPushButton(tr("ɾ��"));
    clearButton = new QPushButton(tr("���"));
    saveButton = new QPushButton(tr("����"));
    runButton = new QPushButton(tr("����"));
}

void FArithmeticDialog::layoutWidgets()
{
    // �㷨ѡ��
    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchLineEdit);

    QHBoxLayout *addButtonLayout = new QHBoxLayout;
    addButtonLayout->addStretch();
    addButtonLayout->addWidget(addButton);

    QVBoxLayout *arithLayout = new QVBoxLayout;
    arithLayout->addLayout(searchLayout);
    arithLayout->addWidget(arithTreeWidget);
    arithLayout->addLayout(addButtonLayout);

    arithWidget->setLayout(arithLayout);

    // �㷨����ѡ��
        // �����
    QHBoxLayout *delRenameButtonLayout = new QHBoxLayout;
    delRenameButtonLayout->addWidget(renameSeqButton);
    delRenameButtonLayout->addWidget(deleteSeqButton);

    QVBoxLayout *savedSeqsLayout = new QVBoxLayout;
    savedSeqsLayout->addWidget(savedListWidget);
    savedSeqsLayout->addLayout(delRenameButtonLayout);

    QWidget *savedSeqsWidget = new QWidget;
    savedSeqsWidget->setLayout(savedSeqsLayout);

        // �����
    QVBoxLayout *recentSeqsLayout = new QVBoxLayout;
    recentSeqsLayout->addWidget(recentListWidget);
    recentSeqsLayout->addWidget(clearSeqsButton);

    QWidget *recentSeqsWidget = new QWidget;
    recentSeqsWidget->setLayout(recentSeqsLayout);

        //
    arithSeqToolBox->addItem(savedSeqsWidget, tr("�����"));
    arithSeqToolBox->addItem(recentSeqsWidget, tr("���ʹ�õ�"));

    QVBoxLayout *arithSeqLayout = new QVBoxLayout;
    arithSeqLayout->addWidget(arithSeqToolBox);
    arithSeqWidget->setLayout(arithSeqLayout);

    // �� �㷨ѡ��+�㷨����ѡ�� �����tabWidget
    tabWidget->addTab(arithWidget, tr("�㷨"));
    tabWidget->addTab(arithSeqWidget, tr("�㷨����"));

    // ��ǰ�㷨����
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(upButton);
    buttonLayout->addWidget(downButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(runButton);

    QHBoxLayout *curArithsLayout = new QHBoxLayout;
    curArithsLayout->addWidget(arithListWidget);
    curArithsLayout->addLayout(buttonLayout);

    //
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(tabWidget);
    layout->addLayout(curArithsLayout);

    setLayout(layout);

    setWindowTitle(tr("����"));
    resize(550, 350);
}

void FArithmeticDialog::connectWidgets()
{
    // ����
    connect(searchLineEdit, SIGNAL(textChanged(QString)), this, SLOT(searchArith(QString)));

    // �㷨ѡ��
    connect(arithTreeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onArithSelectionChanged()));
    connect(arithTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(onAddButtonClicked()));

    // �㷨����
    connect(savedListWidget, SIGNAL(itemClicked(QStringList)), this, SLOT(addAriths(QStringList)));
    connect(savedListWidget, SIGNAL(itemSelectionChanged(bool)), deleteSeqButton, SLOT(setEnabled(bool)));
    connect(savedListWidget, SIGNAL(itemSelectionChanged(bool)), renameSeqButton, SLOT(setEnabled(bool)));

    connect(deleteSeqButton, SIGNAL(clicked()), this, SLOT(deleteSequence()));
    connect(renameSeqButton, SIGNAL(clicked()), this, SLOT(renameSequence()));
    connect(clearSeqsButton, SIGNAL(clicked()), this, SLOT(clearSequences()));

    connect(recentListWidget, SIGNAL(itemClicked(QStringList)), this, SLOT(addAriths(QStringList)));

    // ��ǰ�㷨(����)
    connect(arithListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onCurrentArithSelectionChanged()));
    connect(upButton, SIGNAL(clicked()), this, SLOT(upArith()));
    connect(downButton, SIGNAL(clicked()), this, SLOT(downArith()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteArith()));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearArith()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveArithSequence()));

    //
    connect(addButton, SIGNAL(clicked()), this, SLOT(onAddButtonClicked()));
    connect(runButton, SIGNAL(clicked()), this, SLOT(run()));

}

ArithmeticParamDialog::ArithmeticParamDialog(Arithmetic arith)
{
    QVBoxLayout *layout = new QVBoxLayout;

    int paramNumber = arith.paramNumber();
    QStringList paramDescs = arith.paramDesc();
    QStringList paramTypes = arith.paramType();
    QStringList paramMins = arith.paramMin();
    QStringList paramMaxs = arith.paramMax();
    for (int i = 0; i < paramNumber; ++i)
    {
        QString paramDesc = paramDescs[i];
        QString paramType = paramTypes[i].toLower();
        QString paramMin = paramMins[i];
        QString paramMax = paramMaxs[i];

        // ����label & edit
        QLabel *label = new QLabel(paramDesc);
        QLineEdit *edit = new QLineEdit;
        paramEdits << edit;
        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->addWidget(label);
        hLayout->addWidget(edit);
        layout->addLayout(hLayout);

        // ����validator
        if (paramType == "int")
        {
            QIntValidator *validator = new QIntValidator(paramMin.toInt(), paramMax.toInt(), edit);
            edit->setValidator(validator);
        }
        else if (paramType == "float")
        {
            // Qt�Դ���doubleValidatorò���޷�����0.0-1.0֮������ݡ���2-9��Ȼ�������룬���������critical�Ļ�����Ҫ�Լ�дһ������validator
            QDoubleValidator *validator = new QDoubleValidator(paramMin.toDouble(), paramMax.toDouble(), 10, edit);
            validator->setNotation(QDoubleValidator::StandardNotation);
            edit->setValidator(validator);
        }
    }
    QPushButton *okButton = new QPushButton(tr("ȷ��"));
    QPushButton *cancelButton = new QPushButton(tr("ȡ��"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    setLayout(layout);
}

QStringList ArithmeticParamDialog::params() const
{
    QStringList params;
    foreach (QLineEdit *edit, paramEdits)
    {
        params << edit->text();
    }
    return params;
}

ArithmeticSequenceNameDialog::ArithmeticSequenceNameDialog(QString name)
{
    edit = new QLineEdit(name);
    edit->selectAll();

    QPushButton *okButton = new QPushButton(tr("ȷ��"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    if (this->layout())
    {
        delete this->layout();
    }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(edit);
    layout->addWidget(okButton);
    setLayout(layout);

    setWindowTitle(tr("�������㷨��������"));
}

QString ArithmeticSequenceNameDialog::name() const
{
    return edit->text();
}
