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

#ifndef BULMACONT_H
#define BULMACONT_H

#include <errno.h>

#include <QMenuBar>
#include <QPrinter>
#include <QString>
#include <QPixmap>
#include <QMainWindow>
#include <QEvent>
#include <QApplication>
#include <QAction>
#include <QToolButton>
#include <QMessageBox>

#include "ui_bulmacontbase.h"
#include "empresa.h"
#include "bbloqfecha.h"
#include "configuracion.h"
#include "listventanas.h"
#include "qworkspace2.h"


/// This Class is the base class for your application. It sets up the main
/// window and providing a menubar, toolbar
/// and statusbar. For the main view, an instance of class Bulmages01View is
/// created which creates your view.
class Bulmacont : public QMainWindow, Ui_BulmaContBase {
    Q_OBJECT

private:
    empresa *m_empresaactual;
    /// m_pWorkspace is the MDI frame widget that handles MDI child widgets.
    QWorkspace2 *m_pWorkspace;
    listventanas *m_list;

public:
    Bulmacont(QWidget *parent = 0, Qt::WFlags f = Qt::Window, QString DB = "");
    ~Bulmacont();
    empresa *empresaactual();
    /// Exporta menues.
    QMenu *mui_MenuHerramientas();
    QMenu *mui_MenuVer();

protected:
    /// Event filter to catch close events for MDI child windows and is installed in
    /// createClient() on every child window.
    /** Closing a window calls the eventFilter first which removes the view from the
        connected documents' view list. If the last view is going to be closed, the
        eventFilter() tests if the document is modified; if yes, it asks the user to
        save the document. If the document title contains "Untitled", slotFileSaveAs()
        gets called to get a save name and path. */
    virtual bool eventFilter(QObject* object, QEvent* event);

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
    /// Slot para el libro mayor.
    void slotAsientos();
    void slotPerdidas();
    void slotCentrosCoste();
    void slotCanales();
    void slotViewFullScreen(bool toggle);
    void slotCompBalance();
    virtual void s_asiento1();
    void closeEvent(QCloseEvent *);
    /// Toggle the statusbar.
    void slotViewStatusBar(bool toggle);
    /// Creates a new view for the current document.
    void slotWindowNewWindow();
    /// Change the status message of the whole statusbar temporary.
    void slotStatusHelpMsg(const QString &text);
    /// Gets called when the window menu is activated; recreates the window menu with all
    /// opened window titles.
    void windowMenuAboutToShow();
    void setActionIndexador(bool visible);

private:
    /// El nombre de la base de datos.
    QString DBName;
    /// Configura el 'statusbar'.
    void initStatusBar();

public slots:
    virtual void on_actionTipos_de_IVA_triggered();
    virtual void on_actionFormas_de_Pago_triggered();
    virtual void on_actionAsientos_triggered();
    virtual void on_actionConfiguracion_triggered();
    virtual void on_actionApuntes_Contables_triggered();
    virtual void on_actionLibro_Mayor_triggered();
    virtual void on_actionLibro_Diario_triggered();
    virtual void on_actionPlan_Contable_triggered();
    virtual void on_actionBalance_de_Cuentas_triggered();
    virtual void on_actionBalance_Jer_rquico_de_Sumas_y_Saldos_triggered();
    virtual void on_actionImprimir_triggered();
    virtual void on_actionGuardar_triggered();
    virtual void on_actionActualizar_triggered();
    virtual void on_actionFiltrar_triggered();
    virtual void on_actionRecalcular_Saldos_Iniciales_triggered();
    virtual void on_actionSustituir_Cuentas_triggered();
    virtual void on_actionBloquear_Fechas_triggered();
    virtual void on_actionCanal_por_Defecto_triggered();
    virtual void on_actionCentros_de_Coste_triggered();
    virtual void on_actionCanales_triggered();
    virtual void on_actionCentro_de_Coste_por_Defecto_triggered();
    virtual void on_actionOrdenar_Ventanas_triggered();
    virtual void on_actionOrganizaci_n_en_Cascada_triggered();
    virtual void on_actionAyuda_triggered();
    virtual void on_actionAcerca_de_triggered();
    virtual void on_actionAbrir_Asientos_triggered();
    virtual void on_actionOrdenar_Asientos_triggered();
    virtual void on_actionAsiento_de_Apertura_triggered();
    virtual void on_actionAsiento_de_Regularizacion_triggered();
    virtual void on_actionAsiento_de_Cierre_triggered();
    virtual void on_actionCuentas_Anuales_triggered();
    virtual void on_actionMasas_Patrimoniales_triggered();
    virtual void on_actionRegistro_de_IVA_triggered();
    virtual void on_actionListado_347_triggered();
    virtual void on_actionCobros_y_Pagos_triggered();
    virtual void on_actionAmortizaciones_triggered();
    virtual void on_actionPantalla_Completa_triggered();
    virtual void on_actionSiguiente_triggered();
    virtual void on_actionAnterior_triggered();
    virtual void on_actionInicio_triggered();
    virtual void on_actionFin_triggered();
    virtual void on_actionPaises_triggered();
    virtual void on_actionProvincias_triggered();
    virtual void informaindexador(QWidget *w);
    virtual void on_actionIndexador_triggered();
};

#endif

