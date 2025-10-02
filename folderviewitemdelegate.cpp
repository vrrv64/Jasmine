#include "folderviewitemdelegate.h"

FolderViewItemDelegate::FolderViewItemDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

void FolderViewItemDelegate::initStyleOption(QStyleOptionViewItem * option, const QModelIndex & index) const
{
     QStyledItemDelegate::initStyleOption(option, index);

     if( option->state & QStyle::State_Selected )
          option->font.setWeight( QFont::ExtraBold	);
}
