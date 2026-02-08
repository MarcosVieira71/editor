#pragma once

#include <memory>
#include <functional>
#include <stdexcept>

#include <QTreeWidget>

#include <yarui/reactive/ObservableContainer.h>
#include <yarui/reactive/Subscription.h>

template<typename T>
class TreeWidget
{
    public:

    TreeWidget() = default;

    TreeWidget(QTreeWidget* widget) : _widget(widget)
    {}

    void bindContainer(ObservableContainer<T>& container, std::function<std::string(const T&)> toText)
    {
        _widget->clear();

        for (const auto& item : container.items()) {
            addItem(QString::fromStdString(toText(item)));
        }

        _subscription = container.subscribe(
            [this, toText](const ContainerEvent<T>& ev)
            {
                switch (ev.type)
                {
                    case ContainerEvent<T>::Type::Added:
                        if(!ev.item) throw std::runtime_error("Invalid optional");
                        
                        addItem(QString::fromStdString(toText(*ev.item)));
                        break;

                    case ContainerEvent<T>::Type::Removed:
                        delete _widget->takeTopLevelItem(
                            static_cast<int>(ev.index)
                        );
                        break;

                    default:
                        break;
                }
            }
        );
    }

    private:
        void addItem(const QString& text) {
            auto* item = new QTreeWidgetItem(_widget);
            item->setText(0, text);
        }

    QTreeWidget* _widget;
    Subscription _subscription;
};