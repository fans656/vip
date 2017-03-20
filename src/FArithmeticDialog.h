#pragma once

#include "Image.h"
#include "Thread.h"
#include "ArithmeticManager.h"
#include "stdafx.h"
#include <QDialog>
#include <QList>
#include <QTreeWidgetItem>
#include <QListWidgetItem>

class QTreeWidget;
class QLabel;
class QLineEdit;
class QPushButton;
class QListWidget;
class QToolBox;
class QTabWidget;
class QWidget;

class FArithmeticDialog : public QDialog
{
    Q_OBJECT
//*********************************** 接口 ****************************************
public:
    FArithmeticDialog(QWidget *parent = 0);

public slots:
    void dealData(DataType _type, QList<QList<QString> > _pathGroups);
    void dealData(DataType _type, QList<QList<QString> > _pathGroups, QList<QString> arithSequence);

signals:
    void hasOutput(QString message);
    void hasDealtImage(Image img);
    void imageDealt(QList<QList<QString> > pathListList, QList<QString> arithList);

//*********************************** 实现 ****************************************
private slots:
    void searchArith(QString text);

    void onArithSelectionChanged();
    void onCurrentArithSelectionChanged();
    void onCurrentArithSequenceChanged();

    void onAddButtonClicked();
    void addArith(Arithmetic arith, QStringList params);
    void addAriths(QStringList ariths);

    void upArith();
    void downArith();
    void deleteArith();
    void clearArith();
    void saveArithSequence();

    void renameSequence();
    void deleteSequence();
    void clearSequences();

    void run();

private:
    typedef QList<QString> Group;
    class Worker;
    class SeqListWidget;

    bool isGroupSizeEqual(QList<Group> groups);     // 判断dealData传进的数据组内元素个数是否相同
    bool hasCurrentArithSequence();                 // 判断当前算法序列列表是否不为空
    QStringList currentArithSequence();
    bool arithParsed(QString nameAndParams, QString &name, QStringList &params);
    void refreshArithSequences();
    void refreshArithSequences(QList<ArithSequence> seqs, QListWidget *widget);
    void saveRecentArithSequence(QStringList ariths);

    void createWidgets();
    void layoutWidgets();
    void connectWidgets();

    //
    DataType type;
    QList<QList<QString> > pathGroups;
    int objNumber;      // 启动本次dialog时数据来源的objNumber，刷新算法序列(保存的/最近的)时要用到
    // 算法选择
    QLabel *searchLabel;
    QLineEdit *searchLineEdit;
    QTreeWidget *arithTreeWidget;
    QPushButton *addButton;
    // 算法序列选择
    QToolBox *arithSeqToolBox;

    SeqListWidget *savedListWidget;
    QPushButton *deleteSeqButton;
    QPushButton *renameSeqButton;

    SeqListWidget *recentListWidget;
    QPushButton *clearSeqsButton;
    //
    QTabWidget *tabWidget;  // 用来tab算法选择+算法序列选择
    QWidget *arithWidget;
    QWidget *arithSeqWidget;
    // 当前算法序列
    QListWidget *arithListWidget;
    QPushButton *upButton;
    QPushButton *downButton;
    QPushButton *deleteButton;
    QPushButton *clearButton;
    QPushButton *saveButton;
    QPushButton *runButton;
};

// 实际处理时的线程worker
class FArithmeticDialog::Worker : public ThreadWorker
{
    Q_OBJECT

public:
    Worker(QList<QString> _paths, QList<ArithWithParams<QImage> > _ariths, QStringList _imageNames)
        : paths(_paths), ariths(_ariths), imageNames(_imageNames) {}

    void run()
    {
        QList<QImage> dataList;
        foreach (QString path, paths)
        {
            dataList << QImage(path);
        }
        foreach (ArithWithParams<QImage> arithWithParams, ariths)
        {
            ArithmeticBase<QImage> *arith = arithWithParams.arith;
            QStringList params = arithWithParams.params;
            arith->setParam(dataList, params);
            dataList = arith->getFinalDataList();
        }
        for (int i = 0; i < dataList.size(); ++i)
        {
            Image img;
            img.image = dataList[i];
            img.imageName = imageNames[i];
            emit imageDealt(img);
        }
    }

signals:
    void imageDealt(Image img);

private:
    QList<QString> paths;
    QList<ArithWithParams<QImage> > ariths;
    QStringList imageNames;
};

// 用于“保存的/最近的算法序列”
//      itemClicked时报告算法序列中各算法的名称(带参数)
//      itemSelectionChanged时报告是否有选中的item
class FArithmeticDialog::SeqListWidget : public QListWidget
{
    Q_OBJECT

public:
    SeqListWidget()
    {
        connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onItemClicked(QListWidgetItem*)));
        connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(onItemSelectionChanged()));
        connect(this, SIGNAL(currentRowChanged(int)), this, SLOT(onCurrentRowChanged(int)));
    }

signals:
    void itemClicked(QStringList ariths);
    void itemSelectionChanged(bool hasSelection);

private slots:
    void onItemClicked(QListWidgetItem *item)
    {
        QStringList ariths = item->data(Qt::UserRole).toStringList();
        emit itemClicked(ariths);
    }
    void onItemSelectionChanged()
    {
        QListWidgetItem *item = currentItem();
        emit itemSelectionChanged(bool(item));
    }
    void onCurrentRowChanged(int row)
    {
        emit itemSelectionChanged(row != -1);
    }
};

// 用于输入算法参数（对于有参数的算法）
class ArithmeticParamDialog : public QDialog
{
    Q_OBJECT

public:
    ArithmeticParamDialog(Arithmetic arith);
    QStringList params() const;

private:
    void init(QString name);

    QList<QLineEdit *> paramEdits;
};

// 用于输入要保存/修改的算法序列名称
class ArithmeticSequenceNameDialog : public QDialog
{
    Q_OBJECT

public:
    ArithmeticSequenceNameDialog(QString name = "");
    QString name() const;

private:
    QLineEdit *edit;
};
