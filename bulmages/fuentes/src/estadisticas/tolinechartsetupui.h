/****************************************************************************
** Form interface generated from reading ui file 'tolinechartsetupui.ui'
**
** Created: dom nov 16 21:07:22 2003
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef TOLINECHARTSETUPUI_H
#define TOLINECHARTSETUPUI_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QGroupBox;
class QLabel;
class QLineEdit;
class QCheckBox;
class QPushButton;
class QSpinBox;

class toLineChartSetupUI : public QDialog
{
    Q_OBJECT

public:
    toLineChartSetupUI( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~toLineChartSetupUI();

    QGroupBox* GroupBox1;
    QLabel* TextLabel1;
    QLabel* TextLabel4;
    QLineEdit* MinValue;
    QLineEdit* MaxValue;
    QCheckBox* AutoMax;
    QCheckBox* AutoMin;
    QPushButton* CancelButton;
    QPushButton* OkButton;
    QCheckBox* ShowLast;
    QCheckBox* ShowAxis;
    QCheckBox* ShowLegend;
    QLabel* TextLabel5;
    QSpinBox* Grids;

protected:
    QGridLayout* toLineChartSetupUILayout;
    QGridLayout* GroupBox1Layout;
    QHBoxLayout* Layout1;

protected slots:
    virtual void languageChange();

};

#endif // TOLINECHARTSETUPUI_H
