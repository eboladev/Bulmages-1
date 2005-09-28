/****************************************************************************
** Form interface generated from reading ui file 'balancesprintdlg.ui'
**
** Created: vie ene 2 00:22:53 2004
**      by: The User Interface Compiler ($Id: balancesprintdlg.h,v 1.2 2004/01/16 15:15:40 tborras Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef BALANCESPRINTDLG_H
#define BALANCESPRINTDLG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QGroupBox;
class QRadioButton;
class QPushButton;
class QProgressBar;

class balancesprintdlg : public QDialog
{
    Q_OBJECT

public:
    balancesprintdlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~balancesprintdlg();

    QLabel* textLabel1_2;
    QLabel* textLabel1_2_2;
    QLabel* textLabel1;
    QLineEdit* fechafin;
    QLineEdit* nombalance;
    QLineEdit* fechain;
    QGroupBox* groupBox1;
    QRadioButton* radioButton1_2;
    QRadioButton* radioButton1;
    QPushButton* pushButton1;
    QPushButton* pushButton1_2;
    QProgressBar* progreso;

public slots:
    virtual void fechaincambiada();
    virtual void fechafincambiada();

protected:

protected slots:
    virtual void languageChange();

};

#endif // BALANCESPRINTDLG_H
