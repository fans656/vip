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
//*********************************** �ӿ� ****************************************
public:
    FArithmeticDialog(QWidget *parent = 0);

public slots:
    void dealData(DataType _type, QList<QList<QString> > _pathGroups);
    void dealData(DataType _type, QList<QList<QString> > _pathGroups, QList<QString> arithSequence);

signals:
    void hasOutput(QString message);
    void hasDealtImage(Image img);
    void imageDealt(QList<QList<QString> > pathListList, QList<QString> arithList);

//*********************************** ʵ�� ****************************************
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

    bool isGroupSizeEqual(QList<Group> groups);     // �ж�dealData��������������Ԫ�ظ����Ƿ���ͬ
    bool hasCurrentArithSequence();                 // �жϵ�ǰ�㷨�����б��Ƿ�Ϊ��
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
    int objNumber;      // ��������dialogʱ������Դ��objNumber��ˢ���㷨����(�����/�����)ʱҪ�õ�
    // �㷨ѡ��
    QLabel *searchLabel;
    QLineEdit *searchLineEdit;
    QTreeWidget *arithTreeWidget;
    QPushButton *addButton;
    // �㷨����ѡ��
    QToolBox *arithSeqToolBox;

    SeqListWidget *savedListWidget;
    QPushButton *deleteSeqButton;
    QPushButton *renameSeqButton;

    SeqListWidget *recentListWidget;
    QPushButton *clearSeqsButton;
    //
    QTabWidget *tabWidget;  // ����tab�㷨ѡ��+�㷨����ѡ��
    QWidget *arithWidget;
    QWidget *arithSeqWidget;
    // ��ǰ�㷨����
    QListWidget *arithListWidget;
    QPushButton *upButton;
    QPushButton *downButton;
    QPushButton *deleteButton;
    QPushButton *clearButton;
    QPushButton *saveButton;
    QPushButton *runButton;
};

// ʵ�ʴ���ʱ���߳�worker
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

// ���ڡ������/������㷨���С�
//      itemClickedʱ�����㷨�����и��㷨������(������)
//      itemSelectionChangedʱ�����Ƿ���ѡ�е�item
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

// ���������㷨�����������в������㷨��
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

// ��������Ҫ����/�޸ĵ��㷨��������
class ArithmeticSequenceNameDialog : public QDialog
{
    Q_OBJECT

public:
    ArithmeticSequenceNameDialog(QString name = "");
    QString name() const;

private:
    QLineEdit *edit;
};
