#include "mysortfilterproxymodel.h"
#include <QTextDocumentFragment>

MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

QString MySortFilterProxyModel::htmlToPlainText(const QString& html) const
{
    return QTextDocumentFragment::fromHtml(html).toPlainText();
}

bool MySortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex index1 = sourceModel()->index(sourceRow, 3, sourceParent);

    bool cond1 = sourceModel()->data(index0).toString().contains(filterRegularExpression());
    bool cond2 = sourceModel()->data(index1).toString().contains(filterRegularExpression());
    if ( cond2 ) {
        cond2 = htmlToPlainText(sourceModel()->data(index1).toString()).contains(filterRegularExpression());
    }

    return cond1 || cond2;
}
