#ifndef LISTVIEWDELEGATE_H
#define LISTVIEWDELEGATE_H

#include <QStyledItemDelegate>

class FolderViewItemDelegate : public QStyledItemDelegate
{
public:
    explicit FolderViewItemDelegate(QObject *parent = nullptr);
protected:
    void initStyleOption(QStyleOptionViewItem * option, const QModelIndex & index) const;
};

#endif // LISTVIEWDELEGATE_H
