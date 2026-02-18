#include "Presenter.h"

#include <core/Image.h>
#include <core/transformations/grayscale.h>
#include <core/transformations/inversion.h>
#include <core/transformations/binarization.h>
#include <infra/ThreadPool.h>
#include <infra/ImageLoader.h>
#include <infra/TaskScheduler.h>

#include "View.h"
#include "Model.h"
#include "Commands/FilterCommand.h"



Presenter::Presenter() : _view(std::make_unique<View>()), _model(std::make_unique<Model>())
{   
    initActionMap();
}

void Presenter::start()
{
    _view->show();
    _view->bindModel(*_model);
    _view->bindActionMap(_actionMap);

    _selectionSubscription = _model->selection().subscribe(
        [this](const std::optional<size_t>& idx)
            {
                if (!idx)
                    return;
                refresh();
            }
        );

} 

Presenter::~Presenter() = default;

void Presenter::onOpenImage()
{
    const auto result = _view->fileDialog(FileDialogMode::Open, "Open File", "Images (*.jpeg *.jpg *.png *.bmp)");

    if (!result.has_value())
        return;

    TaskScheduler::schedule<Image>(
        [path = result->first](){return ImageLoader::load(path);},
        [this](Image&& img){
            _model->addImage(std::move(img));
            _view->setImage(img);
        } 
    );
}

void Presenter::onSaveImage()
{
    const auto result = _view->fileDialog(
        FileDialogMode::Save,
        "Save File",
        "JPEG (*.jpg *.jpeg);;PNG (*.png);;BMP (*.bmp)"
    );

    if (!result)
        return;

    TaskScheduler::schedule<bool>(
        [img = _model->currentImage(), path = result->first, extension = result->second](){
            if(!img) return 0;
            try{
                ImageLoader::save(*img, path, extension);
                return 1;
            }
            catch(...){
                return 0;
            }
            
        },
        [this](bool&& ok){
            if(ok) _view->showInfoDialog("Save", "Your image has been successfully saved");
            else _view->showInfoDialog("Save", "Something went wrong while saving your image. You could try again now");
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
    TaskScheduler::schedule<std::optional<std::size_t>>(
        [this]() {
            return _model->undo();
        },
        [this](std::optional<std::size_t> idx) {
            if (idx){
                _model->select(*idx);
                refresh();
            }
        }
    );
}

void Presenter::onRedo()
{
    TaskScheduler::schedule<std::optional<std::size_t>>(
        [this]() {
           return _model->redo();
        },
        [this](std::optional<std::size_t> idx) {
            if (idx){
                _model->select(*idx);
                refresh();

            }
        }
    );  
}

void Presenter::initActionMap()
{
    _view->setOpenCallback([this]() { onOpenImage(); });
    _actionMap.add("Grayscale", [this]() { if(_model->isImageSelected()) onGrayscale(); });
    _actionMap.add("Inversion", [this]() { if(_model->isImageSelected()) onInversion(); });
    _actionMap.add("Binarization", [this]() { if(_model->isImageSelected()) onBinarization(); });
    _actionMap.add("Fit Image", [this]() { _view->fitImage(); });
    _actionMap.add("Undo", [this]() { onUndo(); });
    _actionMap.add("Redo", [this]() { onRedo(); });
    _actionMap.add("Save", [this]() { onSaveImage(); });
}

void Presenter::refresh()
{
    if(auto img = _model->currentImage(); img.has_value())
        _view->setImage(*img);
}