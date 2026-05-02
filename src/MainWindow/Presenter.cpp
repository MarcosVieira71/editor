#include "Presenter.h"

#include <core/image/ImageData.h>
#include <core/filter/sobel.h>
#include <core/transformation/grayscale.h>
#include <core/transformation/inversion.h>
#include <core/transformation/binarization.h>
#include <infra/images/ImageLoader.h>
#include <infra/threading/TaskScheduler.h>

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
    _view->bindSceneActionMap(_sceneActionMap);
    _view->bindTreeActionMap(_treeActionMap);

    _selectionSubscription = _model->selection().subscribe(
        [this](const std::optional<size_t>& id)
            {
                refresh();   
                if (!id) {
                    _treeActionMap.remove("Remove");
                    return;
                }
                _treeActionMap.add("Remove", [i=*id, this](){ onRemoveImage(i); });
            }
        );

} 

Presenter::~Presenter() = default;

void Presenter::onOpenImage()
{
    const auto result = _view->fileDialog(FileDialogMode::Open, "Open File", "Images (*.jpeg *.jpg *.png *.bmp)");

    if (!result.has_value())
        return;

    TaskScheduler::schedule<image::ImageData>(
        [path = result->first](){return ImageLoader::load(path);},
        [this](image::ImageData&& img_data){
            _model->addImage(std::move(img_data));
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

void Presenter::onRemoveImage(std::size_t id)
{
    _model->removeImage(id);
}

void Presenter::onGrayscale() {
    TaskScheduler::schedule(
        [this]() { 
            _model->execute(std::make_unique<FilterCommand>(transformation::grayscale)); 
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
            _model->execute(std::make_unique<FilterCommand>(transformation::inversion));
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
            _model->execute(std::make_unique<FilterCommand>(transformation::binarization));
        },
        [this]() {
            refresh();
        }
    );
}

void Presenter::onSobel()
{
    TaskScheduler::schedule(
        [this]() {
            _model->execute(std::make_unique<FilterCommand>(filter::sobel));
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

void Presenter::onRevert()
{
    TaskScheduler::schedule<std::optional<std::size_t>>(
        [this]() {
           return _model->revert();
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
    _sceneActionMap.add("Grayscale", [this]() { if(_model->isImageSelected()) onGrayscale(); });
    _sceneActionMap.add("Inversion", [this]() { if(_model->isImageSelected()) onInversion(); });
    _sceneActionMap.add("Binarization", [this]() { if(_model->isImageSelected()) onBinarization(); });
    _sceneActionMap.add("Fit Image", [this]() { _view->fitImage(); });
    _sceneActionMap.add("Sobel", [this]() { if(_model->isImageSelected()) onSobel();});
    _sceneActionMap.add("Undo", [this]() { onUndo(); });
    _sceneActionMap.add("Redo", [this]() { onRedo(); });
    _sceneActionMap.add("Revert", [this]() { onRevert(); });

    _treeActionMap.add("Save", [this]() { onSaveImage(); });
}

void Presenter::refresh()
{
    if(auto img = _model->currentImage(); img.has_value())
        _view->setImage(*img);
    else _view->clear();
}