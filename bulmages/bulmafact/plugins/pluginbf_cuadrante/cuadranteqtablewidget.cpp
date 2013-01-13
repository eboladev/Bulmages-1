/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <QKeyEvent>
#include <QEvent>
#include <QLineEdit>
#include <QTextEdit>
#include <QMenu>

#include "cuadranteqtablewidget.h"
#include "blconfiguration.h"
#include "blfunctions.h"
#include "bfcompany.h"
#include "cuadrante1view.h"
#include "cuadranteview.h"


/** Constructor de CuadranteQTableWidget clase derivada de QTableWidget con
un eventHandler especifico
*/
/**
\param parent
**/
CuadranteQTableWidget::CuadranteQTableWidget ( QWidget *parent ) : QTableWidget ( parent )
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( contextMenuRequested ( int, int, const QPoint & ) ), this, SLOT ( on_contextMenuRequested ( int, int, const QPoint & ) ) );
    
}


///
/**
**/
CuadranteQTableWidget::~CuadranteQTableWidget()
{
    
}


///
/**
\param event
**/
void CuadranteQTableWidget::dragEnterEvent ( QDragEnterEvent *event )
{
    BL_FUNC_DEBUG
    event->acceptProposedAction();
    
}


///
/**
\param event
**/
void CuadranteQTableWidget::dragMoveEvent ( QDragMoveEvent *event )
{
    BL_FUNC_DEBUG
    event->acceptProposedAction();
    
}

///
/**
\param event
**/
void CuadranteQTableWidget::dropEvent ( QDropEvent *event )
{
    BL_FUNC_DEBUG
    QTableWidget::dropEvent ( event );
    
}


///
/**
\param row
\param column
\param data
\return
**/
bool CuadranteQTableWidget::dropMimeData ( int row, int column, const QMimeData *data, Qt::DropAction )
{
    BL_FUNC_DEBUG

    if ( data->hasText() ) {
        ( ( CuadranteQTextDocument * ) cellWidget ( row, column ) ) ->addTrabajador ( data->text() );
    }
    
    return ( TRUE );
}


void CuadranteQTableWidget::on_contextMenuRequested ( int , int , const QPoint &  )
{
    BL_FUNC_DEBUG
    
}


/// ======================================================================
/// ======================================================================


///
/**
\param emp
\param parent
**/
CuadranteQTextDocument::CuadranteQTextDocument ( BfCompany *emp, QWidget *parent ) : QLabel ( parent ),/* QTableWidgetItem(QTableWidgetItem::UserType),*/  BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
    setContextMenuPolicy ( Qt::CustomContextMenu );
    connect ( this, SIGNAL ( customContextMenuRequested ( const QPoint &  ) ), this, SLOT ( on_customContextMenuRequested ( const QPoint &  ) ) );
    resize ( 400, 400 );
    
}


