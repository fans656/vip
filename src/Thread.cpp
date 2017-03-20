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
    ++numberOfWorkers;  // ���ﲻ��ʹ��worker��started�źš�����Ϊ�����̵߳�worker���ܻ��Ŷ�
    worker->moveToThread(this); // ��worker�����߳�
    connect(worker, SIGNAL(finished()), this, SLOT(removeWorker()));    // worker������ɺ󣬱��߳�worker����һ
    //connect(this, SIGNAL(ready()), worker, SLOT(start()));  // ����֪ͨworker��ʼ���е��ź�
    //emit ready();   // ֪ͨworker��ʼ����
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
    // ���߳������worker
    ThreadEx *thread = ThreadPool::selectThread(special);
    connect(thread, SIGNAL(ready()), worker, SLOT(start()));  // ����֪ͨworker��ʼ���е��ź�
    thread->addWorker(worker);
    // ����������в��ɱ��жϣ���ʹ��ȫ���ź���
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
    // ���δָ���߳���������ʹ�����2��CPU�߳������������ʧ�������߳���Ϊ1
    if (numberOfThreads == Auto)
    {
        int idealNumber = QThread::idealThreadCount();
        numberOfThreads = (idealNumber == -1 ? 1 : idealNumber);
    }
    // �����߳�
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
