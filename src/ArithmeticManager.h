#ifndef ARITHMETICMANAGER_H
#define ARITHMETICMANAGER_H

#include "ArithmeticBase.h"
#include <QString>
#include <QStringList>
#include <QMultiHash>
#include <QSqlDatabase>
#include <QImage>
#include <QDebug>

class ArithNode
{
public:
    ArithNode(QString name, QString intro,
             int paramNumber, int objNumber,
             QStringList paramDesc, QStringList paramType,
             QStringList paramMin, QStringList paramMax)
        : m_name(name), m_intro(intro),
          m_paramNumber(paramNumber), m_objNumber(objNumber),
          m_paramDesc(paramDesc), m_paramType(paramType),
          m_paramMin(paramMin), m_paramMax(paramMax)
    {}

    QString m_name;
    QString m_intro;
    int m_paramNumber;
    int m_objNumber;
    QStringList m_paramDesc;
    QStringList m_paramType;
    QStringList m_paramMin;
    QStringList m_paramMax;
};

class ArithLeafNode
{
public:
    ArithLeafNode(QString name) : m_name(name) {}
    void addAlgo(ArithNode *algo, int objNumber) { m_ariths.insert(objNumber, algo); }

    QList<ArithNode *> ariths(int objNumber)
    {
        return m_ariths.values(objNumber);
    }

    QString m_name;
    QMultiHash<int, ArithNode *> m_ariths;
};

class ArithRootNode
{
public:
    ArithRootNode(QString name) : m_name(name) {}
    void addLeaf(ArithLeafNode *leaf) { m_leaves << leaf; }
    QString m_name;
    QList<ArithLeafNode *> m_leaves;
};

//*******************************************************
// for user
class Arithmetic
{
public:
    explicit Arithmetic(ArithNode *arith) : p(arith) {}

    QString name() const { return p->m_name; }
    QString intro() const { return p->m_intro; }
    int paramNumber() const { return p->m_paramNumber; }
    int objNumber() const { return p->m_objNumber; }
    QStringList paramDesc() const { return p->m_paramDesc; }
    QStringList paramType() const { return p->m_paramType; }
    QStringList paramMin() const { return p->m_paramMin; }
    QStringList paramMax() const { return p->m_paramMax; }
private:
    ArithNode *p;
};

class ArithLeaf
{
public:
    explicit ArithLeaf(ArithLeafNode *node, int objNumber)
    {
        p = node;
        foreach (ArithNode *arith, node->ariths(objNumber))
        {
            m_ariths << Arithmetic(arith);
        }
    }

    QString name() const { return p->m_name; }
    QList<Arithmetic> ariths() const { return m_ariths; }
private:
    ArithLeafNode *p;
    QList<Arithmetic> m_ariths;
};

class ArithRoot
{
public:
    explicit ArithRoot(ArithRootNode *root, int objNumber)
    {
        p = root;
        foreach (ArithLeafNode *leaf, root->m_leaves)
        {
            m_leaves << ArithLeaf(leaf, objNumber);
        }
    }

    QString name() const { return p->m_name; }
    QList<ArithLeaf> leaves() const { return m_leaves; }
private:
    ArithRootNode *p;
    QList<ArithLeaf> m_leaves;
};

class ArithSequence
{
public:
    enum { Saved = 0, Recent };

    static const QChar Separator;

    ArithSequence() {}
    ArithSequence(QString _name, QStringList _ariths) : name(_name), ariths(_ariths) {}

    QString name;
    QStringList ariths;
};

template <typename T>
class ArithWithParams
{
public:
    ArithWithParams(ArithmeticBase<T> *_arith, QStringList _params)
        : arith(_arith), params(_params) {}

    ArithmeticBase<T> *arith;
    QStringList params;
};

class ArithmeticManager
{
public:
    explicit ArithmeticManager();
    ~ArithmeticManager();

    static ArithRoot tree(int type, int objNumber)
    {
        ArithRootNode *root = roots->value(type);
        return ArithRoot(root, objNumber);
    }
    static Arithmetic arithmeticByName(QString name)
    {
        ArithNode *node = ariths->value(name);
        return Arithmetic(node);
    }
    static ArithmeticBase<QImage> *imageArithInstanceByName(QString name)
    {
        return imageArithInstances->value(name);
    }

    static void saveArithSequence(QString name, QStringList ariths, int type);
    static void renameArithmeticSequence(QString oldName, QString newName);
    static void deleteArithmeticSequence(QString name);
    static void clearRecentArithmeticSequences();

    static QList<ArithSequence> savedArithSequences() { return savedArithSeqs->values(); }
    static QList<ArithSequence> recentArithSequences() { return recentArithSeqs->values(); }

private:
    static void add(ArithmeticBase<QImage> *arith);

    void addRoots(QHash<int, ArithRootNode *> *parent);
    void addLeaves(ArithRootNode *parent, int rootType);
    void addAlgos(ArithLeafNode *parent, int leafType);
    void readArithSeqs();

    static QHash<int, ArithRootNode *> *roots;
    static QHash<QString, ArithNode *> *ariths;

    static QHash<QString, ArithmeticBase<QImage> *> *imageArithInstances;

    static QHash<QString, ArithSequence> *savedArithSeqs;
    static QHash<QString, ArithSequence> *recentArithSeqs;

    QSqlDatabase db;
};

#endif // ARITHMETICMANAGER_H
