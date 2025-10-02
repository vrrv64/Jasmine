#include "folderviewcontroller.h"
#include "folderviewitemdelegate.h"
#include <QDebug>
#include <QDateTime>
#include <QSettings>
#include "ui_mainwindow.h"
#include "mainwindow.h"

FolderViewController::FolderViewController(QObject *parent)
    : QObject{parent}
{

}

void FolderViewController::setup(Ui::MainWindow *ui1)
{
    QSettings settings;
    this->ui = ui1;

    ui->folderList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    int row = settings.value("selected_folder").toInt();

    sqlTableModel = new QSqlTableModel();
    sqlTableModel->setTable("folders");
    sqlTableModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    sqlTableModel->setSort(2, Qt::AscendingOrder);
    sqlTableModel->setHeaderData(1, Qt::Horizontal, tr("Folder"));
    sqlTableModel->select();

    ui->folderList->setModel(sqlTableModel);
    ui->folderList->setModelColumn(1);
    ui->folderList->setSpacing(1);
    //ui->folderList->setStyleSheet("QListView::item { border-bottom: 1px solid red;padding: 14px;}");
    QPalette palette = ui->folderList->palette();
    QString style = QString("QListView::item { show-decoration-selected: 1; padding: 4px; }");
    //style += "QListView::item:alternate { background-color: #EEEEEE; }";
    //style += "QListView::item:selected:active { font-weight: bold; }";
    style += "QListView {margin: 32px -1px -1px -1px;background-color:%1;border-top: 0px solid gray;} ";
    style = style.arg(palette.color(QPalette::Active, QPalette::Window).name());

    ui->folderList->setStyleSheet(style);
    //ui->folderList->setFont(QFont("Helvetica [Cronyx]", 16, QFont::Bold));
    QFont font = ui->folderList->font();
    //ui->folderList->setFont(QFont(font.families(), font.pointSize(), QFont::Bold));
    //ui->folderList->setStyleSheet("QListView::item { show-decoration-selected: 1; padding: 4px; } QListView::item:alternate { background-color: #EEEEEE; } QListView {margin: 32px -1px -1px -1px;background-color:rgb(50,50,50);} ");
    //ui->folderList->setStyleSheet("padding: 28px 0 28px 0");
    ui->folderList->setItemDelegate(new FolderViewItemDelegate());

    QModelIndex index = sqlTableModel->index( row, 1 );
    if ( index.isValid() ) {
        //ui->folderList->selectionModel()->select( index, QItemSelectionModel::Select);
        ui->folderList->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
        cur_selection = index;
    } else {
        index = sqlTableModel->index(0, 1);
        //ui->folderList->selectionModel()->select( index, QItemSelectionModel::Select);
        ui->folderList->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
        cur_selection = index;
    }

    forwardIcon.addFile(":/icons/forward.svg", QSize(16, 16));
    forwardIcon.addFile(":/icons/forward.svg", QSize(24, 24));
    forwardIcon.addFile(":/icons/forward.svg", QSize(32, 32));

    backIcon.addFile(":/icons/back.svg", QSize(16, 16));
    backIcon.addFile(":/icons/back.svg", QSize(24, 24));
    backIcon.addFile(":/icons/back.svg", QSize(32, 32));
    ui->hideFoldersButton->setIcon(backIcon);
    ui->showFoldersButton->setIcon(forwardIcon);
    ui->showFoldersButton->setFlat(true);
    ui->hideFoldersButton->setFlat(true);

    QIcon addFolderIcon;
    addFolderIcon.addFile(":/icons/list-add.svg", QSize(16, 16));
    addFolderIcon.addFile(":/icons/list-add.svg", QSize(24, 24));
    addFolderIcon.addFile(":/icons/list-add.svg", QSize(32, 32));
    ui->addFolderButton->setIcon(addFolderIcon);
    ui->addFolderButton->setFlat(true);
        
    QIcon deleteFolderIcon;
    deleteFolderIcon.addFile(":/icons/list-remove.svg", QSize(16, 16));
    deleteFolderIcon.addFile(":/icons/list-remove.svg", QSize(24, 24));
    deleteFolderIcon.addFile(":/icons/list-remove.svg", QSize(32, 32));
    ui->deleteFolderButton->setIcon(deleteFolderIcon);
    ui->deleteFolderButton->setFlat(true);

    ui->folderList->hide();
    ui->horizontalLayout->setStretchFactor(ui->verticalLayout_2, 0);
    ui->hideFoldersButton->hide();
    ui->addFolderButton->hide();
    ui->deleteFolderButton->hide();
    ui->verticalLayout_2->removeItem(ui->horizontalLayout_3);

    //ui->folderList->setStyleSheet("QListView::item { border-bottom: 1px solid black;padding: 14px;}");
    //ui->folderList->setStyleSheet("QListView::item:alternate { background: lightblue; }");

    connect( ui->addFolderButton, SIGNAL(clicked()), this, SLOT(addFolder()) );
    connect( ui->hideFoldersButton, SIGNAL(clicked()), this, SLOT(hideFoderView()) );
    connect( ui->folderList, SIGNAL(clicked(const QModelIndex &)), this, SLOT(itemClicked(const QModelIndex &)) );
    connect( ui->folderList, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(itemClicked(const QModelIndex &)) );
    connect( ui->folderList->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(currentChanged(const QModelIndex &, const QModelIndex &)) );
            ((MainWindow *)ui->folderList->window())->statusBar()->addPermanentWidget(ui->showFoldersButton);
            ((MainWindow *)ui->folderList->window())->statusBar()->addPermanentWidget(ui->hideFoldersButton);
            ((MainWindow *)ui->folderList->window())->statusBar()->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
            ((MainWindow *)ui->folderList->window())->statusBar()->setSizeGripEnabled(false);
}

