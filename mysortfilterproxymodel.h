#ifndef MYSORTFILTERPROXYMODEL_H
#define MYSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class MySortFilterProxyModel : public QSortFilterProxyModel
{
public:
    explicit MySortFilterProxyModel(QObject *parent = nullptr);
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
private:
    QString htmlToPlainText(const QString& html) const;
};

#endif // MYSORTFILTERPROXYMODEL_H
