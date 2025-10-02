#include "listviewdelegate.h"
#include "mypalette.h"
#include <QApplication>
#include <QPainter>
#include <QDebug>
#include <QDateTime>

ListViewDelegate::ListViewDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

void ListViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QString line0 = index.model()->data(index.model()->index(index.row(), 0)).toString();
    QString line1 = index.model()->data(index.model()->index(index.row(), 1)).toString();

    //cout << line1.toStdString() << endl;
    line1 = QDateTime::fromString(line1, Qt::ISODate).toString("MM/dd/yy hh:mm AP");
    //cout << line1.toStdString() << endl;

    // draw correct background
    opt.text = "";
    QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

    QRect rect = opt.rect;
    QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
    if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
        cg = QPalette::Inactive;

    QFont font = painter->font();

    //cout << rect.width() << ", " << opt.widget->width() << endl;

    // set pen color
    if (opt.state & QStyle::State_Selected) {
        painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
        //font.setWeight(QFont::Bold);
        font.setWeight(QFont::Bold);
    } else {
        painter->setPen(opt.palette.color(cg, QPalette::WindowText));
        //font.setWeight(QFont::Normal);
        font.setWeight(QFont::Medium);
        font.setWeight(QFont::Bold);
    }
    //qDebug() << font.weight();

    // draw 2 lines of text
    //font.setWeight(QFont::Bold);
    font.setPointSize(font.pointSize()+0);
    painter->setFont(font);
    QString str = truncateString(line0, font, opt.widget->width()-30);
    painter->drawText(QRect(rect.left()+10, rect.top()+1, opt.widget->width()-20, rect.height()/2),
                      opt.displayAlignment, str);

    if (opt.state & QStyle::State_Selected)
        painter->setPen(MyPalette::instance()->color("highlightSubtitle"));
    else
        painter->setPen(MyPalette::instance()->color("subtitle"));

    //font.setWeight(QFont::Normal);
    font.setWeight(QFont::Medium);
    font.setPointSize(font.pointSize()-2);
    painter->setFont(font);
    painter->drawText(QRect(rect.left()+10, rect.top()+rect.height()/2-1, opt.widget->width()-20, rect.height()/2),
                      opt.displayAlignment, line1);
    font.setPointSize(font.pointSize()+2);
    painter->setFont(font);

    //qDebug() << MyPalette::instance()->color("separator");
    painter->setPen(MyPalette::instance()->color("separator"));
    painter->drawLine(rect.left(), rect.top()+rect.height(), rect.left()+rect.width(), rect.top()+rect.height());
}

QSize ListViewDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QFontMetrics fm(option.font);
    QSize result = QStyledItemDelegate::sizeHint(option, index);
    //qDebug() << fm.height() << ", " << result.height() << ", " << (result.height() - fm.height()) << fm.leading();
    int padding = fm.leading() <=0 ? 2 : 0;
    result.setHeight(result.height()*2 + padding);
    //result.setHeight(result.height()*2 + 6 - 2*(result.height() - fm.lineSpacing()));
    return result;
}

QString ListViewDelegate::truncateString(const QString& str, const QFont& font, int w) const
{
    int i;
    for(i = str.length()-1;i > 1;i --) {
        QFontMetrics fm(font);
        //int pixelsWide = fm.width(str.left(i));
        int pixelsWide = fm.horizontalAdvance(str.left(i));
        if ( pixelsWide <= w ) {
            break;
        }
    }

    if(i == str.length() - 1)
        return str.left(i+1);
    else
        return str.left(i) + "...";
}
