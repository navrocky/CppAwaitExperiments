#include <iostream>

#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QtConcurrentRun>
#include <CppAwait/Awaitable.h>

using namespace std;

class Thread: public QThread
{
public:
    using QThread::msleep;
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

void startAsyncAndForget(std::string tag, ut::Action func)
{
    new ut::Awaitable(ut::startAsync(tag, func));
}


void startAsyncOnPool(ut::Action func)
{
    auto future = QtConcurrent::run(func);
    future.
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

    QTimer::singleShot(100, &app, SLOT(quit()));

    for (int i = 0; i < 3; i++)
        startAsyncAndForget("<c1a8c242>", []()
        {
            qDebug() << "<9bf2c92f> Before delay";
            asyncDelay("<923ff103>", 500).await();
            qDebug() << "<5d2c85f6> After delay";
            throw std::runtime_error("<8903e8f4>");
        });

    qDebug() << "<22fa4b84> Before event loop";
    app.exec();
    qDebug() << "<64225e1f> Application closed";

    return 0;
}

#include "moc_main.cxx"
