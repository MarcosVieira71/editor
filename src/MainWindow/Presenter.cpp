#include "Presenter.h"

#include <QPixmap>
#include <QFileDialog>

#include "View.h"
#include "../Image.h"

Presenter::Presenter() : _view(std::make_unique<View>())
{   
    _view->setOpenMenuCallback([&](){onOpenImage();});
}

Presenter::~Presenter()
{
}

void Presenter::start()
{
   _view->show();
} 

void Presenter::onOpenImage()
{
    QString path = QFileDialog::getOpenFileName(
        nullptr,
        "Open file",
        QString(),
        "Images (*.jpeg *.jpg *.png)"
    );

    if (path.isEmpty())
        return;

    auto pixmap = QPixmap::fromImage(
        Image::toQImage(Image(path.toStdString()))
    );

    _view->setImage(pixmap);
}
