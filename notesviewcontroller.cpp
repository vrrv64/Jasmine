#include "notesviewcontroller.h"
#include "listviewdelegate.h"
#include "mypalette.h"
//#include <iostream>
#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include <QTextBlock>
#include <QClipboard>
#include <QSqlRecord>
#include <QMainWindow>
#include <QMessageBox>
#include <QFontDialog>
#include <QColorDialog>
#include <QActionGroup>
#include <QSqlTableModel>
#include <QSqlError>
#include <QTextDocumentFragment>
#include <QGraphicsDropShadowEffect>
#include "ui_mainwindow.h"

//using namespace std;

NotesViewController::NotesViewController()
{
}

void NotesViewController::setup(Ui::MainWindow *ui1, int folder_id)
{
    QSettings settings;

    this->ui = ui1;
    this->folder_id = folder_id;

    QListView *notesList = ui->notesList;
    notesList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    sqlTableModel = new QSqlTableModel();
    sqlTableModel->setTable("notes");
    sqlTableModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    sqlTableModel->setSort(1, Qt::DescendingOrder);
    sqlTableModel->setFilter(QString("folder_id = %1").arg(folder_id));
    sqlTableModel->select();

    proxyModel = new MySortFilterProxyModel(notesList);

    proxyModel->setSourceModel(sqlTableModel);

    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(0);

    proxyModel->sort(1, Qt::DescendingOrder);
    //proxyModel->setFilterRole(Qt::UserRole);

    notesList->setModel(proxyModel);
    notesList->setSpacing(1);
    //qDebug() << notesList->selectionMode();
    //notesList->setWordWrap(true);
    //ui->notesList->setStyleSheet("QListView::item { border-bottom: 1px solid black;padding: 14px;}");
    //ui->notesList->setStyleSheet("QListView::item:alternate { background: lightblue; }");
    ui->notesList->setStyleSheet("QListView {background-color:rgb(50,50,50);} ");

    QString textColor = MyPalette::instance()->color("subtitle").name();
    QString textShadowColor = MyPalette::instance()->color("text-shadow").name();
    msgButton = new QPushButton(QString("➕ ") + tr("Add New Note"), ui->textEdit);
    //msgButton = new QPushButton(QString("➕ ") + tr("Add New Note"), ui->textEdit);
    msgButton->setObjectName("msgButton");
    QPalette palette = msgButton->palette();
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect( );
    effect->setBlurRadius( 2 );
    effect->setColor(textShadowColor);
    effect->setOffset(1, 1);
    msgButton->setGraphicsEffect( effect );
    msgButton->setFlat(true);
    QString style = QString("QPushButton#msgButton {color: %1;}").arg(textColor);
    style = style + QString(" QPushButton#msgButton::pressed {background-color: %1;}").arg(palette.color(QPalette::Active, QPalette::Base).name());
    msgButton->setStyleSheet(style);
    msgButton->setBackgroundRole(QPalette::NoRole);
    msgButton->setAttribute(Qt::WA_TranslucentBackground);
    //msgButton->setFont(QFont(msgButton->font().families(), 16, QFont::Bold));
    msgButton->setFont(QFont("Helvetica [Cronyx]", 16, QFont::Bold));
    msgButton->hide();

    QIcon searchIcon;
    searchIcon.addFile(":/icons/search.svg", QSize(16, 16));
    searchIcon.addFile(":/icons/search.svg", QSize(24, 24));
    searchIcon.addFile(":/icons/search.svg", QSize(32, 32));
    ui->lineEdit->addAction(searchIcon, QLineEdit::LeadingPosition);

    QIcon addNoteIcon;
    addNoteIcon.addFile(":/icons/list-add.svg", QSize(16, 16));
    addNoteIcon.addFile(":/icons/list-add.svg", QSize(24, 24));
    addNoteIcon.addFile(":/icons/list-add.svg", QSize(32, 32));
    ui->addNoteButton->setIcon(addNoteIcon);
    ui->addNoteButton->setFlat(true);

    QIcon deleteNoteIcon;
    deleteNoteIcon.addFile(":/icons/list-remove.svg", QSize(16, 16));
    deleteNoteIcon.addFile(":/icons/list-remove.svg", QSize(24, 24));
    deleteNoteIcon.addFile(":/icons/list-remove.svg", QSize(32, 32));
    ui->deleteNoteButton->setIcon(deleteNoteIcon);
    ui->deleteNoteButton->setFlat(true);
    ui->deleteNoteButton->setStyleSheet("margin-right: -10px;");

    enlargeAction = new QAction( tr("Enlarge"), this );
    enlargeAction->setShortcut( QKeySequence(tr("Ctrl+=")) );
    ui->textEdit->addAction(enlargeAction);
    connect( enlargeAction, SIGNAL(triggered()), this, SLOT(enlarge()) );

    shrinkAction = new QAction( tr("Shrink"), this );
    shrinkAction->setShortcut( QKeySequence(tr("Ctrl+-")) );
    ui->textEdit->addAction(shrinkAction);
    connect( shrinkAction, SIGNAL(triggered()), this, SLOT(shrink()) );

    resetSizeAction = new QAction( tr("Reset"), this );
    resetSizeAction->setShortcut( QKeySequence(tr("Ctrl+0")) );
    ui->textEdit->addAction(resetSizeAction);
    connect( resetSizeAction, SIGNAL(triggered()), this, SLOT(resetSize()) );

    //msgButton->setIcon(addNoteIcon);
    //msgButton->repaint();

    ui->notesList->setItemDelegate(new ListViewDelegate());

    int row = settings.value(QString("selected_note/%1").arg(this->folder_id)).toInt();

    QModelIndex index = proxyModel->mapFromSource(sqlTableModel->index( row, 0 ));

    selectNote(index);

    //textFormatChanged(ui->textEdit->currentCharFormat());

    /*if ( ! index.isValid() ) {
        index = proxyModel->mapFromSource(sqlTableModel->index( 0, 0 ));
    }

    if ( index.isValid() ) {
        //ui->notesList->selectionModel()->select( index, QItemSelectionModel::Select);
        ui->notesList->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
        ui->textEdit->setHtml(sqlTableModel->data(sqlTableModel->index(row, 3)).toString());
        ui->textEdit->window()->setWindowTitle(sqlTableModel->data(sqlTableModel->index(row, 0)).toString());
        //ui->notesList->setCurrentIndex(index);
        //ui->textEdit->setFocus();
        //cur_selection = proxyModel->mapToSource(index);
        selectedNote[this->folder_id] = proxyModel->mapToSource(index);
    } else if ( sqlTableModel->rowCount() == 0 ) {
        emptySelection();
    }*/

    ui->notesList->setFocus();
    //ui->statusbar->setFocus();

    ui->textEdit->setAcceptRichText(true);

    connect( sqlTableModel, SIGNAL(beforeUpdate(int, QSqlRecord&)), this, SLOT(updateModified(int, QSqlRecord&)) );
    connect( ui->addNoteButton, SIGNAL(clicked()), this, SLOT(addNote()) );
    connect( msgButton, SIGNAL(clicked()), this, SLOT(addNote()) );
    connect( ui->deleteNoteButton, SIGNAL(clicked()), this, SLOT(deleteNote()) );
    connect( ui->lineEdit, SIGNAL(textChanged(const QString& )), this, SLOT(search(const QString&)) );
    connect( ui->notesList->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(selectionChanged(const QItemSelection &, const QItemSelection &)) );
    connect( ui->notesList->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(currentChanged(const QModelIndex &, const QModelIndex &)) );
    //connect( ui->notesList, SIGNAL(clicked(const QModelIndex &)), this, SLOT(itemClicked(const QModelIndex &)) );
    //connect( ui->notesList, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(itemClicked(const QModelIndex &)) );
    connect( ui->textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()) );
    connect( ui->textEdit, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)), this, SLOT(textFormatChanged(const QTextCharFormat &)) );
    connect( ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(alignmentChanged()) );
    //ui->textEdit->setTextInteractionFlags(Qt::TextBrowserInteraction|Qt::TextEditorInteraction);
}

