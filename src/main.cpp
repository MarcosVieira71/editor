#include <QApplication>
#include <QFile>
#include <QUiLoader>
#include <QWidget>
#include <QLabel>

#include "Image.h"
#include "ToQImage.h"

constexpr const char* path =
    "/home/marcosvieira/editor/test_images/test.jpg";

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QUiLoader loader;
    QFile file("/home/marcosvieira/editor/ui/MainWindow.ui");
    if (!file.open(QFile::ReadOnly)) {
        printf("oioii\n");
        return -1;
    }

    QWidget *window = loader.load(&file);
    file.close();

    if (!window) {
        printf("oioii\n");

        return -1;
    }

    Image img(path);
    QImage qimg = to_QImage(img);
    QPixmap pixmap = QPixmap::fromImage(qimg);


    window->show();
    return app.exec();
}
