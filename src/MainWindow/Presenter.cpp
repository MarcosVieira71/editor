#include "Presenter.h"

#include <QPixmap>
#include <QFileDialog>

#include <Image.h>

#include "View.h"
#include "Model.h"
#include "ToQImageAdapter.h"


Presenter::Presenter() : _view(std::make_unique<View>()), _model(std::make_unique<Model>())
{   
    _view->setOpenMenuCallback([&](){onOpenImage();});
}

void Presenter::start()
{
   _view->show();
} 

Presenter::~Presenter() = default;

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

    auto img = Image(path.toStdString());

    auto pixmap = QPixmap::fromImage(toQImage(img));
    _model->addImage(std::move(img));

    _view->setImage(pixmap);
}
