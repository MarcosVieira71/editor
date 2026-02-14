#include "Presenter.h"

#include <QPixmap>
#include <QFileDialog>

#include <core/Image.h>
#include <core/filters/grayscale.h>
#include <core/filters/inversion.h>
#include <core/filters/binarization.h>
#include <infra/ThreadPool.h>
#include <infra/TaskScheduler.h>

#include "View.h"
#include "Model.h"
#include "ToQImageAdapter.h"
#include "Commands/FilterCommand.h"



Presenter::Presenter() : _view(std::make_unique<View>()), _model(std::make_unique<Model>())
{   
    initActionMap();
}

void Presenter::start()
{
    _view->show();
    _view->bindModel(_model->images());
    _view->bindActionMap(_actionMap);
} 

Presenter::~Presenter() {
    _view.reset();
    _model.reset();
}

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

    TaskScheduler::schedule<Image>(
        [&path](){return Image(path.toStdString());},
        [this](Image&& img){
            auto pixmap = QPixmap::fromImage(toQImage(img));
            _model->addImage(std::move(img));
            _view->setImage(pixmap);
        } 
    );
}


void Presenter::onGrayscale() {
    TaskScheduler::schedule(
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
    TaskScheduler::schedule(
        [this]() {
            _model->execute(std::make_unique<FilterCommand>(inversion));
        },
        [this](){
            refresh();
        }
    );
}


void Presenter::onBinarization()
{
    TaskScheduler::schedule(
        [this]() {
            _model->execute(std::make_unique<FilterCommand>(binarization));
        },
        [this]() {
            refresh();
        }
    );
}


void Presenter::onUndo()
{
    TaskScheduler::schedule(
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
    TaskScheduler::schedule(
        [this]() {
            _model->redo();
        },
        [this]() {
            refresh();
        }
    );
}

void Presenter::initActionMap()
{
    _actionMap.add("Open", [this]() { onOpenImage(); });
    _actionMap.add("Grayscale", [this]() { if(_model->isImageSelected()) onGrayscale(); });
    _actionMap.add("Inversion", [this]() { if(_model->isImageSelected()) onInversion(); });
    _actionMap.add("Binarization", [this]() { if(_model->isImageSelected()) onBinarization(); });
    _actionMap.add("Fit Image", [this]() { _view->fitImage(); });
    _actionMap.add("Undo", [this]() { onUndo(); });
    _actionMap.add("Redo", [this]() { onRedo(); });
}

void Presenter::refresh()
{
    if(auto img = _model->currentImage(); img.has_value())
        _view->setImage(QPixmap::fromImage(toQImage(**img)));
}