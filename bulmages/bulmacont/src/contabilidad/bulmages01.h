/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#ifndef BULMAGES01_H
#define BULMAGES01_H

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
class Bulmages01 : public QMainWindow, Ui_BulmaContBase {
    Q_OBJECT

private:
    empresa *m_empresaactual;
    /// m_pWorkspace is the MDI frame widget that handles MDI child widgets.
    QWorkspace2 *m_pWorkspace;
    listventanas *m_list;

public:
    Bulmages01(QWidget *parent = 0, Qt::WFlags f = Qt::Window, QString DB = "");
    ~Bulmages01();
    empresa *empresaactual() {
        return m_empresaactual;
    }

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
    virtual void s_asiento1() {
        m_empresaactual->s_asiento1();
    }
    void closeEvent( QCloseEvent *);
    /// Toggle the statusbar.
    void slotViewStatusBar(bool toggle);
    /// Creates a new view for the current document.
    void slotWindowNewWindow();
    /// Change the status message of the whole statusbar temporary.
    void slotStatusHelpMsg(const QString &text);
    /// Gets called when the window menu is activated; recreates the window menu with all
    /// opened window titles.
    void windowMenuAboutToShow();

private:
    /// The name used.
    QString DBName;
    /// Setup the statusbar.
    void initStatusBar();

public slots:
    virtual void on_actionTipos_de_IVA_triggered() {
        m_empresaactual->tiposIVA();
    }
    virtual void on_actionFormas_de_Pago_triggered() {
        m_empresaactual->fPago();
    }
    virtual void on_actionAsientos_triggered() {
        m_empresaactual->muestraasientos();
    }
    virtual void on_actionConfiguracion_triggered() {
        m_empresaactual->propiedadempresa();
    }
    virtual void on_actionApuntes_Contables_triggered() {
        m_empresaactual->muestraapuntes1();
    }
    virtual void on_actionLibro_Mayor_triggered() {
        m_empresaactual->libromayor();
    }
    virtual void on_actionLibro_Diario_triggered() {
        m_empresaactual->librodiario();
    }
    virtual void on_actionPlan_Contable_triggered() {
        m_empresaactual->muestracuentas();
    }
    virtual void on_actionBalance_de_Sumas_y_Saldos_triggered() {
        m_empresaactual->librobalance();
    }
    virtual void on_actionBalance_Jer_rquico_de_Sumas_y_Saldos_triggered() {
        m_empresaactual->librobalancetree();
    }
    virtual void on_actionImprimir_triggered() {
        m_empresaactual->boton_imprimir();
    }
    virtual void on_actionGuardar_triggered() {
        m_empresaactual->boton_guardar();
    }
    virtual void on_actionActualizar_triggered() {
        m_empresaactual->boton_reload();
    }
    virtual void on_actionFiltrar_triggered() {
        m_empresaactual->Filtro();
    }
    virtual void on_actionRecalcular_Saldos_Iniciales_triggered() {
        // empresaactual->recalculasaldos();
    }
    virtual void on_actionSustituir_Cuentas_triggered() {
        m_empresaactual->reemplazacuentaenasientos();
    }
    virtual void on_actionBloquear_Fechas_triggered() {
        m_empresaactual->bloqueoFechas();
    }
    virtual void on_actionCanal_por_Defecto_triggered() {
        m_empresaactual->canaldefecto();
    }
    virtual void on_actionCentros_de_Coste_triggered() {
        m_empresaactual->ccostes();
    }
    virtual void on_actionCanales_triggered() {
        m_empresaactual->canales();
    }
    virtual void on_actionCentro_de_Coste_por_Defecto_triggered() {
        m_empresaactual->centrocostedefecto();
    }
    virtual void on_actionOrdenar_Ventanas_triggered() {
        m_pWorkspace->tile();
    }
    virtual void on_actionOrganizaci_n_en_Cascada_triggered() {
        m_pWorkspace->cascade();
    }
    virtual void on_actionAyuda_triggered();
    virtual void on_actionAcerca_de_triggered();
    virtual void on_actionAbrir_Asientos_triggered() {
        m_empresaactual->Abrirasientos();
    }
    virtual void on_actionOrdenar_Asientos_triggered() {
        m_empresaactual->Ordenarasientos();
    }
    virtual void on_actionAsiento_de_Apertura_triggered() {
        m_empresaactual->abreempresa();
    }
    virtual void on_actionAsiento_de_Regularizacion_triggered() {
        m_empresaactual->regularizaempresa();
    }
    virtual void on_actionAsiento_de_Cierre_triggered() {
        m_empresaactual->cierraempresa();
    }
    virtual void on_actionCuentas_Anuales_triggered() {
        m_empresaactual->compbalance();
    }
    virtual void on_actionMasas_Patrimoniales_triggered() {
        m_empresaactual->mpatrimoniales();
    }
    virtual void on_actionRegistro_de_IVA_triggered() {
        m_empresaactual->registroiva();
    }
    virtual void on_actionModelo_347_no_oficial_triggered() {
        m_empresaactual->modelo347();
    }
    virtual void on_actionCobros_y_Pagos_triggered() {
        m_empresaactual->cobPag();
    }
    virtual void on_actionAmortizaciones_triggered() {
        m_empresaactual->amortizaciones();
    }
    virtual void on_actionPantalla_Completa_triggered() {
        if (isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        } // end if
    }
    virtual void on_actionSiguiente_triggered() {
        m_empresaactual->boton_siguiente();
    }
    virtual void on_actionAnterior_triggered() {
        m_empresaactual->boton_anterior();
    }
    virtual void on_actionInicio_triggered() {
        m_empresaactual->boton_primero();
    }
    virtual void on_actionFin_triggered() {
        m_empresaactual->boton_ultimo();
    }
};

#endif

