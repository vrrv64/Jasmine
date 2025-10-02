#ifndef LISTVIEWDELEGATE_H
#define LISTVIEWDELEGATE_H

#include <QStyledItemDelegate>

class ListViewDelegate : public QStyledItemDelegate
{
public:
    explicit ListViewDelegate(QObject *parent = nullptr);
protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const;
private:
    QString truncateString(const QString& str, const QFont& font, int w) const;
    //QColor subtitle, highlightSubtitle, separator;
};

#endif // LISTVIEWDELEGATE_H
