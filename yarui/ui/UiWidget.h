#pragma once

#include <QtUiTools/QUiLoader>
#include <QFile>
#include <QWidget>
#include <stdexcept>

class UiWidget {
public:
    explicit UiWidget(const QString& uiPath) {
        QFile file(uiPath);
        if (!file.open(QFile::ReadOnly))
            throw std::runtime_error(("Cannot open UI file: " + uiPath).toStdString());

        QUiLoader loader;
        _widget = loader.load(&file);
        file.close();

        if (!_widget)
            throw std::runtime_error(("Failed to load UI: " + uiPath).toStdString());
    }

    operator QWidget*() const { return _widget; }

private:
    QWidget* _widget = nullptr;
};