void NotesViewController::folderSelected(int folder_id)
{
    this->folder_id = folder_id;
    sqlTableModel->setFilter(QString("folder_id = %1").arg(folder_id));
    sqlTableModel->select();
}

void NotesViewController::updateModified(int row, QSqlRecord &record) {

    Q_UNUSED(row);

    QDateTime current = QDateTime::currentDateTime();

    record.setValue(1, current.toString(Qt::ISODate));
    record.setGenerated(1, true);

    ui->textEdit->window()->setWindowTitle(record.value(0).toString());
}

void NotesViewController::createLinks()
{
    QModelIndex cur_selection = selectedNote[this->folder_id];
    if ( cur_selection.isValid() ) {
        QString status = realCreateLinks();
        textChanged();
        QMessageBox::information(ui->notesList, "Format Hyperlinks", status);
    } else {
        QMessageBox::information(ui->notesList, "Format Hyperlinks", "No note selected");
    }
}

QString NotesViewController::realCreateLinks()
{
    bool oldState = ui->textEdit->blockSignals(true);
    QRegularExpression regexp("https?://\\S+");
    QTextCursor cursor = ui->textEdit->textCursor();
    int savePos = cursor.position();
    ui->textEdit->moveCursor(QTextCursor::Start);
    bool found = ui->textEdit->find(regexp);
    //qDebug() << found;
    int nNew = 0, nMod = 0;
    while ( found ) {
        cursor = ui->textEdit->textCursor();
        QTextCharFormat fmt = cursor.charFormat();
        QString url = cursor.selection().toPlainText();
        if ( ! fmt.isAnchor() ) {
            //qDebug() << url;
            fmt.setAnchor(true);
            fmt.setAnchorHref(url);
            //fmt.setFontUnderline(true);
            cursor.removeSelectedText();
            cursor.insertText(url, fmt);
            //ui->textEdit->mergeCurrentCharFormat(fmt);
            nNew++;
        } else if ( QString::compare(url, fmt.anchorHref()) != 0 ) {
            fmt.setAnchorHref(url);
            cursor.removeSelectedText();
            cursor.insertText(url, fmt);
            nMod++;
        } else {
            //QString url = cursor.selection().toPlainText();
            //qDebug() << "already anchor! " << url << ", " << fmt.anchorHref();
        }
        found = ui->textEdit->find(regexp);
        //qDebug() << found;
    }
    cursor.setPosition(savePos);
    ui->textEdit->setTextCursor(cursor);
    ui->textEdit->blockSignals(oldState);
    QString out = QString("New: %1, Modified:%2").arg(nNew).arg(nMod);
    return out;
}

