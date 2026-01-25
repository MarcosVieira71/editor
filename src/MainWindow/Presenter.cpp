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
    initActionMap();
    _view->setActionCallback(
        [this](ViewAction action) {
            auto it = _actionMap.find(action);
            if (it != _actionMap.end())
                it->second();
        }
    );

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
        "Images (*.jpeg *.jpg *.png *.bmp)"
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
    if(_model->execute(
        std::make_unique<FilterCommand>(grayscale)
    )) refresh();
}


void Presenter::onInversion()
{
    if(_model->execute(
        std::make_unique<FilterCommand>(inversion)
    )) refresh();
}

void Presenter::onUndo()
{
    if (_model->undo())
        refresh();
}

void Presenter::onRedo()
{
    if (_model->redo())
        refresh();
}

void Presenter::initActionMap()
{
    _actionMap = {
        { ViewAction::Open,      [this]() { onOpenImage(); } },
        { ViewAction::Grayscale, [this]() { if(_model->isImageSelected()) onGrayscale(); } },
        { ViewAction::Inversion, [this]() { if(_model->isImageSelected()) onInversion(); } },
        { ViewAction::FitImage,  [this]() { _view->fitImage(); } },
        { ViewAction::Undo,      [this]() { onUndo(); } },
        { ViewAction::Redo,      [this]() { onRedo(); } }
    };
}

void Presenter::refresh()
{
    if(auto img = _model->currentImage(); img.has_value())
        _view->setImage(QPixmap::fromImage(toQImage(**img)));
}