void CuadranteQTextDocument::on_customContextMenuRequested ( const QPoint & pos )
{
    QMap<QAction *, QString> horarios;
    m_menu = new QMenu ( this );
    QAction *at = m_menu->addAction ( "Vaciar Personal" );
    QAction *com = m_menu->addAction ( "Quitar Comentarios" );
    QAction *fest = m_menu->addAction ( "Festivo" );
    QAction *nofest = m_menu->addAction ( "No Festivo" );
    QMenu *menu1 = m_menu->addMenu ( "Eliminar Personal" );
    BlDbRecordSet *cur1 = mainCompany() ->loadQuery ( "SELECT * FROM horario, trabajador, tipotrabajo WHERE horario.idtrabajador = trabajador.idtrabajador AND trabajador.idtipotrabajo = tipotrabajo.idtipotrabajo AND idcuadrante = " + mdb_idcuadrante + " ORDER BY nomtipotrabajo, nomtrabajador, horainhorario" );
    if ( !cur1 ) throw - 1;
    while ( !cur1->eof() ) {
        QAction * ac = menu1->addAction ( cur1->value( "nomtrabajador" ) + "(" + cur1->value( "horainhorario" ) + ":" + cur1->value( "horafinhorario" ) + ")" );
        horarios[ac] = cur1->value( "idhorario" );
        cur1->nextRecord();
    } // end while
    delete cur1;

    /// Disparamos los plugins.
    int res = g_plugins->run ( "CuadranteQTextDocument_on_customContextMenuRequested", this );
    if ( res != 0 ) {
        return;
    } // end if

    QAction *sel = m_menu->exec ( mapToGlobal ( pos ), at );
    m_accion = sel;

    /// Miramos y tratamos la opcion seleccionada.
    if ( sel == at ) {
        QString query = "DELETE FROM horario WHERE idcuadrante = " + mdb_idcuadrante;
        mainCompany() ->runQuery ( query );
    } // end if

    if ( horarios.contains ( sel ) ) {
        QString query = "DELETE FROM horario WHERE idhorario = " + horarios[sel];
        mainCompany() ->runQuery ( query );
    } // end if

    if ( sel == fest ) {
        QString query = "UPDATE cuadrante SET fiestacuadrante = TRUE WHERE idcuadrante = " + mdb_idcuadrante;
        mainCompany() ->runQuery ( query );
    } // end if

    if ( sel == nofest ) {
        QString query = "UPDATE cuadrante SET fiestacuadrante = FALSE WHERE idcuadrante = " + mdb_idcuadrante;
        mainCompany() ->runQuery ( query );
    } // end if


    if ( sel == com ) {
        QString query = "UPDATE cuadrante SET comentcuadrante = '' WHERE idcuadrante = " + mdb_idcuadrante;
        mainCompany() ->runQuery ( query );
    } // end if

    g_plugins->run ( "CuadranteQTextDocument_on_customContextMenuRequested_Post", this );


    pintaCuadrante ( mdb_idalmacen, mdb_fechacuadrante );

    delete m_menu;

    return;

}

///
/**
\param e
**/
void CuadranteQTextDocument::contextMenuEvent ( QContextMenuEvent *  )
{
    /*QTableWidgetItem::setSelected(TRUE);*/
}


///
/**
\param event
**/
void CuadranteQTextDocument::mouseDoubleClickEvent ( QMouseEvent *  )
{
    Cuadrante1View * cuad = new Cuadrante1View ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->pWorkspace() ->addSubWindow ( cuad );
    cuad->show();
//    CuadranteQTextDocument *newItem = (CuadranteQTextDocument *) mui_cuadrante->cellWidget(mui_cuadrante->currentRow(), mui_cuadrante->currentColumn());
    connect ( cuad, SIGNAL ( saved() ), this, SLOT ( refresh() ) );
    cuad->load ( idcuadrante() );
}



///
/**
**/
CuadranteQTextDocument::~CuadranteQTextDocument()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param idtrabajador
**/
void CuadranteQTextDocument::addTrabajador ( QString idtrabajador )
{
    BL_FUNC_DEBUG
    BlDbRecordSet *cur = NULL;
    try {
        QString horain = "08:00";
        QString horafin = "13:00";
        QString horain1 = "16:00";
        QString horafin1 = "20:00";

        QString querycuad = "SELECT * FROM cuadrante WHERE idcuadrante=" + mdb_idcuadrante;
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( querycuad );
        if ( !cur ) throw - 1;
        if ( !cur->eof() ) {
            if ( cur->value( "aperturacuadrante" ) != "" )
                horain = cur->value( "aperturacuadrante" );
            if ( cur->value( "cierrecuadrante" ) != "" )
                horafin = cur->value( "cierrecuadrante" );
            if ( cur->value( "apertura1cuadrante" ) != "" )
                horain1 = cur->value( "apertura1cuadrante" );
            if ( cur->value( "cierre1cuadrante" ) != "" )
                horafin1 = cur->value( "cierre1cuadrante" );
        } // end if

        QString query = "INSERT INTO horario (idtrabajador, idcuadrante, horainhorario, horafinhorario) VALUES (" + idtrabajador + "," + mdb_idcuadrante + ",'" + horain + "','" + horafin + "')";
        QString query1 = "INSERT INTO horario (idtrabajador, idcuadrante, horainhorario, horafinhorario) VALUES (" + idtrabajador + "," + mdb_idcuadrante + ",'" + horain1 + "','" + horafin1 + "')";
        mainCompany() ->begin();
        mainCompany() ->runQuery ( query );
        mainCompany() ->runQuery ( query1 );
        mainCompany() ->commit();
        pintaCuadrante ( mdb_idalmacen, mdb_fechacuadrante );
        
    } catch ( ... ) {
        mainCompany() ->rollback();
        if ( cur ) delete cur;
        blMsgInfo ( "Error al agregar el trabajador al cuadrante" );
    } // end try
    
}