void NotesViewController::textChanged()
{
    MySortFilterProxyModel *proxyModel = (MySortFilterProxyModel *)ui->notesList->model();
    QSqlTableModel *model = (QSqlTableModel*)proxyModel->sourceModel();
    QDateTime current = QDateTime::currentDateTime();
    QModelIndex cur_selection = selectedNote[this->folder_id];
    if ( cur_selection.isValid() ) {
        int row = cur_selection.row();
        if ( model->data(model->index(row, 3)).toString() != ui->textEdit->toHtml() ) {
            //createLinks();
            //qDebug() << "textChanged: " << ( model->data(model->index(row, 3)).toString() != ui->textEdit->toHtml() );
            model->setData(model->index(row, 3), ui->textEdit->toHtml());
            bool flag = model->submitAll();
            if(flag==false)
                qDebug() << "textChanged: Failed cannot save changes.";
        }
    }
}

void NotesViewController::textFormatChanged(const QTextCharFormat& f)
{
    if ( !editBoldAction || !editUnderlineAction || !editItalicAction || !editStrikeThroughAction ) {
        return;
    }

    //qDebug() << f.fontWeight() << ", " << (QFont::Bold == f.fontWeight());
    editBoldAction->setChecked( f.font().bold() );
    editItalicAction->setChecked( f.fontItalic() );
    editUnderlineAction->setChecked( f.fontUnderline() );
    editStrikeThroughAction->setChecked( f.fontStrikeOut() );

    textColorChanged(f.foreground().color());
    textBgColorChanged(f.background().color());

    /*int fontSize = f.font().pointSize();
    QString unit = "pt";
    if ( fontSize < 0 ) {
        fontSize = f.font().pixelSize();
        unit = "px";
    }
    showFontsAction->setText( QString("Font...    [ %1 - %2%3 ]").arg(f.font().family()).arg(fontSize).arg(unit));*/
    fontChanged(f.font());
}

void NotesViewController::alignmentChanged()
{
    Qt::Alignment a = ui->textEdit->alignment();

    switch (a)
    {
        case Qt::AlignHCenter:
            centerAlignAction->setChecked( true );
            break;
        case Qt::AlignRight:
            rightAlignAction->setChecked( true );
            break;
        case Qt::AlignJustify:
            justifyFillAction->setChecked( true );
            break;
        case Qt::AlignLeft:
        default:
            leftAlignAction->setChecked( true );
            break;
    }
}

void NotesViewController::fontChanged(const QFont& f)
{
    int fontSize = f.pointSize();
    QString unit = "pt";
    if ( fontSize < 0 ) {
        fontSize = f.pixelSize();
        unit = "px";
    }
    showFontsAction->setText( QString("Font...    [ %1 - %2%3 ]").arg(f.family()).arg(fontSize).arg(unit));
}

void NotesViewController::textColorChanged(const QColor& col)
{
    QPixmap pix(24, 24);
    pix.fill(col);
    textColorAction->setIcon(QIcon(pix));
}

