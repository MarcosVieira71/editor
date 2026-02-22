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


    template<typename T, typename Id>
    void bindContainer(
        ObservableContainer<T>& container,
        std::function<std::string(const T&)> toText,
        std::function<Id(const T&)> toId
    )
    {
        _widget->clear();

        for (const auto& item : container.items())
        {
            auto* treeItem = new QTreeWidgetItem(_widget);
            treeItem->setText(0, QString::fromStdString(toText(item)));

            QVariant v;
            v.setValue(toId(item)); 
            treeItem->setData(0, Qt::UserRole, v);
        }

        _containerSubscription = container.subscribe(
            [widget = _widget, toText, toId](const ContainerEvent<T>& ev)
            {
                if (!widget) return;

                switch (ev.type)
                {
                    case ContainerEvent<T>::Type::Added:
                    {
                        auto* item = new QTreeWidgetItem(widget);
                        item->setText(0, QString::fromStdString(toText(*ev.item)));

                        QVariant v;
                        v.setValue(toId(*ev.item));
                        item->setData(0, Qt::UserRole, v);
                    }
                    break;

                    case ContainerEvent<T>::Type::Removed:
                    {
                        delete widget->takeTopLevelItem(static_cast<int>(ev.index));
                    }
                    break;

                    default:
                    break;
                }
            }
        );
    }

    template<typename Id>
    void bindSelection(
        ObservableValue<std::optional<Id>>& currentId,
        std::function<void(std::optional<Id>)> setSelection)
    {
        _selectionSub = currentId.subscribe(
            [widget = _widget](const std::optional<Id>& id)
            {
                if (!widget) return;

                if (!id)
                {
                    widget->clearSelection();
                    return;
                }

                for (int i = 0; i < widget->topLevelItemCount(); ++i)
                {
                    auto* item = widget->topLevelItem(i);
                    auto value = item->data(0, Qt::UserRole).value<Id>();

                    if (value == *id)
                    {
                        widget->setCurrentItem(item);
                        return;
                    }
                }

                widget->clearSelection();
            }
        );

        QObject::connect(
            _widget,
            &QTreeWidget::itemSelectionChanged,
            _widget,
            [this, setSelection]()
            {
                auto items = _widget->selectedItems();

                if (items.empty())
                {
                    setSelection(std::nullopt);
                    return;
                }

                auto* item = items.first();
                auto id = item->data(0, Qt::UserRole);

                if (!id.isValid())
                    setSelection(std::nullopt);
                else
                    setSelection(id.value<Id>());
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
    std::vector<QTreeWidgetItem*> _items;
};