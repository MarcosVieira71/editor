#pragma once

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QPoint>

#include <functional>
#include <vector>


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
private:


    QWidget* _widget;
    QMenu _menu;
};