void FolderViewController:: showFoderView()
{
        //((MainWindow *)window())->statusBar()->clearMessage();
    QAction *viewAction = ((MainWindow *)ui->folderList->window())->findChild<QAction *>("viewAction");
    if ( ui->folderList->isVisible() ) {
        ui->folderList->hide();
        ui->hideFoldersButton->hide();
        ui->showFoldersButton->show();
        //ui->horizontalSpacer1->hide();
        //ui->horizontalSpacer1->invalidate();
        ui->addFolderButton->hide();
        ui->deleteFolderButton->hide();
        //ui->horizontalLayout->removeItem(ui->verticalLayout_2);
        ui->verticalLayout_2->removeItem(ui->horizontalLayout_3);
        ui->horizontalLayout->setStretchFactor(ui->verticalLayout_2, 0);
        ui->hideFoldersButton->setIcon(backIcon);
        if ( viewAction != nullptr ) {
            viewAction->setText(tr("Show Folders List"));
        }
    } else {
        //ui->horizontalLayout->insertItem(0, ui->verticalLayout_2);
        ui->verticalLayout_2->insertItem(1, ui->horizontalLayout_3);
        ui->folderList->show();
        //ui->horizontalSpacer1->show();
        ui->addFolderButton->show();
        ui->deleteFolderButton->show();
        ui->hideFoldersButton->show();
        ui->showFoldersButton->hide();
        ui->horizontalLayout->setStretchFactor(ui->verticalLayout_2, 2);
        ui->showFoldersButton->setIcon(forwardIcon);
        ui->folderList->setFocus();
        if ( cur_selection.isValid() ) {
            //qDebug() << "valid";
            //ui->folderList->selectionModel()->select( cur_selection, QItemSelectionModel::Select);
            //ui->folderList->selectionModel()->setCurrentIndex(cur_selection, QItemSelectionModel::ClearAndSelect);
            //qDebug() << ui->folderList->currentIndex();
        }
        if ( viewAction != nullptr ) {
            viewAction->setText(tr("Hide Folders List"));
        }
    }
}

void FolderViewController::hideFoderView()
{
    QAction *viewAction = ((MainWindow *)ui->folderList->window())->findChild<QAction *>("viewAction");
    ui->folderList->hide();
    ui->hideFoldersButton->hide();
    ui->showFoldersButton->show();
    //ui->horizontalSpacer1->hide();
    //ui->horizontalSpacer1->invalidate();
    ui->addFolderButton->hide();
    ui->deleteFolderButton->hide();
    //ui->horizontalLayout->removeItem(ui->verticalLayout_2);
    ui->verticalLayout_2->removeItem(ui->horizontalLayout_3);
    ui->horizontalLayout->setStretchFactor(ui->verticalLayout_2, 0);
    ui->hideFoldersButton->setIcon(backIcon);
    if ( viewAction != nullptr ) {
        viewAction->setText(tr("Show Folders List"));
    }
}

int FolderViewController::currentFolder()
{
    return sqlTableModel->data(sqlTableModel->index(cur_selection.row(), 0)).toInt();
}

void FolderViewController::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    //qDebug() << "current:" << current;
    //qDebug() << "previous:" << previous;
    emit folderChanged(sqlTableModel->data(sqlTableModel->index(current.row(), 0)).toInt());
}

void FolderViewController::itemClicked(const QModelIndex &index)
{
    cur_selection = index;
    QSettings settings;
    settings.setValue("selected_folder", cur_selection.row());
}

void FolderViewController::addFolder()
{
    QDateTime current = QDateTime::currentDateTime();
    int row = sqlTableModel->rowCount();
    sqlTableModel->insertRows(row, 1);
    sqlTableModel->setData(sqlTableModel->index(row, 1), QString("New Folder"));
    sqlTableModel->setData(sqlTableModel->index(row, 2), current.toString(Qt::ISODate));

    //cout << current.toString(Qt::ISODate).toStdString() << endl;
    qDebug() << row;

    bool flag = sqlTableModel->submitAll();
    if(flag==false)
      qDebug() << "Failed cannot save changes.";
}

void FolderViewController::applicationWillClose()
{
    if ( cur_selection.isValid() ) {
        QSettings settings;
        settings.setValue("selected_folder", cur_selection.row());
    }
}

FolderViewController::~FolderViewController()
{
}
