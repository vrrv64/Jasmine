#include "mainwindow.h"
#include "mypalette.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QLabel>
#include <QDebug>
#include <QDateTime>
#include <QSqlError>
#include <QSettings>
#include <QFontInfo>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QStandardPaths>
#include <QDialogButtonBox>

#include "version.h"

using namespace std;

QSqlError initDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("notes.db");
    //db.setDatabaseName(":memory:");

    if (!db.open())
        return db.lastError();

    return QSqlError();
}

void MainWindow::setupData()
{
    //qDebug() << QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    if ( ! QDir(dataLocation).exists() ) {
        if ( ! QDir().mkpath(dataLocation) ) {
            qDebug() << "Failed to create dir " << dataLocation;
        }
    }

    QString attLocation = QDir(dataLocation).filePath("attachments");
    if ( ! QDir(attLocation).exists() ) {
        QDir().mkdir(attLocation);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    QCoreApplication::setOrganizationName("jasmine");
    QCoreApplication::setApplicationName("Jasmine");

    ui->setupUi(this);
    setupData();

    QFont font("Verdana");
    //font.setPointSize(16);
    ui->textEdit->setFont(font);
    ui->textEdit->setCurrentFont(font);
    ui->notesList->setFont(font);
    //ui->textEdit->setStyleSheet("font: 16pt \"Tex Gyre Chorus\"");
    qDebug() << ui->textEdit->fontInfo().family();

    QSettings settings;
    this->restoreGeometry(settings.value("geometry").toByteArray());

    initDb();

    fvc = new FolderViewController();
    fvc->setup(ui);

    nvc = new NotesViewController();
    nvc->setup(ui, fvc->currentFolder());

    connect( ui->showFoldersButton, SIGNAL(clicked()), fvc, SLOT(showFoderView()) );
    connect( fvc, SIGNAL(folderChanged(int)), nvc, SLOT(folderChanged(int)));
    //connect( ui->hideFoldersButton, SIGNAL(clicked()), fvc, SLOT(hideFoderView()) );

    //this->menuBar()->setStyleSheet("QMenu::separator {background: white}");
    //QApplication::instance()->setStyleSheet("QMenu::separator {background: rgb(80, 80, 80);height: 1px;margin-top:4px;margin-bottom:4px;}");
    ui->notesList->setStyleSheet("QScrollBar:vertical { width: 6px; }");

    QMenu *fileMenu = new QMenu("&File", this);
    this->menuBar()->addMenu(fileMenu);
    QAction *newNoteAction = new QAction(tr("New Note"), this);
    newNoteAction->setShortcut(QKeySequence(tr("Ctrl+N")));
    connect(newNoteAction, SIGNAL(triggered()), nvc, SLOT(addNote()));
    fileMenu->addAction(newNoteAction);

    QAction *newFolderAction = new QAction(tr("New Folder"), this);
    newFolderAction->setShortcut(QKeySequence(tr("Ctrl+Shift+N")));
    connect(newFolderAction, SIGNAL(triggered()), fvc, SLOT(addFolder()));
    fileMenu->addAction(newFolderAction);
    //fileMenu->addAction( tr("&New Note"), nvc, SLOT(addNote()), QKeySequence(tr("Ctrl+N")));

    QAction *quitAction = new QAction(tr("Quit"), this);
    quitAction->setShortcut(QKeySequence(tr("Ctrl+Q")));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    fileMenu->addAction(quitAction);
    //fileMenu->addAction( tr("&Quit"), this, SLOT(close()), QKeySequence(tr("Ctrl+Q")) );

    QMenu *editMenu = new QMenu("&Edit", this);
    QAction *pasteUnformattedAction = new QAction(tr("Paste as Plain Text"), this);
    pasteUnformattedAction->setShortcut(QKeySequence(tr("Ctrl+Shift+V")));
    connect(pasteUnformattedAction, SIGNAL(triggered()), nvc, SLOT(pasteUnformattedText()));
    //editMenu->addAction( tr("Paste Unformatted Text"), nvc, SLOT(pasteUnformattedText()), QKeySequence("Ctrl+Shift+V") );
    editMenu->addAction(pasteUnformattedAction);
    this->menuBar()->addMenu(editMenu);

    QMenu *formatMenu = new QMenu("F&ormat", this);
    this->menuBar()->addMenu(formatMenu);
    nvc->setupFormatMenu(formatMenu);

    QMenu *viewMenu = new QMenu("&View", this);
    this->menuBar()->addMenu(viewMenu);
    QAction *viewAction = new QAction(tr("Show Folders List"), this);
    viewAction->setObjectName("viewAction");
    connect(viewAction, SIGNAL(triggered()), fvc, SLOT(showFoderView()));
    viewMenu->addAction(viewAction);

    QMenu *helpMenu = new QMenu("&Help", this);
    this->menuBar()->addMenu(helpMenu);
    QAction *aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));
    helpMenu->addAction(aboutAction);
    //helpMenu->addAction( tr("&About"), this, SLOT(showAbout()) );
    QAction *aboutQtAction = new QAction(tr("About Qt"), this);
    connect(aboutQtAction, SIGNAL(triggered()), this, SLOT(showAboutQt()));
    helpMenu->addAction(aboutQtAction);

    /*QLabel *tsLabel = new QLabel(this);
    tsLabel->setText("Tue 25 June 2024 11:40:02 AM");
    tsLabel->setAlignment(Qt::AlignTop | Qt::AlignRight);
    tsLabel->setStyleSheet(QString("QLabel { color : %1; }").arg(MyPalette::instance()->color("subtitle").name()));
    //this->menuBar()->setCornerWidget(tsLabel);
    this->statusBar()->addPermanentWidget(tsLabel);*/

    QIcon appIcon;
    appIcon.addFile(":/icons/note.png", QSize(512, 512));
    this->setWindowIcon(appIcon);

    //this->installEventFilter(this);
}

