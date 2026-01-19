#include "Presenter.h"

#include <QPixmap>
#include <QFileDialog>

#include <Image.h>
#include <filters/grayscale.h>
#include <filters/inversion.h>

#include "View.h"
#include "Model.h"
#include "ToQImageAdapter.h"
#include "Commands/FilterCommand.h"

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


void Presenter::onGrayscale()
{
    _model->execute(
        std::make_unique<FilterCommand>(grayscale)
    );
    _view->setImage(QPixmap::fromImage(toQImage(_model->currentImage())));
}

void Presenter::onInversion()
{
    _model->execute(
        std::make_unique<FilterCommand>(inversion)
    );
    _view->setImage(QPixmap::fromImage(toQImage(_model->currentImage())));
}

void Presenter::onUndo()
{
    if (_model->undo())
        _view->setImage(QPixmap::fromImage(toQImage(_model->currentImage())));
}

void Presenter::onRedo()
{
    if (_model->redo())
        _view->setImage(QPixmap::fromImage(toQImage(_model->currentImage())));
}
