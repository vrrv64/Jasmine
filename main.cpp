#include "mainwindow.h"
#include "mypalette.h"

#include <QDebug>
#include <QPalette>
#include <QApplication>
//#include <QStyleFactory>

MyPalette *MyPalette::_instance = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QFont font("P052 [urw]");
    //QFont font("Arial");
    //QFont font("Nimbus Sans [urw]");
    //font.setStyleHint(QFont::Monospace);
    //QApplication::setFont(font);
    //QFontInfo fifo(font);
    //qDebug() << fifo.family();
    //qDebug() << font.substitute("Helvetica");
    //a.setStyle("Windows");
    //qDebug() << QStyleFactory::keys();
    //qDebug() << a.style();
    //QPalette p;
    //auto p = QApplication::palette();
    //qDebug() << (p.color(QPalette::WindowText).lightness() > p.color(QPalette::Window).lightness());
    //a.setStyleSheet("QMenu::separator {background: rgb(80, 80, 80);height: 1px;margin-top:4px;margin-bottom:4px;}");
    MainWindow w;
    w.show();
    //qDebug() << w.palette();
    return a.exec();
}
