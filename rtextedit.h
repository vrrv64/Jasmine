#ifndef RTEXTEDIT_H
#define RTEXTEDIT_H

#include <QTextEdit>

class RTextEdit : public QTextEdit
{
public:
    RTextEdit(QWidget*&);
    bool canInsertFromMimeData(const QMimeData* source) const;
    void insertFromMimeData(const QMimeData* source);
protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    void focusInEvent(QFocusEvent *e);
    void showEvent(QShowEvent *e);
    void resizeEvent(QResizeEvent *e);
private:
    void dropImage(const QUrl& url, const QImage& image);
    void dropTextFile(const QUrl& url);
    void alignMsgButton();
};

#endif // RTEXTEDIT_H
