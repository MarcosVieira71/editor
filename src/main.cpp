#include <QApplication>

#include <QWidget>
#include <QPixmap>

#include "Image.h"
#include "MainWindow/View.h"

constexpr const char* path =
    "../test_images/test.jpg";

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    View main;
    main.show();
    main.scene()->addPixmap(QPixmap::fromImage(Image::toQImage(Image(path))));

    return app.exec();
}
