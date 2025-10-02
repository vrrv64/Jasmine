#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "notesviewcontroller.h"
#include "folderviewcontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showAbout();
    void showAboutQt();
private:
    Ui::MainWindow *ui;
    NotesViewController *nvc;
    FolderViewController *fvc;
    virtual void            closeEvent( QCloseEvent* );
    void setupData();
};
#endif // MAINWINDOW_H
