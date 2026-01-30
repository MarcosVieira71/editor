#include <QApplication>
#include <QTimer>

#include "Image.h"
#include "MainWindow/Presenter.h"
#include <ThreadPool.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Presenter p;
    p.start();

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [](){
        ThreadPool::getInstance().processMainThreadTasks();
    });
    timer.start(16); // ~60 FPS, chama a cada 16ms

    return app.exec();
}
