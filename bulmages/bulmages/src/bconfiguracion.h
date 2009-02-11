/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
 *   http://www.iglues.org Asociación Iglues -- Gestión Empresarial Linux  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef BCONFIGURACION_H
#define BCONFIGURACION_H

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QFontDatabase>
#include <QFileDialog>
#include <QListWidgetItem>

#include "ui_configuracion.h"
#include "bselector.h"
#include "blpostgresqlclient.h"

#ifndef WIN32
#include <sys/wait.h>
#endif


class BSelector;


class BConfiguracion : public QDialog, public Ui_UIconfiguracion
{
    Q_OBJECT

public:
    BConfiguracion ( BSelector *, QWidget *, Qt::WFlags );
    ~BConfiguracion();

private:
    BSelector *PunteroAlSelector;
    void cargarFichaBulmages();
    void cargarFichaUsuarios();
    void tablaconfiguracion();
    void restaurarEmpresa();

private slots:
    virtual void cerrar();
    virtual void listView1_currentChanged ( QListWidgetItem * );
    virtual void listView2_clickBotonDerecho ( QListWidgetItem *, const QPoint&, int );
    virtual void listiView2_clickMouse ( int, QListWidgetItem *, const QPoint&, int );
    virtual void FontChanged ( const QString & );
    virtual void FontSizeChanged ( int );
    virtual void salvarEmpresa();
    virtual void BotonA_10aceptar();
    virtual void BotonA_11rechazar();
    virtual void BotonA_6nombreEmpresa();
    virtual void BotonA_61clonarEmpresa();
    virtual void nuevaEmpresa();
    virtual void borrarEmpresa();
    virtual void BotonB_1Aplicar();
    virtual void BotonB_2Desacer();
    virtual void nuevaFacturacion();
    virtual void BotonContaplus();
    virtual void s_importexportbulmafact();
    virtual void on_mui_restaurarc_clicked();
    virtual void on_mui_restaurarf_clicked();
    virtual void on_mui_borrarempresabc_clicked();
    virtual void on_mui_borrarempresabf_clicked();
};

#endif
