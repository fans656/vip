/*************************************************

  Author: 刘文龙      Version:1.2.1        Date:2012.12.23

  Description:
    提供线程支持

  Interface:
    1. public:
       Thread::run(ThreadWorker *worker, bool uninterruptible = false, bool specialThread = false)

    2. public slots:
       无

    3. signal:
       无

  History:
    1. Date: 2012.12.29
       Author: 刘文龙
       Modification:
            加入线程池，对外接口向下兼容
            Thread::run()新添两个参数：
                uninterruptible 为true时标记此worker的工作不可中断，关闭程序时会提示用户有工作正在进行
                specialThread 为true时运行在一个特定的线程中，避免排队等待
            新添Thread::isBusy()，用于查看是否有工作正在进行
    2. ...

*************************************************/
/*
 * 说明：
 *      将较耗时的操作（如加载、缩放图片）放到GUI之外的线程执行
 *
 * 方法：
 *      创建一个继承自ThreadWorker的类，将处理代码放在重写的run()函数内
 *      处理结果经自定义的signal传回GUI线程
 *      run()所需要的参数可通过 [构造函数 或 自定义函数] 传递
 *      如：
 *
 *      class Worker : public ThreadWorker
 *      {
 *          Q_OBJECT
 *
 *      public:
 *          Worker(QString _path)
 *              : path(_path)
 *          {
 *          }
 *
 *          void run()
 *          {
 *              QImage img = QImage(path).scaled(size);
 *              emit imageScaled(img);
 *          }
 *
 *      signals:
 *          void imageScaled(QImage result);
 *
 *      private:
 *          QString path;
 *          QSize size;
 *      }
 *
 *      在GUI控件中添加接收处理结果的slot，如：
 *      FNotDealtWidget : public QWidget
 *      {
 *      private slots:
 *          void setIcon(QImage image);
 *      }
 *
 *      new一个Worker实例，连接signal/slot，调用Thread::run(ThreadWorker *)
 *      如：
 *      void openImage(QList<QString> paths)
 *      {
 *          foreach (QString path, paths)
 *          {
 *              Worker *w = new Worker(path);
 *              connect(w, SIGNAL(imageScaled(QImage)), this, SLOT(setIcon(QImage)));
 *              Thread::run(w);
 *          }
 *      }
 *
 * 注意：
 *      1. 线程之间的交互只能通过signal/slot机制进行
 *      2. 线程间传递参数时不要使用引用
 *              void imageScaled(QImage img)            // 正确
 *              void imageScaled(const QImage &img)     // 危险
 *      3. GUI相关的Qt类不能用在主线程之外，已知的有：
 *              QPixmap
 *              所有QWidget及其子类
 *      4. 如果线程间所传递参数的类不是继承自QObject，则需要使用qRegisterMeta()在main()中进行注册
 *
 * PS：
 *      建议Worker写成私有类，避免命名冲突
 *      具体实例可参见NotDealImageWidget、DealtImageWidget等源文件
 *
 */

#ifndef THREAD_H
#define THREAD_H

#include <QObject>
#include <QVector>
#include <QThread>

//****************************************************
class ThreadWorker : public QObject
{
    Q_OBJECT

public:
    virtual void run() {}

public slots:
    void start();

signals:
    void started();
    void finished();
};

//****************************************************
class ThreadEx : public QThread
{
    Q_OBJECT

public:
    ThreadEx();
    void addWorker(ThreadWorker *worker);
    void notify() { emit ready(); }
    int rank() const { return numberOfWorkers; }

signals:
    void ready();

private slots:
    void removeWorker() { --numberOfWorkers; }

private:
    int numberOfWorkers;
};

//****************************************************
class Thread : public QObject
{
    Q_OBJECT

public:
    static const bool Uninterruptible = true;
    static const bool Interruptible = false;
    static const bool Special = true;

    Thread();

    static void run(ThreadWorker *worker, bool uninterruptible = false, bool special = false);
    static bool isBusy() { return semaphore < 0; }

private slots:
    void acquire() { --semaphore; }
    void release() { ++semaphore; }

private:
    static Thread *self;    // 用于无法中断worker的signal/slot连接

    static ThreadEx *m_instance;
    static int semaphore;
};

//****************************************************
class ThreadPool
{
public:
    enum { Auto = 0 };

    ThreadPool(int numberOfThreads = Auto);
    static ThreadEx *selectThread(bool special = false);

private:
    static QVector<ThreadEx *> *threads;
    static ThreadEx *specialThread;
};

//class Semaphore
//{
//public:
//    static void acquire(QString id = "");
//    static void release(QString id = "");
//    static void available(QString id = "");

//private:
//    static QHash<QString, int> semaphores;
//};

#endif // THREAD_H