void NotesViewController::textBgColorChanged(const QColor& col)
{
    QPixmap pix(24, 24);
    pix.fill(col);
    textBgColorAction->setIcon(QIcon(pix));
}

void NotesViewController::textSelectionChanged()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if ( cursor.hasSelection() ) {
        clearFormatsAction->setEnabled(true);
    } else {
        clearFormatsAction->setEnabled(false);
    }
}

void NotesViewController::resizeText(float factor, bool reset=false)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if ( cursor.hasSelection() ) {
        int start = cursor.selectionStart();
        int end = cursor.selectionEnd();
	    QTextCharFormat fmt = cursor.charFormat();
	    QFont f = fmt.font();
	    if ( f.pointSize() > 0  ) {
	        f.setPointSize(f.pointSize() + factor);
	    } else if ( f.pixelSize() > 0 ) {
	        f.setPixelSize(f.pixelSize() + factor);
	    }
	    fmt.setFont(f);
	    cursor.setCharFormat(fmt);
	    //ui->textEdit->mergeCurrentCharFormat(fmt);

        cursor.setPosition(start);
        cursor.setPosition(end, QTextCursor::KeepAnchor);
        ui->textEdit->setTextCursor(cursor);
    }
}

void NotesViewController::resizeImage(float factor, bool reset=false)
{
    QTextBlock currentBlock = ui->textEdit->textCursor().block();
    QTextBlock::iterator it;

    for (it = currentBlock.begin(); !(it.atEnd()); ++it)
    {
        QTextFragment fragment = it.fragment();
        if (fragment.isValid())
        {
            if(fragment.charFormat().isImageFormat ())
            {
                QTextImageFormat newImageFormat = fragment.charFormat().toImageFormat();
                //qDebug() << newImageFormat.quality() << ", " << newImageFormat.name() << ", " << newImageFormat.width() << ", " << newImageFormat.height();
                QImage img(newImageFormat.name());

                if ( ! img.isNull() ) {

                    int orig_w = img.width();
                    int cur_w = newImageFormat.width();

                    if ( cur_w == 0 ) {
                        cur_w = orig_w;
                    }

                    if ( reset ) {
                        cur_w = orig_w;
                    } else {
                        cur_w = cur_w * ( 1 + factor);
                    }

                    newImageFormat.setWidth(cur_w);
                    //qDebug() << cur_w << "\n";
                    if ( newImageFormat.isValid() ) {
                        QTextCursor cursor = ui->textEdit->textCursor();
                        cursor.setPosition(fragment.position());
                        cursor.setPosition(fragment.position() + fragment.length(), QTextCursor::KeepAnchor);
                        cursor.setCharFormat(newImageFormat);
                    }
                }
            }
        }
    }
}

void NotesViewController::search(const QString &arg1)
{
    //QSortFilterProxyModel *proxyModel = (QSortFilterProxyModel *)ui->notesList->model();

    proxyModel->setFilterFixedString(arg1);

    //ui->notesList->clearSelection();

    /*if ( cur_selection.isValid() ) {
        QModelIndex qmi = proxyModel->mapFromSource(cur_selection);
        if ( qmi.isValid() ) {
            //ui->notesList->selectionModel()->select(qmi, QItemSelectionModel::Select);
            //ui->notesList->setCurrentIndex(qmi);
            ui->notesList->selectionModel()->clearCurrentIndex();
            ui->notesList->selectionModel()->setCurrentIndex(qmi, QItemSelectionModel::Select);
        }
    }*/
    //qDebug() << proxyModel;
    //qDebug() << ui->notesList->selectionModel();
}

void NotesViewController::selectNote(const QModelIndex& index1)
{
    QModelIndex index = index1;
    //qDebug() << index << ", " << index1;
    if ( ! index.isValid() ) {
        index = proxyModel->mapFromSource(sqlTableModel->index( 0, 0 ));
    }

    if ( index.isValid() ) {
        if ( ui->notesList->selectionModel()->currentIndex().row() != index.row() ) {
            ui->notesList->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
        }
        QModelIndex cur_selection = proxyModel->mapToSource(index);
        ui->textEdit->setReadOnly(false);
        bool oldState = ui->textEdit->blockSignals(true);
        ui->textEdit->setHtml(sqlTableModel->data(sqlTableModel->index(cur_selection.row(), 3)).toString());
        ui->textEdit->blockSignals(oldState);
        ui->textEdit->window()->setWindowTitle(sqlTableModel->data(sqlTableModel->index(cur_selection.row(), 0)).toString());
        selectedNote[this->folder_id] = cur_selection;
        msgButton->hide();
        ui->textEdit->setTextInteractionFlags(Qt::TextBrowserInteraction|Qt::TextEditorInteraction);
    } else {
        emptySelection();
    }
}

