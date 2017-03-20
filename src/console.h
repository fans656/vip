#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QMap>

class FArithmeticDialog; 
class FNotDealImageWidget; 
class FInputOutputWidget; 

class Console : public QObject
{
    Q_OBJECT
public:
    explicit Console(FArithmeticDialog *arithmeticDialog, 
                     FNotDealImageWidget *notDealImageWidget, 
                     FInputOutputWidget *inputOutputWidget, 
                     QObject *parent = 0);
    ~Console(); 
    
signals:
    
public slots:
    void displayOpenImageCmd(const QList<QString> &pathList); 
    void displayDealImageCmd(const QList<QList<QString> > &pathListList, 
                             const QList<QString> &arithList); 
    void slotHasCmd(const QString &cmd); 
    void slotDisplayPickedCmd() const; 
private: 
    FArithmeticDialog *arithmeticDialog; 
    FNotDealImageWidget *notDealImageWidget; 
    FInputOutputWidget *inputOutputWidget; 
    QList<QString> cmd_noError;               //保存所有没有出错的命令
    enum CmdOperation{CmdClear, CmdAppend, CmdNoOperation}; 
    enum Operation{ErrorOperation, NoOperation, OpenImage, 
                   DealImage, ClearInput, ClearOutput, Exit}; 
    enum Keyword{Keyword_open, Keyword_image, Keyword_deal, Keyword_with, Keyword_exit, 
                 Keyword_clear, Keyword_input, Keyword_output, Keyword_help}; 
    struct Statement{Operation operate; 
                     QList<QList<QString> > datapathListList; 
                     QList<QString> arithnameList; }; 
    
    QMap<Keyword, QString> Keywords; 
    
    /***********************************词法分析*************************************/
    const QList<QString> lexicalAnalysis(const QString &cmd) const; 
    inline void pushWord(QString &word, QList<QString> &wordList) const;   
    /***********************************词法分析*************************************/
    
    /***********************************语法分析*************************************/
    const QList<Statement> syntaxAnalysis(const QList<QString> &wordList) const; 
    
    bool groupState(QList<QString> &datapathList, 
                    QList<QString>::const_iterator &iter, 
                    const QList<QString> &wordList) const; 
    bool datapathState(QList<QList<QString> > &datapathListList, 
                       QList<QString>::const_iterator &iter, 
                       const QList<QString> &wordList) const; 
    bool arithnameState(QList<QString> &arithnameList, 
                        QList<QString>::const_iterator &iter, 
                        const QList<QString> &wordList) const; 
    Statement openState(QList<QString>::const_iterator &iter, 
                   const QList<QString> &wordList) const; 
    Statement dealState(QList<QString>::const_iterator &iter, 
                   const QList<QString> &wordList) const; 
    Statement clearState(QList<QString>::const_iterator &iter, 
                   const QList<QString> &wordList) const; 
    Statement exitState(QList<QString>::const_iterator &iter, 
                   const QList<QString> &wordList) const; 
    /***********************************语法分析*************************************/
        
    /***********************************执行命令*************************************/
    CmdOperation execStatement(const QList<Statement> &statementList) const;  
    
    void execOpenImage(const Statement &statement) const; 
    void execDealImage(const Statement &statement) const; 
    void execClearInput(const Statement &statement) const; 
    void execClearOutput(const Statement &statement) const; 
    void execExit(const Statement &statement) const; 
    /***********************************执行命令*************************************/
};

#endif // CONSOLE_H
