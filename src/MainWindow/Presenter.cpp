#include "Presenter.h"

#include <QPixmap>
#include <QFileDialog>

#include <core/Image.h>
#include <core/filters/grayscale.h>
#include <core/filters/inversion.h>
#include <infra/ThreadPool.h>

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
    _view->bindModel(_model->images());
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
    runAsync(
        [this]() {
            _model->execute(std::make_unique<FilterCommand>(grayscale));
        },
        [this]() {
            refresh();
        }
    );
}

void Presenter::onInversion()
{
    runAsync(
        [this]() {
            _model->execute(std::make_unique<FilterCommand>(inversion));
        },
        [this](){
            refresh();
        }
    );
}

void Presenter::onUndo()
{
    runAsync(
        [this]() {
            _model->undo();
        },
        [this]() {
            refresh();
        }
    );
}

void Presenter::onRedo()
{
    runAsync(
        [this]() {
            _model->redo();
        },
        [this]() {
            refresh();
        }
    );
}

void Presenter::runAsync(std::function<void()> task, std::function<void()> after_task)  
{
    ThreadPool::getInstance().runAsync(std::move(task), std::move(after_task));
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