#include <QApplication>
#include <QTimer>

#include <infra/threading/ThreadPool.h>

#include "MainWindow/Presenter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Presenter p;
    p.start();

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [](){
        ThreadPool::getInstance().processMainThreadTasks();
    });
    timer.start(16); // ~60 FPS
    return app.exec();
}