void NotesViewController::itemClicked(const QModelIndex &index)
{
    //QSortFilterProxyModel *proxyModel = (QSortFilterProxyModel *)ui->notesList->model();
    QSqlTableModel *model = (QSqlTableModel*)proxyModel->sourceModel();
    //cur_selection = proxyModel->mapToSource(index);
    QModelIndex cur_selection = proxyModel->mapToSource(index);
    //ui->textEdit->setDisabled(false);
    ui->textEdit->setReadOnly(false);
    bool oldState = ui->textEdit->blockSignals(true);
    ui->textEdit->setHtml(model->data(model->index(cur_selection.row(), 3)).toString());
    ui->textEdit->blockSignals(oldState);
    //cout << "sel changed:: " << cur_selection.row() << ", "<< proxyModel->mapFromSource(cur_selection).row() << endl;
    QSettings settings;
    settings.setValue("selected_note", cur_selection.row());
    //cout << ui->textEdit->document()->firstBlock().text().toStdString() << endl;
    ui->textEdit->window()->setWindowTitle(model->data(model->index(cur_selection.row(), 0)).toString());
}

void NotesViewController::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    //qDebug() << "selected: " << selected;
    //qDebug() << "deselected: " << deselected;
    if ( selected.indexes().length() > 0 ) {
        selectNote(selected.indexes()[0]);
    }
}

void NotesViewController::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    //qDebug() << "current:" << current;
    //qDebug() << "previous:" << previous;
}

void NotesViewController::folderChanged(int new_folder_id)
{
    sqlTableModel->setFilter(QString("folder_id = %1").arg(new_folder_id));
    sqlTableModel->select();
    /*proxyModel->setSourceModel(sqlTableModel);
    if ( proxyModel->rowCount() == 0 ) {
        emptySelection();
    } else {
        msgButton->hide();
    }*/

    QModelIndex index;
    this->folder_id = new_folder_id;
    if ( selectedNote.contains(new_folder_id) ) {
        index = proxyModel->mapFromSource(sqlTableModel->index(selectedNote[new_folder_id].row(), 0));
    } else {
        QSettings settings;
        int row = settings.value(QString("selected_note/%1").arg(this->folder_id)).toInt();
        index = proxyModel->mapFromSource(sqlTableModel->index( row, 0 ));
    }

    selectNote(index);
    ui->notesList->setFocus();
}

void NotesViewController::addNote()
{
    MySortFilterProxyModel *proxyModel = (MySortFilterProxyModel *)ui->notesList->model();
    QSqlTableModel *model = (QSqlTableModel*)proxyModel->sourceModel();
    QDateTime current = QDateTime::currentDateTime();
    int row = 0;
    model->insertRows(row, 1);
    model->setData(model->index(row, 0), QString("New Note"));
    //model->setData(model->index(row, 1), current.toString("MM/dd/yy hh:mm AP"));
    model->setData(model->index(row, 1), current.toString(Qt::ISODate));
    model->setData(model->index(row, 2), current.toString(Qt::ISODate));
    model->setData(model->index(row, 3), "");
    model->setData(model->index(row, 4), this->folder_id);

    //cout << current.toString(Qt::ISODate).toStdString() << endl;

    bool flag = model->submitAll();
    if(flag==false)
      qDebug() << "Failed cannot save changes.";

    row = 0;
	QModelIndex index = proxyModel->index( row, 0 );
    selectNote(index);
	/*if ( index.isValid() ) {
        //ui->notesList->clearSelection();
	    //ui->notesList->selectionModel()->select( index, QItemSelectionModel::Select);
        ui->notesList->selectionModel()->setCurrentIndex( index, QItemSelectionModel::ClearAndSelect);
        bool oldState = ui->textEdit->blockSignals(true);
	    ui->textEdit->setHtml("");
        ui->textEdit->blockSignals(oldState);
	    ui->textEdit->setFocus();
	    //ui->notesList->setCurrentIndex(index);
	    cur_selection = proxyModel->mapToSource(index);
        ui->textEdit->window()->setWindowTitle(proxyModel->data(index).toString());
	}*/
    msgButton->hide();
}

