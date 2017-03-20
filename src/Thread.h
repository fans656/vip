/*************************************************

  Author: ������      Version:1.2.1        Date:2012.12.23

  Description:
    �ṩ�߳�֧��

  Interface:
    1. public:
       Thread::run(ThreadWorker *worker, bool uninterruptible = false, bool specialThread = false)

    2. public slots:
       ��

    3. signal:
       ��

  History:
    1. Date: 2012.12.29
       Author: ������
       Modification:
            �����̳߳أ�����ӿ����¼���
            Thread::run()��������������
                uninterruptible Ϊtrueʱ��Ǵ�worker�Ĺ��������жϣ��رճ���ʱ����ʾ�û��й������ڽ���
                specialThread Ϊtrueʱ������һ���ض����߳��У������Ŷӵȴ�
            ����Thread::isBusy()�����ڲ鿴�Ƿ��й������ڽ���
    2. ...

*************************************************/
/*
 * ˵����
 *      ���Ϻ�ʱ�Ĳ���������ء�����ͼƬ���ŵ�GUI֮����߳�ִ��
 *
 * ������
 *      ����һ���̳���ThreadWorker���࣬��������������д��run()������
 *      ���������Զ����signal����GUI�߳�
 *      run()����Ҫ�Ĳ�����ͨ�� [���캯�� �� �Զ��庯��] ����
 *      �磺
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
 *      ��GUI�ؼ�����ӽ��մ�������slot���磺
 *      FNotDealtWidget : public QWidget
 *      {
 *      private slots:
 *          void setIcon(QImage image);
 *      }
 *
 *      newһ��Workerʵ��������signal/slot������Thread::run(ThreadWorker *)
 *      �磺
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
 * ע�⣺
 *      1. �߳�֮��Ľ���ֻ��ͨ��signal/slot���ƽ���
 *      2. �̼߳䴫�ݲ���ʱ��Ҫʹ������
 *              void imageScaled(QImage img)            // ��ȷ
 *              void imageScaled(const QImage &img)     // Σ��
 *      3. GUI��ص�Qt�಻���������߳�֮�⣬��֪���У�
 *              QPixmap
 *              ����QWidget��������
 *      4. ����̼߳������ݲ������಻�Ǽ̳���QObject������Ҫʹ��qRegisterMeta()��main()�н���ע��
 *
 * PS��
 *      ����Workerд��˽���࣬����������ͻ
 *      ����ʵ���ɲμ�NotDealImageWidget��DealtImageWidget��Դ�ļ�
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
    static Thread *self;    // �����޷��ж�worker��signal/slot����

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
