#include <QApplication>

#include "Image.h"
#include "MainWindow/Presenter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Presenter p;
    p.start();

    return app.exec();
}
