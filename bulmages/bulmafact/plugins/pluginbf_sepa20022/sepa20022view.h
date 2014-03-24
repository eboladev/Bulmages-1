/***************************************************************************
 *   Copyright (C) 2014 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#ifndef COBROVIEW_H
#define COBROVIEW_H

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtCore/QTextStream>

#include "ui_sepa20022base.h"
#include "blpostgresqlclient.h"
#include "bldatesearch.h"
#include "bldialogchanges.h"
#include "bfform.h"
#include "facturaslist.h"
#include "sepa20022writer.h"

class BfCompany;


/** Ventana de ficha de cobro.
    Se encarga de la presentacion de la ficha de cobro y del tratamiento de eventos producidos
    en dicha ventana.
    Deriva de BfForm para metodos comunes a todas las ventanas.
    Deriva de Cobro para el manejo de la Base de datos. */
class Sepa20022View : public BfForm, public Ui_Sepa20022Base
{
    Q_OBJECT

public:
    FacturasList *m_facturasList;
    Sepa20022View ( FacturasList *,  BfCompany *, QWidget * );
    ~Sepa20022View();

    /// Como ya es un signaling slot en la clase ficha no se delcara como slot.
    virtual void on_mui_aceptar_clicked();
    virtual void closeEvent ( QCloseEvent * );

 public slots:
   void on_mui_buscararchivo_clicked();
 
 private:
     Sepa20022Writer *m_q19;
     QString ids;
     void idsFacturas ( void );
};

#endif
