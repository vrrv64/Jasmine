#include "rtextedit.h"
#include "mainwindow.h"
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QMimeData>
#include <QStatusBar>
#include <QMouseEvent>
#include <QImageReader>
#include <QImageWriter>
#include <QApplication>
#include <QTemporaryFile>
#include <QStandardPaths>

RTextEdit::RTextEdit(QWidget*& w): QTextEdit(w)
{

}

bool RTextEdit::canInsertFromMimeData(const QMimeData* source) const
{
    return source->hasImage() || source->hasUrls() ||
        QTextEdit::canInsertFromMimeData(source);
}

void RTextEdit::insertFromMimeData(const QMimeData* source)
{
    QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QString attLocation = QDir(dataLocation).filePath("attachments");

    if (source->hasImage())
    {
        QString outfileName = QDir(attLocation).filePath("XXXXXX");
        QTemporaryFile outfile(outfileName);
        outfile.open();
        outfile.setAutoRemove(false);
        outfile.close();
        outfileName = outfile.fileName();
        //qDebug() << outfileName;
        QImage img(qvariant_cast<QImage>(source->imageData()));
        if ( img.height() > 180 ) {
            img = img.scaledToHeight(180);
        }
        QImageWriter writer(outfileName);
        writer.setFormat("png");
        if ( ! writer.write( img ) ) {
            qDebug() << "Failed to write image at " << outfileName;
            qDebug() << writer.errorString();
        } else {
            QUrl url(outfileName);
            dropImage(url, img);
        }
    }
    else if (source->hasUrls())
    {
        foreach (QUrl url, source->urls())
        {
            QFileInfo info(url.toLocalFile());
            if (QImageReader::supportedImageFormats().contains(info.suffix().toLower().toLatin1()))
                dropImage(url, QImage(info.filePath()));
            else
                dropTextFile(url);
        }
    }
    else
    {
        QTextEdit::insertFromMimeData(source);
    }
}

void RTextEdit::dropImage(const QUrl& url, const QImage& image)
{
    if (!image.isNull())
    {
        document()->addResource(QTextDocument::ImageResource, url, image);
        textCursor().insertImage(url.toString());
    }
}

void RTextEdit::dropTextFile(const QUrl& url)
{
    QFile file(url.toLocalFile());
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        textCursor().insertText(file.readAll());
}

void RTextEdit::mouseMoveEvent(QMouseEvent *e)
{
    QTextEdit::mouseMoveEvent(e);
    //qDebug() << ((MainWindow *)ui)->statusBar();
    QString anchor = anchorAt(e->pos());
    //qDebug() << anchor.isNull() << ", " << anchor.length() << ", " << QApplication::overrideCursor() << ", " << e;
    //qDebug() << cursor();
    if ( ! anchor.isNull() ) {
        if ( Qt::PointingHandCursor != viewport()->cursor() ) {
            viewport()->setCursor(Qt::PointingHandCursor);
            ((MainWindow *)window())->statusBar()->showMessage(anchor);
        }
    } else if ( Qt::PointingHandCursor == viewport()->cursor() ) {
        viewport()->setCursor(Qt::IBeamCursor);
        ((MainWindow *)window())->statusBar()->clearMessage();
    }
}

void RTextEdit::focusInEvent(QFocusEvent *e)
{
    QTextEdit::focusInEvent(e);
    QTextCursor cursor = textCursor();
    QTextCharFormat fmt = cursor.charFormat();
    QString anchor = fmt.anchorHref();
    if ( ! anchor.isNull() ) {
        if ( Qt::PointingHandCursor != viewport()->cursor() ) {
            viewport()->setCursor(Qt::PointingHandCursor);
            ((MainWindow *)window())->statusBar()->showMessage(anchor);
        }
    } else if ( Qt::PointingHandCursor == viewport()->cursor() ) {
        viewport()->setCursor(Qt::IBeamCursor);
        ((MainWindow *)window())->statusBar()->clearMessage();
    }
}

void RTextEdit::alignMsgButton()
{
    QPushButton *msgButton = this->findChild<QPushButton *>("msgButton");
    if ( msgButton != nullptr ) {
        msgButton->move(this->rect().center() - msgButton->rect().center());
    }
}

void RTextEdit::showEvent(QShowEvent *e)
{
    QTextEdit::showEvent(e);
    alignMsgButton();
}

void RTextEdit::resizeEvent(QResizeEvent *e)
{
    QTextEdit::resizeEvent(e);
    alignMsgButton();
}
