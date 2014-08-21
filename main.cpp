#include <iostream>

#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QElapsedTimer>
#include <QtConcurrentRun>
#include <CppAwait/Awaitable.h>

using namespace std;

class Thread: public QThread
{
public:
    using QThread::msleep;
};

class TimerTicker : public QObject
{
    Q_OBJECT
public:
    TimerTicker(int interval)
    {
        timer_ = startTimer(interval);
    }

    bool event(QEvent *e)
    {
        if (e->type() == QEvent::Timer)
            qDebug() << "<3ac15722> Timer tick";
    }

private:
    int timer_;
};

class TimerCompleter : public QObject
{
    Q_OBJECT
public:
    TimerCompleter(const ut::Completer& completer)
        : completer_(completer)
    {}

public slots:
    void timeout()
    {
        deleteLater();
        qDebug() << "<aa4f77c2> timeout";
        completer_();
    }
private:
    ut::Completer completer_;
};

ut::Awaitable asyncDelay(const std::string& tag, int delay)
{
    ut::Awaitable awt;
    awt.setTag(tag);

    auto timerCompleter = new TimerCompleter(awt.takeCompleter());

    QTimer::singleShot(delay, timerCompleter, SLOT(timeout()));

    return std::move(awt);
}

void blockingCall(int msec)
{
    QElapsedTimer timer;
    timer.start();
    while (1)
    {
        if (timer.elapsed() > msec)
            break;
    }
}

void startAsyncAndForget(std::string tag, ut::Action func)
{
    new ut::Awaitable(ut::startAsync(tag, func));
}

ut::Awaitable startAsyncOnPool(const std::string& tag, ut::Action func)
{
    ut::Awaitable awt;
    awt.setTag(tag);
    auto completer = awt.takeCompleter();
    QtConcurrent::run([completer, func]()
    {
        func();
        completer();
    });
    return std::move(awt);
}

void qtMsgHandler(QtMsgType, const char *msg)
{
    std::cout << QDateTime::currentDateTime().toString("hh:mm:ss.zzz").toLocal8Bit().data() << ": "
              "thread=" << QThread::currentThreadId() << ", " << msg << std::endl;
}

int main(int argc, char** argv)
{
    qInstallMsgHandler(&qtMsgHandler);

    QCoreApplication app(argc, argv);

    qDebug() << "<64225e1f> Application started";

    TimerTicker ticker(100);

#if 1
    auto task = ut::startAsync("<49ccd99c>", []()
    {
        qDebug() << "<30e0508d> Before start on pool";
        auto task = startAsyncOnPool("<1a37114c>", [](){
            qDebug() << "<ce3f8e7e> Started on pool, before blocking call";
            blockingCall(500);
            qDebug() << "<e58ab2ad> After blocking call";
        });
        qDebug() << "<3552e165> After call blocking op on pool";
        task.await();
        qDebug() << "<0e9d0aa8> After await blocking call results";
    });
#endif

#if 0
    for (int i = 0; i < 3; i++)
        startAsyncAndForget("<c1a8c242>", []()
        {
            qDebug() << "<9bf2c92f> Before async delay";
            asyncDelay("<923ff103>", 500).await();
            qDebug() << "<5d2c85f6> After async delay";
            throw std::runtime_error("<8903e8f4>");
        });
#endif

    qDebug() << "<22fa4b84> Before event loop";
    QTimer::singleShot(1000, &app, SLOT(quit()));
    app.exec();
    qDebug() << "<64225e1f> Application closed";

    return 0;
}

#include "main.moc"