void NotesViewController::deleteNote()
{
    MySortFilterProxyModel *proxyModel = (MySortFilterProxyModel *)ui->notesList->model();
    QSqlTableModel *model = (QSqlTableModel *)proxyModel->sourceModel();
    QModelIndexList indexes = ui->notesList->selectionModel()->selectedIndexes();

    if (indexes.isEmpty()) {
        QMessageBox::information(ui->notesList, "Delete selected note", "No note selected");
        return;
    }

    QString msg = indexes.size() == 1
        ? QString("The selected note \"%1\" will be permanently deleted. Do you wish to continue?").arg(proxyModel->data(indexes[0]).toString())
        : QString("%1 notes will be permanently deleted. Do you wish to continue?").arg(indexes.size());
    QMessageBox::StandardButton reply = QMessageBox::warning(ui->notesList, "Delete selected notes", msg, QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return;
    }

    // Sort rows in descending order to avoid index shifting
    QList<int> sourceRows;
    for (const QModelIndex& index : indexes) {
        sourceRows.append(proxyModel->mapToSource(index).row());
    }
    std::sort(sourceRows.begin(), sourceRows.end(), std::greater<int>());

    // Delete rows
    for (int row : sourceRows) {
        model->removeRow(row);
    }

    // Submit changes and handle errors
    if (!model->submitAll()) {
        qDebug() << "Failed to delete note: " << model->lastError().text();
        QMessageBox::critical(ui->notesList, "Error", "Failed to delete note: " + model->lastError().text());
        model->revertAll();
        return;
    }
    model->select();

    // Select next note or clear selection
    QSettings settings;
    if (proxyModel->rowCount() == 0) {
        emptySelection();
        settings.remove(QString("selected_note/%1").arg(folder_id));
    } else {
        int row = -1;
        int maxRow = indexes[indexes.size() - 1].row();
        if (maxRow < proxyModel->rowCount()) {
            row = maxRow; // Select next row
        } else if (maxRow > 0) {
            row = maxRow - 1; // Select previous row
        }
        QModelIndex index = proxyModel->index(row, 0);
        if (index.isValid()) {
            selectNote(index);
            settings.setValue(QString("selected_note/%1").arg(folder_id), proxyModel->mapToSource(index).row());
        } else {
            emptySelection();
            settings.remove(QString("selected_note/%1").arg(folder_id));
        }
    }
}

void NotesViewController::emptySelection()
{
    bool oldState = ui->textEdit->blockSignals(true);
    ui->textEdit->setHtml("");
    if ( sqlTableModel->rowCount() == 0 ) {
        QString textColor = MyPalette::instance()->color("subtitle").name();
        msgButton->show();
    } else {
        msgButton->hide();
    }
    ui->textEdit->setReadOnly(true);
    ui->textEdit->blockSignals(oldState);
    ui->textEdit->window()->setWindowTitle("Jasmine");
}

void NotesViewController::pasteUnformattedText()
{
    QString subtype = "html";
    QString html = QApplication::clipboard()->text(subtype);
    QString text = QTextDocumentFragment::fromHtml(html).toPlainText();
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat fmt = QTextCharFormat();
    cursor.insertText(text, fmt);
    ui->textEdit->setTextCursor(cursor);
}

