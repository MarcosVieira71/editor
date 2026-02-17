#pragma once

#include <memory>
#include <functional>
#include <stdexcept>

#include <QTreeWidget>

#include <yarui/reactive/ObservableContainer.h>
#include <yarui/reactive/Subscription.h>

class TreeWidget
{
    public:

    TreeWidget() = default;

    TreeWidget(QTreeWidget* widget) : _widget(widget)
    {}

    ~TreeWidget()
    {
        _subscription = {};
    }

    template<typename T>
    void bindContainer(ObservableContainer<T>& container, std::function<std::string(const T&)> toText)
    {
        _widget->clear();

        for (const auto& item : container.items()) {
            addItem(QString::fromStdString(toText(item)));
        }

        _subscription = container.subscribe(
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

    private:
        void addItem(const QString& text) {
            auto* item = new QTreeWidgetItem(_widget);
            item->setText(0, text);
        }

    QTreeWidget* _widget;
    Subscription _subscription;
};