#ifndef BULMATPV_H
#define BULMATPV_H

#include <qmainwindow.h>

class QTextEdit;

class BulmaTPV: public QMainWindow
{
    Q_OBJECT

public:
    BulmaTPV();
    ~BulmaTPV();

protected:
    void closeEvent( QCloseEvent* );

private slots:
    void newDoc();
    void choose();
    void load( const QString &fileName );
    void save();
    void saveAs();
    void print();

    void about();
    void aboutQt();

private:
    QPrinter *printer;
    QTextEdit *e;
    QString filename;
};


#endif
