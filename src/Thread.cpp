#include "Thread.h"
#include <QDebug>

void ThreadWorker::start()
{
    emit started();
    run();
    emit finished();
    delete this;
}

//****************************************************
ThreadEx::ThreadEx()
    : numberOfWorkers(0)
{

}
void ThreadEx::addWorker(ThreadWorker *worker)
{
    ++numberOfWorkers;  // 这里不能使用worker的started信号——因为加入线程的worker可能会排队
    worker->moveToThread(this); // 将worker移入线程
    connect(worker, SIGNAL(finished()), this, SLOT(removeWorker()));    // worker运行完成后，本线程worker数减一
    //connect(this, SIGNAL(ready()), worker, SLOT(start()));  // 连接通知worker开始运行的信号
    //emit ready();   // 通知worker开始运行
}
//****************************************************
ThreadEx *Thread::m_instance = 0;
int Thread::semaphore = 0;
Thread *Thread::self = 0;

Thread::Thread()
{
    self = this;
}

void Thread::run(ThreadWorker *worker, bool uninterruptible, bool special)
{
    // 向线程中添加worker
    ThreadEx *thread = ThreadPool::selectThread(special);
    connect(thread, SIGNAL(ready()), worker, SLOT(start()));  // 连接通知worker开始运行的信号
    thread->addWorker(worker);
    // 如果本次运行不可被中断，则使用全局信号量
    if (uninterruptible)
    {
        connect(worker, SIGNAL(started()), self, SLOT(acquire()));
        connect(worker, SIGNAL(finished()), self, SLOT(release()));
    }
    thread->notify();
}

//****************************************************
QVector<ThreadEx *> *ThreadPool::threads;
ThreadEx *ThreadPool::specialThread;

ThreadPool::ThreadPool(int numberOfThreads)
{
    // 如果未指定线程数，则尝试使用最佳2倍CPU线程数，如果尝试失败则置线程数为1
    if (numberOfThreads == Auto)
    {
        int idealNumber = QThread::idealThreadCount();
        numberOfThreads = (idealNumber == -1 ? 1 : idealNumber);
    }
    // 创建线程
    threads = new QVector<ThreadEx *>(numberOfThreads);
    for (int i = 0; i < threads->size(); ++i)
    {
        (*threads)[i] = new ThreadEx;
        (*threads)[i]->start();
    }
    specialThread = new ThreadEx;
    specialThread->start();
}

ThreadEx *ThreadPool::selectThread(bool special)
{
    if (special)
    {
        return specialThread;
    }

    int minIndex = 0;
    for (int i = 1; i < threads->size(); ++i)
    {
        if (threads->at(minIndex)->rank() > threads->at(i)->rank())
        {
            minIndex = i;
        }
    }
    return threads->at(minIndex);
}

//*******************  Semaphore **********************

//void Semaphore::acquire(QString id = "")
//{
//}

//void Semaphore::release(QString id = "")
//{

//}

//void Semaphore::available(QString id = "")
//{

//}
