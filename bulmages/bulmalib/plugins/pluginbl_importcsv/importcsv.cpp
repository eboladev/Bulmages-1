/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <fstream>

#include <QtWidgets/QMessageBox>
#include <QtGui/QCloseEvent>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtWidgets/QWidget>
#include <QtWidgets/QFileDialog>

#include "local_blI18n.h"
#include "blmaincompany.h"
#include "importcsv.h"
#include "blconfiguration.h"

/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
ImportCSV::ImportCSV ( BlMainCompany *comp, QWidget *parent )
        : BlWidget ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );
        rellenarTablas();

        mainCompany()->insertWindow ( windowTitle(), this );
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear la ventana de importacion" ) );
    } // end try
    
}

/** No precisa acciones adicionales en el destructor.
*/
ImportCSV::~ImportCSV()
{
    BL_FUNC_DEBUG
    /// Como no deriva de BlForm ni de ninguna de las ventanas estandar hacemos un removeWindow
    mainCompany()->removeWindow ( this ); 
}

void ImportCSV::on_mui_buscarArchivo_clicked()
{

    QString fileName = QFileDialog::getOpenFileName ( this, _ ( "Open file" ), "", _ ( "All files (*)" ) );
    mui_archivo->setText ( fileName );
}


QString ImportCSV::dbFieldTypeByName(QString fieldName, QString dbTable)
{
    QString query = "SELECT column_name, data_type FROM information_schema.columns WHERE table_name = '" + dbTable + "' AND column_name = '" + fieldName + "';";
    QString result = "";
    /// Cargamos el query y lo recorremos
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    if ( !cur->eof() ) {
        result = cur->value("data_type");
    } // end if
    delete cur;
    
    return result;
}


QStringList ImportCSV::csvLineParser(QString data, QString fieldDelimiter, QString textDelimiter) {
    QStringList values;
     
    bool quotes = false;
    unsigned int pos = 0;
    QString field = "";

    while ( pos < data.length() ) {
      QString c = QString(data.at(pos));
      
      if ( (!quotes) && (c == textDelimiter ) ) {
	  quotes = true;
      } else if ( (quotes) && (c == textDelimiter ) ) {
	  if ( (pos + 1 < data.length()) && (QString(data.at(pos + 1)) == textDelimiter) ) {
	      field.append(c);
	      pos++;
	  } else {
	      quotes = false;
	  } // end if
      } else if ( (!quotes) && (c == fieldDelimiter) ) {
	  values.append(field);
	  field.clear();
      } else if ( (!quotes) && (c == "\n" || c == "\r") ) {
	  values.append(field);
	  field.clear();
      } else {
	  field.append(c);
      } // end if
      pos++;
    } // end while
        
    return values;
}