///
/**
\param idalmacen
\param date
\return
**/
void CuadranteQTextDocument::setAlmFecha ( QString idalmacen, const QDate &date )
{
    BL_FUNC_DEBUG
    BlDbRecordSet *cur1 = NULL;
    try {
        /// Comprobamos que exista el cuadrante y si no existe lo creamos.
        cur1 = mainCompany() ->loadQuery ( "SELECT * FROM cuadrante WHERE fechacuadrante = '" + date.toString ( "dd/MM/yyyy" ) + "' AND idalmacen=" + idalmacen );
        if ( !cur1 ) throw - 1;
        if ( cur1->eof() ) {
            QString query = "INSERT INTO cuadrante (idalmacen, fechacuadrante) VALUES (" + idalmacen + ",'" + date.toString ( "dd/MM/yyyy" ) + "')";
            mainCompany() ->begin();
            mainCompany() ->runQuery ( query );
            mainCompany() ->commit();
        } // end if
        delete cur1;
        pintaCuadrante ( idalmacen, date );
    } catch ( ... ) {
        mainCompany() ->rollback();
        blMsgInfo ( "Error en la carga" );
        if ( cur1 ) delete cur1;
        return;
    } // end try
    
}


///
/**
\param idalmacen
\param data
**/
void CuadranteQTextDocument::pintaCuadrante ( QString idalmacen, const QDate &date )
{
    BL_FUNC_DEBUG
    mdb_idalmacen = idalmacen;
    mdb_fechacuadrante = date;

    ImpCuadrante *imp = new ImpCuadrante ( ( BfCompany * ) mainCompany() );
    imp->mdb_idalmacen = idalmacen;
    imp->mdb_fechacuadrante = date;
    imp->generar();
    QLabel::setAlignment ( Qt::AlignTop );
    QLabel::setText ( imp->m_html );
    mdb_idcuadrante = imp->mdb_idcuadrante;
    
    return;
}


///
/**
\return
**/
QString CuadranteQTextDocument::idcuadrante()
{
    BL_FUNC_DEBUG
    
    return mdb_idcuadrante;
}


///
/**
\return
**/
QDate CuadranteQTextDocument::fechacuadrante()
{
    BL_FUNC_DEBUG
    
    return mdb_fechacuadrante;
}


///
/**
\return
**/
QString CuadranteQTextDocument::idalmacen()
{
    BL_FUNC_DEBUG
    
    return mdb_idalmacen;
}


///
/**
**/
void CuadranteQTextDocument::refresh()
{
    BL_FUNC_DEBUG
    pintaCuadrante ( mdb_idalmacen, mdb_fechacuadrante );
    
}


///
/**
**/
void CuadranteQTextDocument::setText ( QString text )
{
    BL_FUNC_DEBUG
    QLabel::setText ( text );
    
}


///
/**
**/
void CuadranteQTextDocument::setidcuadrante ( QString text )
{
    BL_FUNC_DEBUG
    mdb_idcuadrante = text;
    
}

