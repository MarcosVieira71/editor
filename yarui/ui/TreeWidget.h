#pragma once

#include <memory>
#include <functional>
#include <stdexcept>

#include <QTreeWidget>

#include <yarui/reactive/ObservableContainer.h>
#include <yarui/reactive/ObservableValue.h>
#include <yarui/reactive/Subscription.h>
#include <yarui/ui/ReactiveWidget.h>

class TreeWidget : public ReactiveWidget
{
    public:

    TreeWidget() = default;

    TreeWidget(QTreeWidget* widget) : ReactiveWidget(widget), _widget(widget)
    {}

    ~TreeWidget() = default;
    
    TreeWidget(const TreeWidget&) = delete;
    TreeWidget& operator=(const TreeWidget&) = delete;

    TreeWidget(TreeWidget&&) noexcept = default;
    TreeWidget& operator=(TreeWidget&&) noexcept = default;


    template<typename T>
    void bindContainer(ObservableContainer<T>& container, std::function<std::string(const T&)> toText)
    {
        _widget->clear();

        for (const auto& item : container.items()) {
            addItem(QString::fromStdString(toText(item)));
        }

        _containerSubscription = container.subscribe(
            [widget = _widget, toText](const ContainerEvent<T>& ev)
            {
                if (!widget) return; 

                switch (ev.type)
                {
                    case ContainerEvent<T>::Type::Added:
                        if(!ev.item) throw std::runtime_error("Invalid optional");
                        {
                            auto* item = new QTreeWidgetItem(widget);
                            item->setText(0, QString::fromStdString(toText(*ev.item)));
                        }
                        break;

                    case ContainerEvent<T>::Type::Removed:
                        delete widget->takeTopLevelItem(static_cast<int>(ev.index));
                        break;

                    default:
                        break;
                }
            }
        );
    }

    void bindSelection(
        ObservableValue<std::optional<size_t>>& currentIndex,
        std::function<void(std::optional<size_t>)> setSelection)
    {
        _selectionSub = currentIndex.subscribe(
            [widget = _widget](const std::optional<size_t>& index)
            {
                if (!widget) return;

                if (!index) {
                    widget->clearSelection();
                    return;
                }

                auto* item = widget->topLevelItem(static_cast<int>(*index));
                if (item)
                    widget->setCurrentItem(item);
            }
        );

        QObject::connect(
            _widget,
            &QTreeWidget::itemSelectionChanged,
            _widget,
            [this, setSelection]()
            {
                auto items = _widget->selectedItems();

                if (items.empty()) {
                    setSelection(std::nullopt);
                    return;
                }

                auto* item = items.first();
                int index = _widget->indexOfTopLevelItem(item);

                if (index < 0)
                    setSelection(std::nullopt);
                else
                    setSelection(static_cast<size_t>(index));
            }
        );

    }


    private:
        void addItem(const QString& text) {
            auto* item = new QTreeWidgetItem(_widget);
            item->setText(0, text);
        }

    QTreeWidget* _widget;
    Subscription _containerSubscription;
    Subscription _selectionSub;
};