/** No precisa acciones adicionales en el destructor.
*/
void ImportCSV::on_mui_aceptar_clicked()
{
    BL_FUNC_DEBUG
    
    QFile file ( mui_archivo->text() );
    if ( !file.open ( QIODevice::ReadOnly | QIODevice::Text ) )
        return;

    QString linea = "";
    QStringList list1;
    if ( mui_cabeceras->isChecked() ) {
	/// La primera fila deberia decirnos que campos hay en el fichero.
	/// La leemos y almacenamos estos datos poque nos seran utiles.    
	linea = file.readLine();
	/// Dependiendo del separador que usemos hacemos un split en funcion de ello
	if ( mui_radioSeparadorTab->isChecked() ) {
	    list1 = csvLineParser(linea, "\t");
	} else {
	    list1 = csvLineParser(linea, mui_separador->text());
	} // end if
	
	list1.replaceInStrings("\"", "");
	
    } // end if


    mainCompany()->begin();
    try {
	while ( !file.atEnd() ) {
	
	    QStringList list1Tmp = list1;
	    /// Las lineas siguientes son los datos
	    /// Los leemos y segun su orden los vamos colocando.
	    linea = file.readLine();
	    QStringList list2;
	    /// Dependiendo del separador que usemos hacemos un split en funcion de ello
	    if ( mui_radioSeparadorTab->isChecked() ) {
		list2   = csvLineParser(linea, "\t");
	    } else {
		list2   = csvLineParser(linea, mui_separador->text());
	    } // end if
	    list2.replaceInStrings(QRegExp("^$"), "''");
	    /// Montamos el query
	    QString query = "";

	    
	    /// Si se dispone de cabecera, se puede averiguar el tipo de dato y ajustarlo en caso necesario.
	    if ( mui_cabeceras->isChecked() ) {

		for (int i = 0; i < list1.size(); ++i) {
		    QString tipo = dbFieldTypeByName(list1.at(i), mui_combotablas->currentText());
		    if (tipo.contains("numeric", Qt::CaseInsensitive)) {
			/// Cambia comas a puntos cuando se trata de tipo numerico.
			QString newString = list2.at(i);
			list2.replace(i, newString.replace(QString(","), QString(".")));
		    } else if (tipo.contains("character", Qt::CaseInsensitive)) {
			/// Si el dato es un texto lo proteje entre comillas.
			QString newString = list2.at(i);
			list2.replace(i, "'" + mainCompany()->sanearCadenaUtf8(newString) + "'");
		    } // end if
		} // end for

	    } // end if


	    if (mui_insert->isChecked()) {
		    /// Llamar a funciones especiales en funcion de la tabla
		    
		    if (mui_combotablas->currentText() == "articulo") {
			/// Si no existe la familia
			if (!list1.contains("idfamilia", Qt::CaseInsensitive)) {
			    blMsgError(_("Debe existir el campo idfamilia para crear nuevos registros."));
			    return;
			} // end if
			
			/// Si no existe el campo de 'codigocompletoarticulo' entonces se crea.
			if (!list1.contains("codarticulo", Qt::CaseInsensitive)) {
			    list1Tmp.append("codarticulo");
			    list2.append("''"); /// Codigo de articulo vacio para que se calcule por la base de datos.
			} // end if
		    } // end if
	    
		    query = "INSERT INTO " + mui_combotablas->currentText() +" (" + list1Tmp.join(",") + ") VALUES(" + list2.join(",") + ")";
		    
	    } else {
		    query = "UPDATE " + mui_combotablas->currentText() + " SET ";
		    int condition = -1;
		    
		    for (int i = 0; i < list1.size(); ++i) {
			query += list1[i] + " = " + list2[i];

			if (i + 1 < list1.size()) {
			    query += ", ";
			} // end if

			QString compara = mui_updateField->text().trimmed();
			
			if (list1.at(i).trimmed() == compara) {
			    condition = i;
			} // end if
		    } // end for

		    if (condition == -1) {
			/// No se ha encontrado el campo para condicion de actualizar.
			throw -100;
		    }
		    
		    query += " WHERE " + mui_updateField->text().trimmed() + " = " + list2.at(condition);

	    } // end if
	    
	    mainCompany()->runQuery(query);
	} // end while
	mainCompany()->commit();
	blMsgInfo(_("Importacion realizada satisfactoriamente"));
    } catch (...) {
	mainCompany()->rollback();
	blMsgInfo("Hubo un error en la importacion");
    } // end try
    file.close();

    
}


/// Segun las cabeceras presentamos una forma u otra.
void ImportCSV::on_mui_cabeceras_stateChanged ( int state )
{
    if ( state != 0 ) {
        m_claves.clear();
	/// Abrimos el fichero
        QFile file ( mui_archivo->text() );
        if ( !file.open ( QIODevice::ReadOnly | QIODevice::Text ) )
            return;
	/// Cogemos la cabecera y la leemos
        QString linea = file.readLine();
        QStringList list1;
	/// Dependiendo del separador que usemos hacemos un split en funcion de ello
        if ( mui_radioSeparadorTab->isChecked() ) {
            list1   = linea.split ( "\t" );
        } else {
            list1   = linea.split ( mui_separador->text() );
        } // end if

	/// Ponemos cada campo entre corchetes y generamos la cadena de cabeceras.
        QString text = "";
        for ( int i = 0; i < list1.size(); ++i ) {
            m_claves.append ( "[" + list1.at ( i ) + "]" );
            text += "[" + list1.at ( i ) + "] ";
        } // end for
	/// Escribimos la cabecera.
        mui_cabecerasEdit->setText ( text );
        file.close();
    } // end if
}


