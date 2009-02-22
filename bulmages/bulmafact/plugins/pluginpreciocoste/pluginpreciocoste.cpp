/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include <QToolButton>

#include "pluginpreciocoste.h"


///
/**
\return
**/
int entryPoint ( Bulmafact * )
{
    _depura ( "Estoy dentro del plugin de precio de Coste", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginpreciocoste", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    return 0;
}


/// Al crear la ventana de trabajadores tambien creamos un combo box para el tipo de trabajador.
/**
\param trab
\return
**/
int ArticuloView_ArticuloView_Post ( ArticuloView *trab )
{
    _depura ( "ArticuloView_ArticuloView_Post", 0 );

    trab->addDbField ( "preciocostearticulo", BlDbField::DBnumeric, BlDbField::DBNothing, _( "Precio Coste" ) );
    trab->addDbField ( "margenarticulo", BlDbField::DBnumeric, BlDbField::DBNothing, _( "Margen" ) );
    trab->addDbField ( "actualizarmargenarticulo", BlDbField::DBboolean, BlDbField::DBNothing, _( "Actualizar PVP" ) );


    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout19" ) );

    QLabel *textLabel2_9_26 = new QLabel ( trab->m_frameplugin );
    textLabel2_9_26->setObjectName ( QString::fromUtf8 ( "textLabel2_9_9" ) );
    hboxLayout160->addWidget ( textLabel2_9_26 );
    textLabel2_9_26->setText ( "Precio Coste" );

    QLineEdit *passtrab = new QLineEdit ( trab->m_frameplugin );
    passtrab->setObjectName ( QString::fromUtf8 ( "mui_preciocostearticulo" ) );
    hboxLayout160->addWidget ( passtrab );

    QLabel *textLabel2_9_27 = new QLabel ( trab->m_frameplugin );
    textLabel2_9_27->setObjectName ( QString::fromUtf8 ( "textLabel2_9_97" ) );
    hboxLayout160->addWidget ( textLabel2_9_27 );
    textLabel2_9_27->setText ( "Margen" );

    QLineEdit *passtrab1 = new QLineEdit ( trab->m_frameplugin );
    passtrab1->setObjectName ( QString::fromUtf8 ( "mui_margenarticulo" ) );
    hboxLayout160->addWidget ( passtrab1 );


    QLabel *textLabel2_9_28 = new QLabel ( trab->m_frameplugin );
    textLabel2_9_28->setObjectName ( QString::fromUtf8 ( "textLabel2_9_98" ) );
    hboxLayout160->addWidget ( textLabel2_9_28 );
    textLabel2_9_28->setText ( "Actualizar" );

    QCheckBox *passtrab2 = new QCheckBox ( trab->m_frameplugin );
    passtrab2->setObjectName ( QString::fromUtf8 ( "mui_actualizarmargenarticulo" ) );
    hboxLayout160->addWidget ( passtrab2 );

    /// Comprobamos que exista el layout.
    QVBoxLayout *m_hboxLayout1 = trab->m_frameplugin->findChild<QVBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QVBoxLayout ( trab->m_frameplugin );
        m_hboxLayout1->setSpacing ( 0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addLayout ( hboxLayout160 );
    return 0;
}


int BfSubForm_on_mui_list_editFinished ( BfSubForm *subform )
{
    _depura ( "BfSubForm_on_mui_list_editFinished", 0 );
    int err;
    BlDbRecordSet *cur = NULL;
    BlDbRecordSet *cur1 = NULL;
    BlDbRecordSet *cur2 = NULL;

    if (  subform->tableName() != "lpedidoproveedor"
            && subform->tableName() != "lalbaranp"
            && subform->tableName() != "lfacturap" ) {
        return 0;
    } // end if

    BlDbSubFormRecord *rec = subform->lineaat ( subform->m_prevRow );
    if ( rec == NULL ) {
        return -1;
    } // end if

    BlDbSubFormField *camp = ( BlDbSubFormField * ) subform->item ( subform->m_prevRow, subform->m_prevCol );
    camp->refresh();

    if ( camp->nomcampo() == "pvplalbaranp"  || camp->nomcampo() == "pvplfacturap" ) {
        BlDbSubFormRecord * rec = subform->lineaat ( subform->currentRow() );

        if ( rec->DBvalue ( "idarticulo" ) == "" ) return 0;

        QString query = "SELECT preciocostearticulo FROM articulo WHERE idarticulo = " + rec->DBvalue ( "idarticulo" );
        cur = subform->mainCompany() ->loadQuery ( query );
        if ( ! cur->eof() ) {
            if ( "'" + cur->valor ( "preciocostearticulo" ) + "'" == camp->valorcampoprep ( err ) ) {
                delete cur;
                return 0;
            } // end if
        } // end if
        delete cur;

        int ret = QMessageBox::question ( subform, "Pregunta",
                                          "Actualizo precio de coste del articulo..?",
                                          QMessageBox::Yes,
                                          QMessageBox::No );

        if ( ret == QMessageBox::Yes ) {
            QString query1 = "UPDATE articulo SET preciocostearticulo=" + camp->valorcampoprep ( err ) + " where idarticulo=" + rec->DBvalue ( "idarticulo" );
            subform->mainCompany() ->begin();
            subform->mainCompany() ->runQuery ( query1 );
            subform->mainCompany() ->commit();
        } // end if
        return 0;
    } // end if

    if ( camp->nomcampo() == "codigocompletoarticulo" ) {
        cur = subform->mainCompany() ->loadQuery ( "SELECT * FROM articulo WHERE codigocompletoarticulo = '" + camp->text() + "'" );
        if ( !cur->eof() ) {
            rec->setDbValue ( "idarticulo", cur->valor ( "idarticulo" ) );
            rec->setDbValue ( "codigocompletoarticulo", cur->valor ( "codigocompletoarticulo" ) );
            rec->setDbValue ( "nomarticulo", cur->valor ( "nomarticulo" ) );
            if (  subform->tableName() == "lpedidoproveedor"
                    || subform->tableName() == "lalbaranp"
                    || subform->tableName() == "lfacturap" ) {
                rec->setDbValue ( "desc" + subform->tableName(), cur->valor ( "nomarticulo" ) );
                rec->setDbValue ( "cant" + subform->tableName(), "1.00" );
                rec->setDbValue ( "descuento" + subform->tableName(), "0.00" );
                rec->setDbValue ( "pvp" + subform->tableName(), cur->valor ( "preciocostearticulo" ) );
            } // end if
        } else {
            mensajeAviso ( subform->tr ( "Articulo inexistente" ) );
            delete cur;
            return -1;
        } // end if

        cur1 = subform->mainCompany() ->loadQuery ( "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor ( "idtipo_iva" ) + " ORDER BY fechatasa_iva LIMIT 1" );
        if ( !cur->eof() ) {
            if ( subform->tableName() == "lpedidoproveedor"
                    || subform->tableName() == "lalbaranp"
                    || subform->tableName() == "lfacturap" ) {
                rec->setDbValue ( "iva" + subform->tableName(), cur1->valor ( "porcentasa_iva" ) );


                if ( subform->idproveedor() != "" ) {
                    cur2 = subform->mainCompany() ->loadQuery ( "SELECT recargoeqproveedor FROM proveedor WHERE idproveedor = " + subform->idproveedor() );
                    if ( !cur2->eof() ) {
                        if ( cur2->valor ( "recargoeqproveedor" ) == "t" ) {
                            rec->setDbValue ( "reqeq" + subform->tableName(), cur1->valor ( "porcentretasa_iva" ) );
                        } // end if
                    } // end if
                    delete cur2;
                } else {
                    rec->setDbValue ( "reqeq" + subform->tableName(), "0" );
                } // end if

            } // end if
        } // end if
        if ( cur1 != NULL )
            delete cur1;
        if ( cur != NULL )
            delete cur;
    } // end if

    return 0;
}


int ArticuloListSubForm_ArticuloListSubForm_Post ( ArticuloListSubForm *artl )
{
    artl->addSubFormHeader ( "preciocostearticulo", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, artl->tr ( "P Coste" ) );
    artl->addSubFormHeader ( "margenarticulo", BlDbField::DBnumeric, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, artl->tr ( "Margen" ) );
    artl->addSubFormHeader ( "actualizarmargenarticulo", BlDbField::DBboolean, BlDbField::DBNoSave, BlSubFormHeader::DBNone | BlSubFormHeader::DBNoWrite, artl->tr ( "Actualizar Margen" ) );
}