void NotesViewController::setupFormatMenu(QMenu *formatMenu)
{
    QString separatorColor = MyPalette::instance()->color("separator").name();
    formatMenu->setStyleSheet(QString("QMenu::separator {background: %1;height: 1px;margin-top:4px;margin-bottom:4px;}").arg(separatorColor));

    QFont f;
    QIcon textBoldIcon;
    textBoldIcon.addFile(":/icons/text-bold.svg", QSize(24, 24));
    editBoldAction = new QAction( textBoldIcon, tr("Bold"), this );
    f = editBoldAction->font(); f.setBold( true ); editBoldAction->setFont( f );
    editBoldAction->setShortcut( QKeySequence(tr("Ctrl+B")) );
    editBoldAction->setCheckable( true );
    connect( editBoldAction, SIGNAL(triggered(bool)), this, SLOT(toggleFontBold(bool)) );
    formatMenu->addAction(editBoldAction);

    QIcon textItalicIcon;
    textItalicIcon.addFile(":/icons/text-italic.svg", QSize(24, 24));
    editItalicAction = new QAction( textItalicIcon, tr("Italic"), this );
    f = editItalicAction->font(); f.setItalic( true ); editItalicAction->setFont( f );
    editItalicAction->setShortcut( QKeySequence(tr("Ctrl+I")) );
    editItalicAction->setCheckable( true );
    connect( editItalicAction, SIGNAL(triggered(bool)), ui->textEdit, SLOT(setFontItalic(bool)) );
    formatMenu->addAction(editItalicAction);

    QIcon textUnderlineIcon;
    textUnderlineIcon.addFile(":/icons/text-underline.svg", QSize(24, 24));
    editUnderlineAction = new QAction( textUnderlineIcon, tr("Underline"), this );
    f = editUnderlineAction->font(); f.setUnderline( true ); editUnderlineAction->setFont( f );
    editUnderlineAction->setShortcut( QKeySequence(tr("Ctrl+U")) );
    editUnderlineAction->setCheckable( true );
    connect( editUnderlineAction, SIGNAL(triggered(bool)), ui->textEdit, SLOT(setFontUnderline(bool)) );
    formatMenu->addAction(editUnderlineAction);

    QIcon textStrikeoutIcon;
    textStrikeoutIcon.addFile(":/icons/text-strikethrough.svg", QSize(24, 24));
    editStrikeThroughAction = new QAction( textStrikeoutIcon, tr("Strikethrough"), this );
    f = editStrikeThroughAction->font(); f.setStrikeOut( true ); editStrikeThroughAction->setFont( f );
    editStrikeThroughAction->setShortcut( QKeySequence(tr("Ctrl+T")) );
    editStrikeThroughAction->setCheckable( true );
    connect( editStrikeThroughAction, SIGNAL(triggered(bool)), this, SLOT(toggleFontStrikeThrough(bool)) );
    formatMenu->addAction(editStrikeThroughAction);

    formatMenu->addSeparator();

    QActionGroup *alignActionGroup = new QActionGroup(this);

    QIcon leftAlignIcon;
    leftAlignIcon.addFile(":/icons/justify-left.svg", QSize(24, 24));
    leftAlignAction = new QAction( leftAlignIcon, tr("Left"), alignActionGroup );
    leftAlignAction->setShortcut( QKeySequence(tr("Ctrl+L")) );
    leftAlignAction->setCheckable( true );
    formatMenu->addAction(leftAlignAction);

    QIcon centerAlignIcon;
    centerAlignIcon.addFile(":/icons/justify-center.svg", QSize(24, 24));
    centerAlignAction = new QAction( centerAlignIcon, tr("Center"), alignActionGroup );
    centerAlignAction->setShortcut( QKeySequence(tr("Ctrl+E")) );
    centerAlignAction->setCheckable( true );
    formatMenu->addAction(centerAlignAction);

    QIcon rightAlignIcon;
    rightAlignIcon.addFile(":/icons/justify-right.svg", QSize(24, 24));
    rightAlignAction = new QAction( rightAlignIcon, tr("Right"), alignActionGroup );
    rightAlignAction->setShortcut( QKeySequence(tr("Ctrl+R")) );
    rightAlignAction->setCheckable( true );
    formatMenu->addAction(rightAlignAction);

    QIcon justifyFillIcon;
    justifyFillIcon.addFile(":/icons/justify-fill.svg", QSize(24, 24));
    justifyFillAction = new QAction( justifyFillIcon, tr("Justify"), alignActionGroup );
    justifyFillAction->setShortcut( QKeySequence(tr("Ctrl+J")) );
    justifyFillAction->setCheckable( true );
    formatMenu->addAction(justifyFillAction);

    connect( alignActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(textAlign(QAction*)) );

    formatMenu->addSeparator();

    //qDebug() << ui->textEdit->textColor();
    //qDebug() << ui->textEdit->textBackgroundColor();

    showFontsAction = new QAction( tr("Font..."), this );
    formatMenu->addAction(showFontsAction);
    connect( showFontsAction , SIGNAL(triggered()), this, SLOT(textFont()) );

    QPixmap pix(24, 24);
    pix.fill(ui->textEdit->textColor());
    QIcon textColorIcon = QIcon(pix);
    textColorAction = new QAction( textColorIcon, tr("Color..."), this );
    formatMenu->addAction(textColorAction);
    connect( textColorAction, SIGNAL(triggered()), this, SLOT(textColor()) );

    pix.fill(ui->textEdit->textBackgroundColor());
    QIcon textBgColorIcon = QIcon(pix);
    textBgColorAction = new QAction( textBgColorIcon, tr("Background color..."), this );
    formatMenu->addAction(textBgColorAction);
    connect( textBgColorAction, SIGNAL(triggered()), this, SLOT(textBgColor()) );

    formatMenu->addSeparator();

    createLinksAction = new QAction( tr("Hyperlinks..."), this );
    formatMenu->addAction(createLinksAction);
    createLinksAction->setShortcut( QKeySequence(tr("Ctrl+K")) );
    connect( createLinksAction, SIGNAL(triggered()), this, SLOT(createLinks()) );

    formatMenu->addSeparator();

    QIcon clearFormatsIcon;
    clearFormatsIcon.addFile(":/icons/clear.svg", QSize(24, 24));
    clearFormatsAction = new QAction( clearFormatsIcon, tr("Clear all for&matting"), this );
    clearFormatsAction->setEnabled(false);
    clearFormatsAction->setShortcut( QKeySequence(tr("Ctrl+M")) );
    formatMenu->addAction(clearFormatsAction);
    connect( clearFormatsAction, SIGNAL(triggered()), this, SLOT(clearFormats()) );
    //connect( ui->textEdit, SIGNAL(copyAvailable(bool)), clearFormatsAction, SLOT(setEnabled(bool)) );
    connect( ui->textEdit, SIGNAL(selectionChanged()), this, SLOT(textSelectionChanged()) );

    formatMenu->addSeparator();
}

