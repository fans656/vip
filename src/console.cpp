#include "console.h"
#include "FArithmeticDialog.h"
#include "FNotDealImageWidget.h"
#include "FInputOutputWidget.h"
#include "FInputWidget.h"
#include <QDebug>

Console::Console(FArithmeticDialog *arithmeticDialog, 
                 FNotDealImageWidget *notDealImageWidget,
                 FInputOutputWidget *inputOutputWidget,
                 QObject *parent) :
    QObject(parent),
    arithmeticDialog(arithmeticDialog),
    notDealImageWidget(notDealImageWidget),
    inputOutputWidget(inputOutputWidget)
  
{
    Keywords[Keyword_open] = "open";
    Keywords[Keyword_image] = "image";
    Keywords[Keyword_deal] = "deal";
    Keywords[Keyword_with] = "with";
    Keywords[Keyword_exit] = "exit";
    Keywords[Keyword_clear] = "clear";
    Keywords[Keyword_input] = "input";
    Keywords[Keyword_output] = "output";
    Keywords[Keyword_help] = "/?";
    
    connect(inputOutputWidget, SIGNAL(hasCmd(QString)),
            this, SLOT(slotHasCmd(QString)));
    connect(inputOutputWidget, SIGNAL(pickcmdTriggered()),
            this, SLOT(slotDisplayPickedCmd()));
}

Console::~Console()
{
}

void Console::slotDisplayPickedCmd() const
{
    QTextEdit *textEdit = new QTextEdit;
    foreach (QString cmd, cmd_noError)
    {
        textEdit->append(cmd);
    }
    textEdit->show();
}

void Console::slotHasCmd(const QString &cmd)
{
    const QList<QString> &wordList = lexicalAnalysis(cmd); 
    const QList<Console::Statement> &statementList = syntaxAnalysis(wordList);
    switch (execStatement(statementList))
    {
    case CmdNoOperation: break;
    case CmdAppend: cmd_noError.push_back(cmd); break;
    case CmdClear: cmd_noError.clear(); break;
    default: break;
    }
}

///将关键字加入词法分析结果列表
inline void 
Console::pushWord(QString &word, QList<QString> &wordList) const
{
    if (!word.isEmpty())
    {
        wordList.push_back(word);
        word.clear();
    }
}

///词法分析
const QList<QString> 
Console::lexicalAnalysis(const QString &cmd) const
{
    bool inBracket = false;   //当前字符是否在括号中，在括号中不会把路径识别为关键字
    bool inQuotation = false; //当前字符是否在引号中，在引号中不会去掉空格且会把括号识别为路径
    QList<QString> wordList;
    QString word;
    
    for (QString::const_iterator iter = cmd.constBegin();
         iter != cmd.constEnd(); iter++)
    {
        if ((*iter) == ' ')
        {
            if (inQuotation)
            {
                word.push_back(*iter);
            }
            else
            {
                pushWord(word, wordList);
            }
        }
        else if ((*iter) == '\t' || (*iter) == '\n' || (*iter) == '\r')
        {
            pushWord(word, wordList);
        }
        else if (!inQuotation && (*iter) == '(')
        {
            pushWord(word, wordList);
            inBracket = true;
            wordList.push_back(*iter);
        }
        else if (!inQuotation && (*iter) == ')')
        {
            pushWord(word, wordList);
            inBracket = false;
            wordList.push_back(*iter);
        }
        else if ((*iter) == '<' || (*iter) == '>' || (*iter) == ',')
        {
            pushWord(word, wordList);
            wordList.push_back(*iter);
        }
        else if ((*iter) == '"')
        {
            inQuotation = !inQuotation;
        }
        else
        {
            word.push_back(*iter);
            if (!inBracket)
            {
                foreach(const QString &keyword, Keywords)
                {
                    if (word.compare(keyword, Qt::CaseInsensitive) == 0)
                    {
                        pushWord(word, wordList);
                        break; 
                    }
                }
            }
        }
    }
    pushWord(word, wordList);
    qDebug() << "词法分析:" << wordList;
    return wordList;
}