void MainWindow::showAbout()
{
    QString version = J_VERSION;
    QDateTime buildTime = QDateTime::fromString(QString(__DATE__) + " " + __TIME__, "MMM dd yyyy hh:mm:ss");
    //version += buildTime.toString("yyyyMMddThhmmss");
    //qDebug() << buildTime;

    QMessageBox msgAbout;
	const QString message = "<p style='text-align: center;margin-bottom:20px;'><img src=':/note.png' alt='' width='46' height='46'></p>"
	                        "<p style='text-align: center;'><strong>Jasmine</strong></p>"
	                        "<p style='text-align: center;'>" + version + "</p>"
	                        "<p style='text-align: center;'>" + buildTime.toString("yyyyMMddThhmmss") + "</p>"
	                        "<p style='text-align: center;'>Copyright &copy; 2025.</p>";
	
	msgAbout.setInformativeText(message);
	msgAbout.setWindowTitle("About");
    QGridLayout *lay = msgAbout.findChild<QGridLayout *>();
    QMargins margins = lay->contentsMargins();
    margins.setRight(20);
    //margins.setLeft(60);
    lay->setContentsMargins(margins);

    QList<QObject*> list = msgAbout.findChildren<QObject*>();
    foreach (auto obj, list) {
        QDialogButtonBox *qdbb = qobject_cast<QDialogButtonBox*>(obj);
        //qDebug() << obj;
        if ( qdbb ) {
            qdbb->hide();
            break;
        }
    }

	msgAbout.exec();
}

void MainWindow::showAboutQt()
{
    QMessageBox::aboutQt(this);
}

void MainWindow:: closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    //settings.setValue("windowState", saveState());
    nvc->applicationWillClose();
    fvc->applicationWillClose();
    QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
    delete nvc;
    delete fvc;
    delete ui;
}
