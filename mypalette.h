#ifndef MYPALETTE_H
#define MYPALETTE_H

#include<QMap>
#include<QColor>

class MyPalette
{
public:
    static MyPalette* instance();
    QColor color(const QString&);
    static MyPalette *_instance;
private:
    MyPalette();
    QMap<QString, QColor> colors;
};

#endif // MYPALETTE_H
