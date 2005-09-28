/****************************************************************************
** Form interface generated from reading ui file 'mantComptes.ui'
**
** Created: mié nov 12 02:35:58 2003
**      by: The User Interface Compiler ($Id: mantComptes.h,v 1.2 2004/01/16 15:15:40 tborras Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MANTCOMPTES_H
#define MANTCOMPTES_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QTabWidget;
class QWidget;
class QListView;
class QListViewItem;
class QTable;
class QToolButton;

class PlaComptes : public QDialog
{
    Q_OBJECT

public:
    PlaComptes( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~PlaComptes();

    QLabel* TextLabel1;
    QLineEdit* codigocuenta;
    QLabel* TextLabel2;
    QLineEdit* desccuenta;
    QTabWidget* TabWidget2;
    QWidget* tab;
    QListView* ListView1;
    QWidget* tab_2;
    QTable* tablacuentas;
    QToolButton* toolButton6;
    QToolButton* toolButton5_2;
    QToolButton* toolButton5_2_2;
    QToolButton* ToolButton12;

public slots:
    virtual void borrarcuenta();
    virtual void codigocambiado(const QString &);
    virtual void dbtabla(int,int,int,const QPoint&);
    virtual void descripcioncambiada(const QString &);
    virtual void editarcuenta();
    virtual void listdblpulsada(QListViewItem *);
    virtual void listpulsada(QListViewItem *);
    virtual void return_descripcion();
    virtual void nuevacuenta();
    virtual void return_codigo();

protected:
    QGridLayout* mantComptesLayout;
    QHBoxLayout* Layout6;
    QHBoxLayout* Layout2;
    QHBoxLayout* Layout16;
    QGridLayout* tabLayout;
    QGridLayout* tabLayout_2;
    QHBoxLayout* layout22;
    QHBoxLayout* layout23;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;
    QPixmap image3;

};

#endif // MANTCOMPTES_H
