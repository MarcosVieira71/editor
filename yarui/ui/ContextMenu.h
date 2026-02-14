#pragma once

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QPoint>

#include <functional>
#include <vector>

#include <yarui/reactive/ObservableMap.h>
#include <yarui/reactive/Subscription.h>

class ContextMenu
{
public:
    ContextMenu() = default;

    explicit ContextMenu(QWidget* widget)
    {
        setWidget(widget);
    }

    void addAction(
        const std::string& text,
        std::function<void()> callback
    )
    {
        QAction* action = _menu.addAction(QString::fromStdString(text));

        QObject::connect(
            action,
            &QAction::triggered,
            action,
            [cb = std::move(callback)]()
            {
                cb();
            }
        );
    }

    void addSeparator()
    {
        _menu.addSeparator();
    }
    
    void setWidget(QWidget* widget)
    {
        if (!widget)
            throw std::invalid_argument("Widget is null");

        _widget = widget;

        _widget->setContextMenuPolicy(Qt::CustomContextMenu);

        QObject::connect(
            _widget,
            &QWidget::customContextMenuRequested,
            _widget,
            [this](const QPoint& pos)
            {
                _menu.exec(_widget->mapToGlobal(pos));
            }
        );
    }

    void bindMap(ObservableMap<std::string, std::function<void()>>& map)
    {
        _menu.clear();
        std::map<std::string, QAction*> actions;

        for (auto& [key, callback] : map.items()) {
            QAction* action = _menu.addAction(QString::fromStdString(key));
            QObject::connect(action, &QAction::triggered, [cb = callback]() { cb(); });
            actions[key] = action;
        }

        _subscription = map.subscribe(
            [&menu = _menu, &actions](const MapEvent<std::string, std::function<void()>>& ev)
            {
                switch (ev.type)
                {
                    case MapEvent<std::string, std::function<void()>>::Type::Added:
                        if (!ev.item) throw std::runtime_error("Invalid optional");
                        {
                            QAction* action = menu.addAction(QString::fromStdString(ev.key));
                            QObject::connect(action, &QAction::triggered, [cb = *ev.item]() { cb(); });
                            actions[ev.key] = action;
                        }
                        break;

                    case MapEvent<std::string, std::function<void()>>::Type::Removed:
                        {
                            auto it = actions.find(ev.key);
                            if (it != actions.end()) {
                                menu.removeAction(it->second);
                                delete it->second;
                                actions.erase(it);
                            }
                        }
                        break;

                    case MapEvent<std::string, std::function<void()>>::Type::Cleared:
                        for (auto& [key, action] : actions) {
                            menu.removeAction(action);
                            delete action;
                        }
                        actions.clear();
                        break;
                }
            }
        );
    }

private:
    Subscription _subscription;
    QWidget* _widget;
    QMenu _menu;
};