///尖括号中的数据路径
bool Console::groupState(QList<QString> &datapathList, 
                         QList<QString>::const_iterator &iter,
                         const QList<QString> &wordList) const
{
    if (++iter == wordList.constEnd())
    {
        return false;
    }
    else if ((*iter) == ">")
    {
        return true;
    }
    else
    {
        while (iter != wordList.constEnd())
        {
            datapathList.push_back(*iter);
            if (++iter == wordList.constEnd())
            {
                return false;
            }
            else if ((*iter) == ",")
            {
                iter++;
                continue;
            }
            else if ((*iter) == ">")
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        return false;
    }
}

///圆括号中的数据路径
bool Console::datapathState(QList<QList<QString> > &datapathListList, 
                            QList<QString>::const_iterator &iter,
                            const QList<QString> &wordList) const
{
    if (++iter == wordList.constEnd())
    {
        return false;
    }
    else if ((*iter) == ")")
    {
        iter++;
        return true;
    }
    else
    {
        while (iter != wordList.constEnd())
        {
            QList<QString> datapathList;
            
            if ((*iter) == "<")
            {
                if (!groupState(datapathList, iter, wordList))
                {
                    return false;
                }
            }
            else
            {
                datapathList.push_back(*iter);
            }
            datapathListList.push_back(datapathList);
            
            if (++iter == wordList.constEnd())
            {
                return false;
            }
            else if ((*iter) == ",")
            {
                iter++;
                continue;
            }
            else if ((*iter) == ")")
            {
                iter++;
                return true;
            }
            else
            {
                return false;
            }
        }
        return false;
    }
}

///圆括号中的算法名称
bool Console::arithnameState(QList<QString> &arithnameList, 
                             QList<QString>::const_iterator &iter,
                             const QList<QString> &wordList) const
{
    if (++iter == wordList.constEnd())
    {
        return false;
    }
    else if ((*iter) == ")")
    {
        iter++;
        return true;
    }
    else
    {
        while (iter != wordList.constEnd())
        {
            arithnameList.push_back(*iter);
            
            if (++iter == wordList.constEnd())
            {
                return false;
            }
            else if ((*iter) == ",")
            {
                iter++;
                continue;
            }
            else if ((*iter) == ")")
            {
                iter++;
                return true;
            }
            else
            {
                return false;
            }
        }
        return false;
    }
}

///open语句
Console::Statement 
Console::openState(QList<QString>::const_iterator &iter, 
                   const QList<QString> &wordList) const
{
    Statement statement;
    if (++iter == wordList.constEnd())
    {
        inputOutputWidget->addInputInfo("incomplete or error command, add /? for help");
        statement.operate = ErrorOperation;
        return statement;
    }
    else if ((*iter) == Keywords[Keyword_help])
    {
        inputOutputWidget->addInputInfo("open datatype (\"path2\", \"path2\", ...)");
        statement.operate = NoOperation;
        iter++;
        return statement;
    }
    else if ((*iter) == Keywords[Keyword_image])
    {
        if (++iter == wordList.constEnd())
        {
            inputOutputWidget->addInputInfo("incomplete or error command, add /? for help");
            statement.operate = ErrorOperation;
            return statement;
        }
        else if ((*iter) == Keywords[Keyword_help])
        {
            inputOutputWidget->addInputInfo("open image (\"path2\", \"path2\", ...)");
            statement.operate = NoOperation;
            iter++;
            return statement;
        }
        else if ((*iter) == "(")
        {
            if (!datapathState(statement.datapathListList, iter, wordList) )
            {
                statement.operate = ErrorOperation;
                inputOutputWidget->addInputInfo("incomplete or error command, add /? for help");
                return statement;
            }
        }
        else
        {
            //在此添加其他有关open image的命令
            inputOutputWidget->addInputInfo("incomplete or error command, add /? for help");
            statement.operate = ErrorOperation;
            iter++;
            return statement;
        }
        statement.operate = OpenImage;
    }
    else
    {
        //在此添加其他有关open的命令
        inputOutputWidget->addInputInfo("incomplete or error command, add /? for help");
        statement.operate = ErrorOperation;
        iter++;
        return statement;
    }
    return statement;
}

///deal语句
Console::Statement
Console::dealState(QList<QString>::const_iterator &iter, 
                   const QList<QString> &wordList) const
{
    Statement statement;
    if (++iter == wordList.constEnd())
    {
        inputOutputWidget->addInputInfo("incomplete or error command, add /? for help");
        statement.operate = ErrorOperation;
        return statement;
    }
    else if ((*iter) == Keywords[Keyword_help])
    {
        inputOutputWidget->addInputInfo(
                    "deal datatype (\"path2\", \"path2\", ...) with (arithname1, arithname2, ...)");
        statement.operate = NoOperation;
        iter++;
        return statement;
    }
    else if ((*iter) == Keywords[Keyword_image])
    {
        if (++iter == wordList.constEnd())
        {
            inputOutputWidget->addInputInfo("incomplete or error command, add /? for help");
            statement.operate = ErrorOperation;
            return statement;
        }
        else if ((*iter) == Keywords[Keyword_help])
        {
            inputOutputWidget->addInputInfo(
                        "deal image (<\"path1\", \"path2\">, <path3>, ...) with (arithname1, arithname2, ...)");
            statement.operate = NoOperation;
            iter++;
            return statement;
        }
        else if ((*iter) == "(")
        {
            if (!datapathState(statement.datapathListList, iter, wordList))
            {
                statement.operate = ErrorOperation;
                inputOutputWidget->addInputInfo("incomplete or error command, add /? for help");
                return statement;
            }
        }
        else
        {
            //在此添加其他有关deal image的命令
            inputOutputWidget->addInputInfo("incomplete or error command, add /? for help");
            statement.operate = ErrorOperation;
            iter++;
            return statement;
        }
        
        if (iter == wordList.constEnd())
        {
            statement.operate = ErrorOperation;
            inputOutputWidget->addInputInfo("incomplete or error command, add /? for help");
            return statement;
        }
        else if ((*iter) == Keywords[Keyword_with])
        {
            if (++iter == wordList.constEnd())
            {
                inputOutputWidget->addInputInfo("incomplete or error command, add /? for help");
                statement.operate = ErrorOperation;
                return statement;
            }
            else if ((*iter) == "(")
            {
                if (!arithnameState(statement.arithnameList, iter, wordList))
                {
                    statement.operate = ErrorOperation;
                    inputOutputWidget->addInputInfo("incomplete or error command, add /? for help");
                    return statement;
                }
            }
            else
            {
                //在此添加其他有关deal image () with的命令
                inputOutputWidget->addInputInfo("incomplete or error command, add /? for help");
                statement.operate = ErrorOperation;
                iter++;
                return statement;
            }
        }
        statement.operate = DealImage;
    }
    else
    {
        //在此添加其他有关deal的命令
        inputOutputWidget->addInputInfo("incomplete or error command, add /? for help");
        statement.operate = ErrorOperation;
        iter++;
        return statement;
    }
    return statement;
}

///clear语句
Console::Statement
Console::clearState(QList<QString>::const_iterator &iter, 
                    const QList<QString> &wordList) const
{
    Statement statement;
    if (++iter == wordList.constEnd())
    {
        inputOutputWidget->addInputInfo("incomplete or error command, add /? for help");
        statement.operate = ErrorOperation;
        return statement;
    }
    else if ((*iter) == Keywords[Keyword_help])
    {
        inputOutputWidget->addInputInfo("clear input/output");
        statement.operate = ErrorOperation;
        iter++;
        return statement;
    }
    else if ((*iter) == Keywords[Keyword_input])
    {
        statement.operate = ClearInput;
        iter++;
        return statement;
    }
    else if ((*iter) == Keywords[Keyword_output])
    {
        statement.operate = ClearOutput;
        iter++;
        return statement;
    }
    else
    {
        //在此添加其他有关clear命令
        inputOutputWidget->addInputInfo("incomplete or error command, add /? for help");
        statement.operate = ErrorOperation;
        iter++;
        return statement;
    }
}

///exit语句
Console::Statement
Console::exitState(QList<QString>::const_iterator &iter, 
                   const QList<QString> &wordList) const
{
    Statement statement;
    statement.operate = Exit;
    iter++;
    return statement;
}

///语法分析
const QList<Console::Statement> 
Console::syntaxAnalysis(const QList<QString> &wordList) const
{
    QList<Statement> statementList;
    QList<QString>::const_iterator iter = wordList.constBegin();
    
    while(iter != wordList.constEnd())
    {
        Statement statement;
        if (iter->compare(Keywords[Keyword_open], Qt::CaseInsensitive) == 0)
        {
            statement = openState(iter, wordList);
            statementList.push_back(statement);
        }
        else if (iter->compare(Keywords[Keyword_deal], Qt::CaseInsensitive) == 0)
        {
            statement = dealState(iter, wordList);
            statementList.push_back(statement);
        }
        else if (iter->compare(Keywords[Keyword_clear], Qt::CaseInsensitive) == 0)
        {
            statement = clearState(iter, wordList);
            statementList.push_back(statement);
        }
        else if (iter->compare(Keywords[Keyword_exit], Qt::CaseInsensitive) == 0)
        {
            statement = exitState(iter, wordList);
            statementList.push_back(statement);
        }
        else
        {
            inputOutputWidget->addInputInfo("command error!");
            break;
        }
        
        /*
        qDebug() << "语法分析" << statement.operate;
        for (QList<QList<QString> >::const_iterator iter = statement.datapathListList.constBegin();
             iter != statement.datapathListList.constEnd(); iter++)
        {
            qDebug() << *iter;
        }
        */
    }
    
    return statementList;
}

void Console::execOpenImage(const Statement &statement) const
{
    QList<QString> pathList;
    for (QList<QList<QString> >::const_iterator iter
         = statement.datapathListList.constBegin();
         iter != statement.datapathListList.constEnd(); iter++)
    {
        pathList.append(*iter);
    }
    notDealImageWidget->openImage(pathList);
}

void Console::execDealImage(const Statement &statement) const
{
    arithmeticDialog->dealData(ImageType,
                               statement.datapathListList,
                               statement.arithnameList);
}

void Console::execClearInput(const Statement &statement) const
{
    inputOutputWidget->clearInput();
}

void Console::execClearOutput(const Statement &statement) const
{
    inputOutputWidget->clearOutput();
}

void Console::execExit(const Statement &statement) const
{
    QApplication::exit(0);
}

Console::CmdOperation 
Console::execStatement(const QList<Statement> &statementList) const
{
    CmdOperation cmdOperation = CmdAppend;
    for (QList<Statement>::const_iterator iter = statementList.constBegin();
         iter != statementList.constEnd(); iter++)
    {
        switch(iter->operate)
        {
        case NoOperation: break;
        case ErrorOperation: cmdOperation = CmdNoOperation; break;
        case OpenImage: execOpenImage(*iter); break;
        case DealImage: execDealImage(*iter); break;
        case ClearInput: execClearInput(*iter); cmdOperation = CmdClear; break;
        case ClearOutput: execClearOutput(*iter); break;
        case Exit: execExit(*iter); break;
        default: break;
        }
    }
    return cmdOperation;
}

void Console::displayOpenImageCmd(const QList<QString> &pathList)
{
    QString cmd;
    QList<QString>::const_iterator iter = pathList.constBegin();
    cmd = "open image (\"" + *iter;
    while (++iter != pathList.constEnd())
    {
        cmd += "\", \"" + *iter;
    }
    cmd += "\")";
    inputOutputWidget->addInputCmd(cmd);
    cmd_noError.push_back(cmd);
}

void Console::displayDealImageCmd(const QList<QList<QString> > &pathListList, 
                                  const QList<QString> &arithList)
{
    QString cmd;
    QList<QList<QString> >::const_iterator iter1 = pathListList.constBegin();
    cmd = "deal image (";

    QList<QString>::const_iterator iterChild = iter1->constBegin();
    cmd += "<\"" + *iterChild;
    while (++iterChild != iter1->constEnd())
    {
        cmd += "\", \"" + *iterChild;
    }
    cmd += "\">";

    while (++iter1 != pathListList.constEnd())
    {
        QList<QString>::const_iterator iterChild = iter1->constBegin();
        cmd += ", <\"" + *iterChild;
        while (++iterChild != iter1->constEnd())
        {
            cmd += "\", \"" + *iterChild;
        }
        cmd += "\">";
    }

    QList<QString>::const_iterator iter2 = arithList.constBegin();
    cmd += ") with (\"" + *iter2;
    while (++iter2 != arithList.constEnd())
    {
        cmd += "\", \"" + *iter2;
    }
    cmd += "\")";

    inputOutputWidget->addInputCmd(cmd);
    cmd_noError.push_back(cmd);

}
