#ifndef FOLDERVIEWCONTROLLER_H
#define FOLDERVIEWCONTROLLER_H

#include <QIcon>
#include <QObject>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FolderViewController : public QObject
{
    Q_OBJECT
public:
    explicit FolderViewController(QObject *parent = nullptr);
    void setup(Ui::MainWindow *);
    void applicationWillClose();
    ~FolderViewController();
    int currentFolder();

signals:
    void folderChanged(int);

public slots:
    void addFolder();

private slots:
    void itemClicked(const QModelIndex &);
    void showFoderView();
    void hideFoderView();
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    QSqlTableModel *sqlTableModel;
    Ui::MainWindow *ui;
    QModelIndex cur_selection;
    QIcon forwardIcon;
    QIcon backIcon;
};

#endif // FOLDERVIEWCONTROLLER_H
