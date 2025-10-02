#ifndef NOTESVIEWCONTROLLER_H
#define NOTESVIEWCONTROLLER_H

#include <QListView>
#include <QPushButton>
#include <QSqlTableModel>
#include <QTextCharFormat>
#include "mysortfilterproxymodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class NotesViewController: public QObject
{
    Q_OBJECT
public:
    NotesViewController();
    void setup(Ui::MainWindow *, int);
    void folderSelected(int id);
    void applicationWillClose();
    ~NotesViewController();

public slots:
    void addNote();
    void pasteUnformattedText();
    void setupFormatMenu(QMenu*);
    void toggleFontBold(bool);
    void toggleFontStrikeThrough(bool strikeOut);
    void folderChanged(int);

private slots:
    void search(const QString &arg1);
    void updateModified(int row, QSqlRecord &record);
    void deleteNote();
    void itemClicked(const QModelIndex&);
    void textChanged();
    void textFormatChanged(const QTextCharFormat&);
    void textAlign(QAction *a);
    void textColor();
    void textBgColor();
    void textFont();
    void fontChanged(const QFont&);
    void alignmentChanged();
    void textColorChanged(const QColor&);
    void textBgColorChanged(const QColor&);
    void textSelectionChanged();
    void clearFormats();
    void resizeImage(float, bool);
    void resizeText(float, bool);
    void resetSize();
    void enlarge();
    void shrink();
    void createLinks();
    void selectionChanged(const QItemSelection &, const QItemSelection &);
    void currentChanged(const QModelIndex &, const QModelIndex &);

private:
    int folder_id;
    QSqlTableModel *sqlTableModel;
    MySortFilterProxyModel *proxyModel;
    Ui::MainWindow *ui;
    //QModelIndex cur_selection;
    QMap<int, QModelIndex> selectedNote;
    QAction *editBoldAction, *editItalicAction, *editUnderlineAction, *editStrikeThroughAction;
    QAction *leftAlignAction, *rightAlignAction, *centerAlignAction, *justifyFillAction;
    QAction *textColorAction, *textBgColorAction, *clearFormatsAction;
    QAction *enlargeAction, *shrinkAction, *resetSizeAction, *showFontsAction, *createLinksAction;
    void selectNote(const QModelIndex& index);
    void emptySelection();
    QString realCreateLinks();
    QPushButton *msgButton;
};

#endif // NOTESVIEWCONTROLLER_H
