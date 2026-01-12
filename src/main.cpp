#include <QApplication>

#include <QWidget>
#include <QPixmap>
#include <QFileDialog>

#include "Image.h"
#include "MainWindow/View.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    View main;
    main.show();
    main.setOpenMenuCallback([&main]() {
        QString path = QFileDialog::getOpenFileName(
            nullptr,                
            "Open file",          
            QString(),                
            "Images (*.jpeg *.jpg *.png)"
        );

        if (!path.isEmpty()) {
            printf("%s\n", path.toUtf8().constData());
            main.scene()->addPixmap(QPixmap::fromImage(Image::toQImage(Image(path.toStdString()))));
        }
    });

    return app.exec();
}