///
/**
\return
**/
const QString CuadranteQTextDocument::impresion()
{
    BL_FUNC_DEBUG

    try {
        QString html = "";
        html += "<td>\n";
        QString style = "";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM cuadrante, almacen WHERE cuadrante.idalmacen = almacen.idalmacen AND almacen.idalmacen=" + mdb_idalmacen + " AND cuadrante.fechacuadrante ='" + mdb_fechacuadrante.toString ( "dd/MM/yyyy" ) + "'" );
        if ( !cur ) throw - 1;
        if ( !cur->eof() ) {
            if ( cur->value( "fiestacuadrante" ) == "t" ) {
                style = " style=\"festivo\"";
            }
            html += "<para " + style + "> <b><font color=\"red\" size=\"5\">" + cur->value( "nomalmacen" ) + "</font></b><font size=\"5\">: " + mdb_fechacuadrante.toString ( "dd/MM/yyyy" ) + "</font></para><spacer length=\"0.1cm\"/>\n";
        } // end if


        QString oldnomtipotrabajo = "";

        BlDbRecordSet *cur1 = mainCompany() ->loadQuery ( "SELECT * FROM horario, trabajador, tipotrabajo WHERE horario.idtrabajador = trabajador.idtrabajador AND trabajador.idtipotrabajo = tipotrabajo.idtipotrabajo AND idcuadrante = " + mdb_idcuadrante + " ORDER BY nomtipotrabajo, horainhorario, nomtrabajador" );
        if ( !cur1 ) throw - 1;
        while ( !cur1->eof() ) {
            if ( oldnomtipotrabajo != cur1->value( "nomtipotrabajo" ) ) {
                html +=  "<para " + style + "><font color=\"red\" size=\"5\">" + cur1->value( "nomtipotrabajo" ) + ":</font></para><spacer length=\"0.1cm\"/>\n";
                oldnomtipotrabajo = cur1->value( "nomtipotrabajo" );
            } // end if
            html += "<para " + style + "><font face=\"Helvetica\" size=\"6\" color=\"" + cur1->value( "colortipotrabajo" ) + "\">" + cur1->value( "nomtrabajador" ) + " " + cur1->value( "apellidostrabajador" );
            html += "<sup>(" + cur1->value( "horainhorario" ).left ( 5 ) + "--" + cur1->value( "horafinhorario" ).left ( 5 ) + ")</sup></font></para><spacer length=\"0.1cm\"/>\n";

            cur1->nextRecord();
        } // end while
        delete cur1;

        if ( cur->value( "comentcuadrante" ) != "" ) {
            html += "<para " + style + "><font face=\"Helvetica\" size=\"5\" color=\"black\">" + cur->value( "comentcuadrante" ).replace ( "\n", "<spacer length=\"0.1cm\"/>\n" ) + "</font></para>";
        } // end if

        delete cur;

        html += "</td>\n";

        
        return html;
    } catch ( ... ) {} // end try
    return "";
}


/// =====================================================================================