void NotesViewController::toggleFontBold(bool bold)
{
    QFont::Weight newWeight = ( bold ? QFont::Bold : QFont::Normal);
    ui->textEdit->setFontWeight( newWeight );
}

void NotesViewController::toggleFontStrikeThrough(bool strikeOut)
{
    QTextCharFormat fmt = QTextCharFormat();
    fmt.setFontStrikeOut(strikeOut);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}

void NotesViewController::textAlign(QAction *a)
{
    qDebug() << "textAlign";
    if ( a == leftAlignAction ) {
        ui->textEdit->setAlignment(Qt::AlignLeft);
    } else if ( a == centerAlignAction ) {
        ui->textEdit->setAlignment(Qt::AlignHCenter);
    } else if ( a == rightAlignAction ) {
        ui->textEdit->setAlignment(Qt::AlignRight);
    } else if ( a == justifyFillAction ) {
        ui->textEdit->setAlignment(Qt::AlignJustify);
    }
}

void NotesViewController::textColor()
{
    QColor c = QColorDialog::getColor( ui->textEdit->textColor(), ui->textEdit, "Select Color", QColorDialog::DontUseNativeDialog );
    if ( !c.isValid() )
        return;
    ui->textEdit->setTextColor(c);
    textColorChanged(c);
}

void NotesViewController::textBgColor()
{
    QColor c = QColorDialog::getColor( ui->textEdit->textColor(), ui->textEdit, "Select Color", QColorDialog::DontUseNativeDialog );
    //QColor c = QColorDialog::getColor( ui->textEdit->textBackgroundColor(), ui->textEdit );
    if ( !c.isValid() )
        return;
    ui->textEdit->setTextBackgroundColor(c);
    textBgColorChanged(c);
}

void NotesViewController::textFont()
{
    bool ok;
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    QFont f = QFontDialog::getFont( &ok, fmt.font(), ui->textEdit, "Select Font", QFontDialog::DontUseNativeDialog );
    /*if ( !f.isValid() )
        return;*/
    fmt.setFont(f);
    ui->textEdit->mergeCurrentCharFormat(fmt);
    //showFontsAction->setText( QString("Font...    [ %1 - %2 ]").arg(f.family()).arg(f.pointSize()));
    /*int fontSize = f.pointSize();
    QString unit = "pt";
    if ( fontSize < 0 ) {
        fontSize = f.pixelSize();
        unit = "px";
    }
    showFontsAction->setText( QString("Font...    [ %1 - %2%3 ]").arg(f.family()).arg(fontSize).arg(unit));*/
    fontChanged(f);
    //ui->textEdit->setCurrentFont(f);
}

void NotesViewController::clearFormats()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if ( cursor.hasSelection() ) {
	    QTextCharFormat fmt = QTextCharFormat();
	    QString text = cursor.selection().toPlainText();
	    cursor.beginEditBlock();
	    cursor.removeSelectedText();
	    cursor.insertText(text, fmt);
	    cursor.endEditBlock();
    }
}

void NotesViewController::enlarge()
{
    resizeImage(0.1);
    resizeText(1);
}

void NotesViewController::shrink()
{
    resizeImage(-0.1);
    resizeText(-1);
}

void NotesViewController::resetSize()
{
    resizeImage(1, true);
}

void NotesViewController::applicationWillClose()
{
    QSettings settings;
    //settings.setValue(QString("selections/%1").arg(this->folder_id), cur_selection.row());
    for(auto it = selectedNote.begin(); it != selectedNote.end(); ++it) {
        auto index = it.value();
        if ( index.isValid() ) {
            //qDebug() << index;
            settings.setValue(QString("selected_note/%1").arg(it.key()), index.row());
        }
    }
}

NotesViewController::~NotesViewController()
{
    delete sqlTableModel;
    delete proxyModel;
    delete msgButton;
    delete editStrikeThroughAction;
    delete editBoldAction;
    delete editUnderlineAction;
    delete editItalicAction;
}
