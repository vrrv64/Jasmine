#include "mypalette.h"

#include <QPalette>
#include <QApplication>

MyPalette::MyPalette()
{
    auto p = QApplication::palette();
    bool darkMode = (p.color(QPalette::WindowText).lightness() > p.color(QPalette::Window).lightness());
    if ( darkMode ) {
        colors["separator"] = QColor::fromRgb(80, 80, 80, 255);
        colors["text-shadow"] = QColor::fromRgb(0, 0, 0, 255);
    } else {
        colors["separator"] = QColor::fromRgb(208, 208, 208, 255);
        colors["text-shadow"] = QColor::fromRgb(255, 255, 255, 255);
    }

    colors["subtitle"] = QColor::fromRgb(138, 138, 138, 255);
    colors["highlightSubtitle"] = QColor::fromRgb(200, 200, 200, 255);
}

MyPalette *MyPalette::instance()
{
    if ( _instance == nullptr ) {
        _instance = new MyPalette();
    }
    return _instance;
}

QColor MyPalette::color(const QString& key)
{
    return colors[key];
}