///
/**
\param emp
\param parent
**/
ImpCuadrante::ImpCuadrante ( BfCompany *emp ) :  BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
ImpCuadrante::~ImpCuadrante()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param idalmacen
\param data
**/
void ImpCuadrante::generar()
{
    BL_FUNC_DEBUG
    BlDbRecordSet *cur = NULL;
    BlDbRecordSet *cur1 = NULL;

    /// Disparamos los plugins.
    int res = g_plugins->run ( "ImpCuadrante_generar", this );
    if ( res != 0 ) {
        return;
    } // end if

    try {
        QString html = "";
        cur = mainCompany() ->loadQuery ( "SELECT * FROM cuadrante, almacen WHERE cuadrante.idalmacen = almacen.idalmacen AND almacen.idalmacen=" + mdb_idalmacen + " AND cuadrante.fechacuadrante ='" + mdb_fechacuadrante.toString ( "dd/MM/yyyy" ) + "'" );
        if ( !cur ) throw - 1;
        if ( !cur->eof() ) {
            if ( cur->value( "fiestacuadrante" ) == "t" ) {
                html += "<table width=\"240\" height=\"300\" bgcolor=\"#999999\"><tr><td>";
            } else {
                html += "<table width=\"240\" height=\"300\" bgcolor=\"#FFFFFF\"><tr><td>";
            }
            html += "<font size=\"2\" color=\"#660000\"><B>" + cur->value( "nomalmacen" ) + "</B>: " + mdb_fechacuadrante.toString ( "dd/MM/yyyy" ) + "</font><BR>";
            mdb_idcuadrante = cur->value( "idcuadrante" );
        } // end if

        QString oldnomtipotrabajo = "";

        cur1 = mainCompany() ->loadQuery ( "SELECT * FROM horario, trabajador, tipotrabajo WHERE horario.idtrabajador = trabajador.idtrabajador AND trabajador.idtipotrabajo = tipotrabajo.idtipotrabajo AND idcuadrante = " + mdb_idcuadrante + " ORDER BY nomtipotrabajo, nomtrabajador, horainhorario " );
        if ( !cur1 ) throw - 1;
        while ( !cur1->eof() ) {


            if ( oldnomtipotrabajo != cur1->value( "nomtipotrabajo" ) ) {
                html += "<font size=\"2\" color=\"#00FF00\" >" + cur1->value( "nomtipotrabajo" ) + ":</font><BR>";
                oldnomtipotrabajo = cur1->value( "nomtipotrabajo" );
            } // end if

            /// Si hay conflictos con el trabajador.
            if ( buscaConflictos ( cur1->value( "idtrabajador" ), mdb_fechacuadrante, cur1->value( "horainhorario" ).left ( 5 ), cur1->value( "horafinhorario" ).left ( 5 ) ) ) {
                html += "<font size=\"3\" color=\"#FF0000\">ERROR</FONT> ";
            }

            html += "<font size=\"3\" color=\"" + cur1->value( "colortipotrabajo" ) + "\">" + cur1->value( "nomtrabajador" ) + " " + cur1->value( "apellidostrabajador" );
            html += " (" + cur1->value( "horainhorario" ).left ( 5 ) + "--" + cur1->value( "horafinhorario" ).left ( 5 ) + ") </font><BR>";

            cur1->nextRecord();
        } // end while
        delete cur1;

        if ( cur->value( "comentcuadrante" ) != "" ) {
            html += "<HR><font size=\"2\" color=\"#000000\">" + cur->value( "comentcuadrante" ).replace ( "\n", "<BR>" ) + "</font>";
        } // end if
        delete cur;

        html += "</td></tr></table>";
        m_html = html;
        
    } catch ( ... ) {
        blMsgInfo ( "Error en el pintado" );
        if ( cur ) delete cur;
        if ( cur1 ) delete cur1;
        return;
    } // end try
}


///
/**
\param idtrabajador
\param date
\param horain
\param horafin
\return
**/
bool ImpCuadrante::buscaConflictos ( QString idtrabajador, const QDate &date, QString horain, QString horafin )
{
    BL_FUNC_DEBUG

    bool conflicto = FALSE;
    QString query = "SELECT * FROM horario NATURAL LEFT JOIN cuadrante  WHERE idtrabajador = " + idtrabajador;
    query += " AND ((horainhorario < '" + horain + "' AND horafinhorario > '" + horain + "')";
    query += " OR (horainhorario > '" + horain + "' AND horafinhorario < '" + horafin + "')";
    query += " OR (horainhorario < '" + horafin + "' AND horafinhorario > '" + horafin + "')";
    query += ")";
    query += " AND fechacuadrante = '" + date.toString ( "dd/MM/yyyy" ) + "'";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    if ( cur ) {
        if ( cur->numregistros() > 1 ) {

            conflicto = TRUE;
        } // end if
        delete cur;
    }

    query = "SELECT * FROM ausencia WHERE idtrabajador = " + idtrabajador + " AND fechainausencia <= '" + date.toString ( "dd/MM/yyyy" ) + "' AND fechafinausencia >= '" + date.toString ( "dd/MM/yyyy" ) + "'";
    cur = mainCompany() ->loadQuery ( query );
    if ( cur ) {
        if ( !cur->eof() ) {

            conflicto = TRUE;
        } // end if
        delete cur;
    } // end if

    
    return conflicto;
}