/// Para cada linea del archivo leido la procesamos y hacemos la insercion en la base de datos.
void ImportCSV::procesarLinea ( const QString &linea )
{
    /// Primero trabajamos sobre el separado de campos segun el delimitador establecido.
    QStringList list1;
    if ( mui_radioSeparadorTab->isChecked() ) {
        list1   = linea.split ( "\t" );
    } else {
        list1   = linea.split ( mui_separador->text() );
    } // end if

    /// Luego componemos en query de insercion
    QString query = "INSERT INTO " + mui_combotablas->currentText() + "(";
    QString coma = "";
    for ( int i = 0; i < mui_list->rowCount(); ++i ) {
        if ( mui_list->item ( i, 1 ) )
            if ( mui_list->item ( i, 1 )->text() != "" ) {
                query += coma + mui_list->item ( i, 0 )->text();
                coma = ",";
            } // end if
    } // end for

    query += ") VALUES (";

    /// Para procesar los valores nos vamos a la tabla.
    coma = "";
    for ( int i = 0; i < mui_list->rowCount(); ++i ) {
        if ( mui_list->item ( i, 1 ) )
            if ( mui_list->item ( i, 1 )->text() != "" ) {
                QString valor = mui_list->item ( i, 1 )->text();
                for ( int j = 0; j < m_claves.size() && j < list1.size(); ++j ) {
                    valor.replace ( m_claves[j], list1[j] );
                    valor.replace ( "[" + QString::number ( j ) + "]", list1[j] );
                } // end for
                query += coma + "'" + mainCompany()->sanearCadenaUtf8(valor) + "'";
                coma = ",";
            } // end if
    } // end for
    query += ")";

    mainCompany()->runQuery ( query );
}


/// Rellenamos el ComboBox de tablas.
void ImportCSV::rellenarTablas()
{
    QString query = "select * from information_schema.tables where table_schema='public' and table_type='BASE TABLE' ORDER BY table_name";
    BlDbRecordSet *cur = mainCompany()->loadQuery ( query );
    mui_combotablas->clear();
    while ( !cur->eof() ) {
        mui_combotablas->addItem ( cur->value( "table_name" ) );
        cur->nextRecord();
    } // end while
    delete cur;
}


/// Detectamos el cambio en el combo de tablas y presentamos los atributos.
void ImportCSV::on_mui_combotablas_activated ( const QString & text )
{

    /// Cogemos la tabla y la presentamos.
    QStringList lista_campos;
    QString query = "SELECT a.attnum, a.attname AS field, t.typname AS type, a.attlen AS length, a.atttypmod AS lengthvar, a.attnotnull AS notnull FROM pg_class c, pg_attribute a, pg_type t WHERE c.relname = $1 and  a.attnum > 0 and a.attrelid = c.oid and a.atttypid = t.oid ORDER BY a.attnum";
    BlDbRecordSet *cur = mainCompany()->load ( query,text );
    mui_list->setRowCount ( 2 );
    mui_list->setColumnCount ( cur->numregistros() );
    int col = 0;
    while ( !cur->eof() ) {
        QTableWidgetItem *newItem = new QTableWidgetItem ( cur->value( "field" ) );
	lista_campos << cur->value( "field" );
        mui_list->setItem ( 0, col, newItem );
        cur->nextRecord();
        col ++;
    } // end while
    delete cur;
    
    /// Ahora si hay archivo lo leemos e intentamos hacer una presentacion de los datos tambien.
    QFile file ( mui_archivo->text() );
    if ( !file.open ( QIODevice::ReadOnly | QIODevice::Text ) )
        return;
    if (file.atEnd())
	return;

    /// La primera fila deberia decirnos que campos hay en el fichero.
    /// La leemos y almacenamos estos datos poque nos seran utiles.    
    QString linea = file.readLine();
    QStringList list1;
    /// Dependiendo del separador que usemos hacemos un split en funcion de ello
    if ( mui_radioSeparadorTab->isChecked() ) {
	list1   = linea.split ( "\t" );
    } else {
	list1   = linea.split ( mui_separador->text() );
    } // end if

	
    /// Procesamos todo el fichero.
    int row = 1;  /// Las filas en la tabla (ya hay una que son los encabezados)
    while ( !file.atEnd() ) {
	/// Las lineas siguientes son los datos
	/// Los leemos y segun su orden los vamos colocando.
	linea = file.readLine();
	QStringList list2;
	/// Dependiendo del separador que usemos hacemos un split en funcion de ello
	if ( mui_radioSeparadorTab->isChecked() ) {
	    list2   = linea.split ( "\t" );
	} else {
	    list2   = linea.split ( mui_separador->text() );
	} // end if
	
	mui_list->setRowCount ( row + 1 );
	/// Ahora buscamos los valores en la tabla y los vamos colocando segun aparezcan.
	for (int campo = 0; campo < lista_campos.size(); ++campo) {
	    QString fieldName = lista_campos.at(campo);
	    int index = list1.indexOf("\""+fieldName+"\"");
	    if (index != -1) {
		/// Se ha encontrado un elemento y podemos poner el texto.
		QTableWidgetItem *newItem = new QTableWidgetItem ( list2.at(index) );
		mui_list->setItem ( row, campo, newItem );
	    } // end if
	} // end for
	row++;
    } // end while
    file.close();
}

