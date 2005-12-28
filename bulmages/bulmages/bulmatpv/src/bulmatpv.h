#ifndef BULMATPV_H
#define BULMATPV_H

#include <q3mainwindow.h>
//Added by qt3to4:
#include <QCloseEvent>

class Q3TextEdit;

class BulmaTPV: public Q3MainWindow
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
    Q3TextEdit *e;
    QString filename;
};


#endif
