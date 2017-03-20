#include "ArithmeticManager.h"
#include "togray.h"
#include "zhifangtu.h"
#include "xiangjian.h"
#include <QSqlQuery>
#include <QtGui>

QHash<int, ArithRootNode *> *ArithmeticManager::roots = 0;
QHash<QString, ArithNode *> *ArithmeticManager::ariths = 0;
QHash<QString, ArithmeticBase<QImage> *> *ArithmeticManager::imageArithInstances = 0;
QHash<QString, ArithSequence> *ArithmeticManager::savedArithSeqs = 0;
QHash<QString, ArithSequence> *ArithmeticManager::recentArithSeqs = 0;

const QChar ArithSequence::Separator = '|';

ArithmeticManager::ArithmeticManager()
{
    imageArithInstances = new QHash<QString, ArithmeticBase<QImage> *>;

    // 添加算法实例
    add(new ToGray);
    add(new Zhifangtu);
    add(new XiangJian);

    // 连接数据库
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=Arithmetic.mdb");
    if (!db.open())
    {
        QMessageBox::information(0, "Error","Database Failure");
    }
    // 读取算法数据
    roots = new QHash<int, ArithRootNode *>;
    ariths = new QHash<QString, ArithNode *>;
    addRoots(roots);

    // 读取算法序列数据
    savedArithSeqs = new QHash<QString, ArithSequence>;
    recentArithSeqs = new QHash<QString, ArithSequence>;
    readArithSeqs();
}

ArithmeticManager::~ArithmeticManager()
{
    db.close();
}

void ArithmeticManager::saveArithSequence(QString name, QStringList ariths, int type)
{
    // 添加进数据库
    QSqlQuery query;
    query.prepare("insert into ArithmeticSequence (AS_name, AS_arithmetics, AS_type) "
                   "values (:name, :ariths, :type)");
    query.bindValue(0, name);
    query.bindValue(1, ariths.join(ArithSequence::Separator));
    query.bindValue(2, type);
    query.exec();
    // 添加至hash
    switch (type)
    {
    case ArithSequence::Saved:
        savedArithSeqs->insert(name, ArithSequence(name, ariths));
        break;
    case ArithSequence::Recent:
        recentArithSeqs->insert(name, ArithSequence(name, ariths));
        break;
    default:
        break;
    }
}
void ArithmeticManager::renameArithmeticSequence(QString oldName, QString newName)
{
    ArithSequence seq = savedArithSeqs->take(oldName);
    seq.name = newName;
    savedArithSeqs->insert(newName, seq);

    QSqlQuery query;
    query.prepare("update ArithmeticSequence set AS_name = :newName where AS_name = :oldName");
    query.bindValue(0, newName);
    query.bindValue(1, oldName);
    query.exec();
}

void ArithmeticManager::deleteArithmeticSequence(QString name)
{
    savedArithSeqs->remove(name);

    QSqlQuery query;
    query.prepare("delete from ArithmeticSequence where AS_name = :name");
    query.bindValue(0, name);
    query.exec();
}

void ArithmeticManager::clearRecentArithmeticSequences()
{
    recentArithSeqs->clear();

    QSqlQuery query;
    query.prepare("delete from ArithmeticSequence where AS_type = :type");
    query.bindValue(0, ArithSequence::Recent);
    query.exec();
}

void ArithmeticManager::add(ArithmeticBase<QImage> *arith)
{
    QString name = arith->getArithmeticName();
    imageArithInstances->insert(name, arith);
}

void ArithmeticManager::addRoots(QHash<int, ArithRootNode *> *parent)
{
    QSqlQuery query("select ART_id, ART_name from ArithmeticRootType");
    while (query.next())
    {
        int rootType = query.value(0).toInt();
        QString name = query.value(1).toString();

        ArithRootNode *root = new ArithRootNode(name);
        parent->insert(rootType, root);

        addLeaves(root, rootType);
    }
}

void ArithmeticManager::addLeaves(ArithRootNode *parent, int rootType)
{
    QSqlQuery query("select ALT_id, ALT_name from ArithmeticLeafType where ALT_roottype='"
                    + QString::number(rootType) + "'");
    while (query.next())
    {
        int leafType = query.value(0).toInt();
        QString name = query.value(1).toString();

        ArithLeafNode *leaf = new ArithLeafNode(name);
        parent->addLeaf(leaf);

        addAlgos(leaf, leafType);
    }
}

void ArithmeticManager::addAlgos(ArithLeafNode *parent, int leafType)
{
    QSqlQuery query("select A_name, A_introduce, A_paramNumber, A_objectNumber, A_paramDescription, "
                    "A_paramType, A_paramMin, A_paramMax from Arithmetic where A_leaftype='"
                    + QString::number(leafType) + "'");
    while (query.next())
    {
        QString name = query.value(0).toString();
        QString intro = query.value(1).toString();
        int paramNumber = query.value(2).toInt();
        int objNumber = query.value(3).toInt();
        QStringList paramDesc = query.value(4).toString().split(';', QString::SkipEmptyParts);
        QStringList paramType = query.value(5).toString().split(';', QString::SkipEmptyParts);
        QStringList paramMin = query.value(6).toString().split(';', QString::SkipEmptyParts);
        QStringList paramMax = query.value(7).toString().split(';', QString::SkipEmptyParts);

        ArithNode *algo = new ArithNode(name, intro, paramNumber, objNumber,
                                      paramDesc, paramType, paramMin,paramMax);
        parent->addAlgo(algo, objNumber);
        ariths->insert(name, algo);
    }
}

void ArithmeticManager::readArithSeqs()
{
    QSqlQuery query("select AS_name, AS_arithmetics, AS_type from ArithmeticSequence");
    while (query.next())
    {
        QString name = query.value(0).toString();
        QStringList ariths = query.value(1).toString().split(ArithSequence::Separator);
        int type = query.value(2).toInt();
        switch (type)
        {
        case ArithSequence::Saved:
            savedArithSeqs->insert(name, ArithSequence(name, ariths));
            break;
        case ArithSequence::Recent:
            recentArithSeqs->insert(name, ArithSequence(name, ariths));
            break;
        default:
            break;
        }
    }
}
