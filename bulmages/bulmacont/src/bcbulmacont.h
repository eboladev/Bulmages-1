/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociación Iglues -- Contabilidad Linux         *
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

#ifndef BCBULMACONT_H
#define BCBULMACONT_H

#include <errno.h>

#include <QPrinter>
#include <QString>
#include <QPixmap>
#include <QMainWindow>
#include <QEvent>
#include <QAction>
#include <QToolButton>
#include <QMessageBox>
#include <QMdiSubWindow>

#include "blfunctions.h"
#include "ui_bcbulmacontbase.h"
#include "bccompany.h"
#include "blconfiguration.h"
#include "blwindowlistdock.h"
#include "blworkspace.h"
#include "blmainwindow.h"

/// This Class is the base class for your application. It sets up the main
/// window and providing a menubar, toolbar
/// and statusbar. For the main view, an instance of class Bulmages01View is
/// created which creates your view.
class BC_EXPORT BcBulmaCont : public BlMainWindow, public Ui_BcBulmaContBase
{
    Q_OBJECT

private:
    BcCompany *m_company;
    /// m_pWorkspace is the MDI frame widget that handles MDI child widgets.
    BlWorkspace *m_pWorkspace;
    BlWindowListDock *m_list;

public:
    BcBulmaCont ( QWidget *parent = 0, Qt::WFlags f = Qt::Window, QString DB = "" );
    ~BcBulmaCont();
    BcCompany *company();
    /// Exporta menues.
    QMenu *mui_MenuHerramientas();
    BlWorkspace *workspace();

protected:
    /// Event filter to catch close events for MDI child windows and is installed in
    /// createClient() on every child window.
    /** Closing a window calls the eventFilter first which removes the view from the
        connected documents' view list. If the last view is going to be closed, the
        eventFilter() tests if the document is modified; if yes, it asks the user to
        save the document. If the document title contains "Untitled", slotFileSaveAs()
        gets called to get a save name and path. */
    virtual bool eventFilter ( QObject* object, QEvent* event );

private slots:
    /// Reverts the last editing action.
    void slotEditUndo();
    /// put the marked text/object into the clipboard and remove
    /// it from the document.
    void slotEditCut();
    /// Put the marked text/object into the clipboard.
    void slotEditCopy();
    /// Paste the clipboard into the document.
    void slotEditPaste();
    /// Slot para mostrar los listados de cuentas.
    void slotListadoCuentas();
    void slotPerdidas();
    void slotCentrosCoste();
    void slotCanales();
    void slotViewFullScreen ( bool toggle );
    virtual void s_asiento1();
    void closeEvent ( QCloseEvent * );
    /// Toggle the statusbar.
    void slotViewStatusBar ( bool toggle );
    /// Creates a new view for the current document.
    void slotWindowNewWindow();
    /// Change the status message of the whole statusbar temporary.
    void slotStatusHelpMsg ( const QString &text );
    /// Gets called when the window menu is activated; recreates the window menu with all
    /// opened window titles.
    void windowMenuAboutToShow();
    void setActionIndexador ( bool visible );

private:
    /// El nombre de la base de datos.
    QString DBName;
    /// Configura el 'statusbar'.
    void initStatusBar();

public slots:
    virtual void on_actionTipos_de_IVA_triggered();
    virtual void on_actionFormas_de_Pago_triggered();
    virtual void on_actionConfiguracion_triggered();
    virtual void on_actionRecalcular_Saldos_Iniciales_triggered();
    virtual void on_actionSustituir_Cuentas_triggered();
    virtual void on_actionCanal_por_Defecto_triggered();
    virtual void on_actionCentros_de_Coste_triggered();
    virtual void on_actionCanales_triggered();
    virtual void on_actionCentro_de_Coste_por_Defecto_triggered();
    virtual void on_actionOrdenar_Ventanas_triggered();
    virtual void on_actionOrganizaci_n_en_Cascada_triggered();
    virtual void on_actionAyuda_triggered();
    virtual void on_actionAcerca_de_triggered();
    virtual void on_actionMasas_Patrimoniales_triggered();
    virtual void on_actionPantalla_Completa_triggered();
    virtual void on_actionPaises_triggered();
    virtual void informaIndexador ( QWidget *w );
    virtual void informaIndexador ( QMdiSubWindow *w );
    virtual void on_actionIndexador_triggered();
    virtual void on_actionCerrar_Asientos_triggered();
};

#endif

