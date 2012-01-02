#include <QWidget>

#include <vector>
#include "devolucion2.h"
#include "blfixed.h"


/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;


Devolucion::Devolucion ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( ( QDialog* ) this );
    this->mui_tablesFrame->setDisabled ( TRUE );

    m_ticket = NULL;
    /*
        m_value = 0;
        base basesimp;
        base basesimpreqeq;
        BlDbRecord *linea;

        Ticket *tick = emp->ticketActual();

        /// Impresion de los contenidos.
        QString l;
        BlFixed irpf ( "0" );

        BlDbRecordSet *cur = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
        if ( cur ) {
            if ( !cur->eof() ) {
                irpf = BlFixed ( cur->value( "valor" ) );
            } // end if
            delete cur;
        } // end if


        BlFixed descuentolinea ( "0.00" );
        for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
            linea = tick->listaLineas() ->at ( i );
            BlFixed cant ( linea->dbValue ( "cantlalbaran" ) );
            BlFixed pvpund ( linea->dbValue ( "pvplalbaran" ) );
            BlFixed desc1 ( linea->dbValue ( "descuentolalbaran" ) );
            BlFixed cantpvp = cant * pvpund;
            BlFixed base = cantpvp - cantpvp * desc1 / 100;
            descuentolinea = descuentolinea + ( cantpvp * desc1 / 100 );
            basesimp[linea->dbValue ( "ivalalbaran" ) ] = basesimp[linea->dbValue ( "ivalalbaran" ) ] + base;
            basesimpreqeq[linea->dbValue ( "reqeqlalbaran" ) ] = basesimpreqeq[linea->dbValue ( "reqeqlalbaran" ) ] + base;
        } // end for

        BlFixed basei ( "0.00" );
        base::Iterator it;
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
            basei = basei + it.value();
        } // end for

        /// Calculamos el total de los descuentos.
        /// De momento aqui no se usan descuentos generales en venta.
        BlFixed porcentt ( "0.00" );

        /// Calculamos el total de base imponible.
        BlFixed totbaseimp ( "0.00" );
        BlFixed parbaseimp ( "0.00" );
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
            if ( porcentt > BlFixed ( "0.00" ) ) {
                parbaseimp = it.value() - it.value() * porcentt / 100;
            } else {
                parbaseimp = it.value();
            } // end if
            totbaseimp = totbaseimp + parbaseimp;
        } // end for

        /// Calculamos el total de IVA.
        BlFixed totiva ( "0.00" );
        BlFixed pariva ( "0.00" );
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
            BlFixed piva ( it.key().toAscii().constData() );
            if ( porcentt > BlFixed ( "0.00" ) ) {
                pariva = ( it.value() - it.value() * porcentt / 100 ) * piva / 100;
            } else {
                pariva = it.value() * piva / 100;
            } // end if
            totiva = totiva + pariva;
        } // end for

        /// Calculamos el total de recargo de equivalencia.
        BlFixed totreqeq ( "0.00" );
        BlFixed parreqeq ( "0.00" );
        for ( it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it ) {
            BlFixed preqeq ( it.key().toAscii().constData() );
            if ( porcentt > BlFixed ( "0.00" ) ) {
                parreqeq = ( it.value() - it.value() * porcentt / 100 ) * preqeq / 100;
            } else {
                parreqeq = it.value() * preqeq / 100;
            } // end if
            totreqeq = totreqeq + parreqeq;
        } // end for

        BlFixed totirpf = totbaseimp * irpf / 100;

        BlFixed total = totiva + totbaseimp + totreqeq - totirpf;

        mui_total->setText(total.toQString());
    */
}


Devolucion::~Devolucion()
{}




void Devolucion::on_mui_refLineEdit_returnPressed()
{
    if ( m_ticket ) {
        delete m_ticket;
        m_ticket = NULL;
    } // end if

    QString query1 = "SELECT * FROM albaran WHERE refalbaran = '" + mui_refLineEdit->text() + "'";

    BlDbRecordSet *curs = mainCompany()->loadQuery ( query1 );
    if ( !curs->eof() ) {
        m_ticket = new BtTicket ( mainCompany(), NULL );
        m_ticket->load ( curs->value( "idalbaran" ) );
        this->mui_tablesFrame->setEnabled ( TRUE );
        pintar();

    } else {
        this->mui_tablesFrame->setDisabled ( TRUE );
    }
    delete curs;

}
struct lineastr {
    float cantidad;
    QString ref;
    float precioUnidad;
    int linea;
};

void Devolucion::pintar()
{
    BlDbRecord *item;

    std::vector<lineastr> lineasPositivas;
    std::vector<lineastr> lineasNegativas;

    for ( int i = 0; i < m_ticket->listaLineas() ->size(); ++i ) {
        item = m_ticket->listaLineas() ->at ( i );
        float cantidadLinea = QString ( item->dbValue ( "cantlalbaran" ) ).toFloat();
        lineastr newlinea;
        newlinea.cantidad = cantidadLinea;
        newlinea.ref = item->dbValue ( "codigocompletoarticulo" );
        newlinea.precioUnidad = item->dbValue ( "pvplalbaran" ).toFloat();
        newlinea.linea = i;

        if ( cantidadLinea > 0 ) {
            lineasPositivas.push_back ( newlinea );
        } else {
            lineasNegativas.push_back ( newlinea );
        } // end if
    } // end for

    for ( unsigned int i = 0; i < lineasPositivas.size(); i++ ) {
        for ( unsigned int j = 0; j < lineasNegativas.size(); j++ ) {
            if ( ( lineasPositivas[i].ref == lineasNegativas[j].ref ) && ( lineasPositivas[i].precioUnidad == lineasNegativas[j].precioUnidad ) ) {
                lineasPositivas[i].cantidad += lineasNegativas[j].cantidad;
            } // end if
        } // end for
    } // end for

    delete mui_ticketTable;
    delete mui_devolverTable;

    mui_ticketTable = new QTableWidget ( mui_ticketFrame );
    mui_ticketTable->setObjectName ( QString::fromUtf8 ( "mui_ticketTable" ) );
    mui_ticketTable->setEditTriggers ( QAbstractItemView::NoEditTriggers );
    mui_ticketTable->setTabKeyNavigation ( false );
    mui_ticketTable->setProperty ( "showDropIndicator", QVariant ( false ) );
    mui_ticketTable->setDragDropOverwriteMode ( false );
    mui_ticketTable->setSelectionMode ( QAbstractItemView::SingleSelection );
    mui_ticketTable->setSelectionBehavior ( QAbstractItemView::SelectRows );
    mui_ticketTable->setCornerButtonEnabled ( false );
    verticalLayout_13->addWidget ( mui_ticketTable );

    mui_devolverTable = new QTableWidget ( mui_devolverFrame );
    mui_devolverTable->setObjectName ( QString::fromUtf8 ( "mui_devolverTable" ) );
    mui_devolverTable->setEditTriggers ( QAbstractItemView::NoEditTriggers );
    mui_devolverTable->setTabKeyNavigation ( false );
    mui_devolverTable->setProperty ( "showDropIndicator", QVariant ( false ) );
    mui_devolverTable->setDragDropOverwriteMode ( false );
    mui_devolverTable->setSelectionMode ( QAbstractItemView::SingleSelection );
    mui_devolverTable->setSelectionBehavior ( QAbstractItemView::SelectRows );
    mui_devolverTable->setCornerButtonEnabled ( false );
    verticalLayout_12->addWidget ( mui_devolverTable );

    QObject::connect ( this->mui_ticketTable, SIGNAL ( cellDoubleClicked ( int, int ) ),
                       this, SLOT ( ticketTableDoubleclick ( int , int ) ) );
    QObject::connect ( this->mui_devolverTable, SIGNAL ( cellDoubleClicked ( int, int ) ),
                       this, SLOT ( devolverTableDoubleclick ( int , int ) ) );

    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM cliente WHERE idcliente=" + m_ticket->dbValue ( "idcliente" ) );
    if ( !cur->eof() ) {
        mui_clienteLabel->setText ( cur->value( "cifcliente" ).toAscii() + " " + cur->value( "nomcliente" ).toAscii() );
    }
    delete cur;
    mui_fechaLabel->setText ( m_ticket->dbValue ( "fechalbaran" ) + " " + m_ticket->dbValue ( "horaalbaran" ) );

    refreshDevolver();


    QTableWidgetItem *tableItem;

    int numCols = 5;

    if ( mui_ticketTable->columnCount() < numCols )
        mui_ticketTable->setColumnCount ( numCols );

    for ( int i = 0; i < numCols; i++ ) {
        QTableWidgetItem *__colItem = new QTableWidgetItem();
        mui_ticketTable->setHorizontalHeaderItem ( i, __colItem );
    }

    mui_ticketTable->horizontalHeaderItem ( 0 )->setText ( _ ( "Unds." ) );
    mui_ticketTable->horizontalHeaderItem ( 1 )->setText ( _ ( "Producto" ) );
    mui_ticketTable->horizontalHeaderItem ( 2 )->setText ( _ ( "P.U." ) );
    mui_ticketTable->horizontalHeaderItem ( 3 )->setText ( _ ( "Total" ) );
    mui_ticketTable->horizontalHeaderItem ( 4 )->setText ( _ ( "Linea" ) );
    mui_ticketTable->setColumnHidden ( 4, TRUE );

    if ( mui_devolverTable->columnCount() < numCols )
        mui_devolverTable->setColumnCount ( numCols );

    for ( int i = 0; i < numCols; i++ ) {
        QTableWidgetItem *__colItem = new QTableWidgetItem();
        mui_devolverTable->setHorizontalHeaderItem ( i, __colItem );
    }

    mui_devolverTable->horizontalHeaderItem ( 0 )->setText ( _ ( "Unds." ) );
    mui_devolverTable->horizontalHeaderItem ( 1 )->setText ( _ ( "Producto" ) );
    mui_devolverTable->horizontalHeaderItem ( 2 )->setText ( _ ( "P.U." ) );
    mui_devolverTable->horizontalHeaderItem ( 3 )->setText ( _ ( "Total" ) );
    mui_devolverTable->horizontalHeaderItem ( 4 )->setText ( _ ( "Linea" ) );
    mui_devolverTable->setColumnHidden ( 4, TRUE );


    int tmp = 0;
    for ( unsigned int i = 0; i < lineasPositivas.size(); i++ ) {
        if ( lineasPositivas[i].cantidad > 0 ) {
            this->mui_ticketTable->insertRow ( tmp );
            item = m_ticket->listaLineas() ->at ( lineasPositivas[i].linea );

            tableItem = new QTableWidgetItem ( BlFixed ( QString::number ( lineasPositivas[i].cantidad ) ).toQString() );
            this->mui_ticketTable->setItem ( tmp, 0, tableItem );

            tableItem = new QTableWidgetItem ( item->dbValue ( "nomarticulo" ) );
            this->mui_ticketTable->setItem ( tmp, 1, tableItem );

            tableItem = new QTableWidgetItem ( item->dbValue ( "pvplalbaran" ) );
            this->mui_ticketTable->setItem ( tmp, 2, tableItem );

            BlFixed totalLinea ( "0.00" );
            totalLinea = BlFixed ( item->dbValue ( "pvplalbaran" ) ) * lineasPositivas[i].cantidad;
            tableItem = new QTableWidgetItem ( totalLinea.toQString() );
            this->mui_ticketTable->setItem ( tmp, 3, tableItem );

            tableItem = new QTableWidgetItem ( QString::number ( i ) );
            this->mui_ticketTable->setItem ( tmp, 4, tableItem );
            tmp++;
        }
    }

    /*for ( int i = 0; i < m_ticket->listaLineas() ->size(); ++i )
    {
     this->mui_ticketTable->insertRow ( i );
     item = m_ticket->listaLineas() ->at ( i );

     tableItem = new QTableWidgetItem ( item->dbValue ( "cantlalbaran" ) );
     this->mui_ticketTable->setItem ( i, 0, tableItem );

     tableItem = new QTableWidgetItem ( item->dbValue ( "nomarticulo" ) );
     this->mui_ticketTable->setItem ( i, 1, tableItem );

     tableItem = new QTableWidgetItem ( item->dbValue ( "pvplalbaran" ) );
     this->mui_ticketTable->setItem ( i, 2, tableItem );

     BlFixed totalLinea ( "0.00" );
     totalLinea = BlFixed ( item->dbValue ( "cantlalbaran" ) ) * BlFixed ( item->dbValue ( "pvplalbaran" ) );
     tableItem = new QTableWidgetItem ( totalLinea.toQString() );
     this->mui_ticketTable->setItem ( i, 3, tableItem );

     tableItem = new QTableWidgetItem ( QString::number ( i ) );
     this->mui_ticketTable->setItem ( i, 4, tableItem );

    }// end for*/
}

void  Devolucion::anadir ( int prow )
{
    int row = prow;
    BlFixed unidades ( mui_ticketTable->item ( row, 0 )->text() );
    QTableWidgetItem *tableItem;
    if ( unidades > 0 ) {
        int rowCount = 0;
        if ( mui_devolverTable->rowCount() > 0 )
            rowCount = mui_devolverTable->rowCount();

        int devRow = rowCount;
        for ( int i = 0; i < rowCount; i++ ) {
            if ( BlFixed ( mui_ticketTable->item ( row, 4 )->text() ) == BlFixed ( mui_devolverTable->item ( i, 4 )->text() ) ) {
                devRow = i;
                i = rowCount;
            }
        }

        if ( devRow == rowCount ) {
            mui_devolverTable->insertRow ( devRow );

            tableItem = new QTableWidgetItem ( BlFixed ( QString ( "1" ) ).toQString() );
            mui_devolverTable->setItem ( devRow, 0, tableItem );

            tableItem = new QTableWidgetItem ( mui_ticketTable->item ( row, 1 )->text() );
            mui_devolverTable->setItem ( devRow, 1, tableItem );

            tableItem = new QTableWidgetItem ( mui_ticketTable->item ( row, 2 )->text() );
            mui_devolverTable->setItem ( devRow, 2, tableItem );

            tableItem = new QTableWidgetItem ( mui_ticketTable->item ( row, 2 )->text() );
            mui_devolverTable->setItem ( devRow, 3, tableItem );

            tableItem = new QTableWidgetItem ( mui_ticketTable->item ( row, 4 )->text() );
            mui_devolverTable->setItem ( devRow, 4, tableItem );
        } else {
            tableItem = new QTableWidgetItem ( ( BlFixed ( mui_devolverTable->item ( devRow, 0 )->text() ) + 1 ).toQString() );
            mui_devolverTable->setItem ( devRow, 0, tableItem );

            tableItem = new QTableWidgetItem ( ( BlFixed ( mui_devolverTable->item ( devRow, 0 )->text() ) *BlFixed ( mui_devolverTable->item ( devRow, 2 )->text() ) ).toQString() );
            mui_devolverTable->setItem ( devRow, 3, tableItem );

        }

        if ( unidades > 1 ) {
            tableItem = new QTableWidgetItem ( ( unidades - 1 ).toQString() );
            mui_ticketTable->setItem ( row, 0, tableItem );
            tableItem = new QTableWidgetItem ( ( ( unidades - 1 ) *BlFixed ( mui_ticketTable->item ( row, 2 )->text() ) ).toQString() );
            mui_ticketTable->setItem ( row, 3, tableItem );
        } else {
            mui_ticketTable->removeRow ( row );
        }
    }
    refreshDevolver();

}

void  Devolucion::quitar ( int prow )
{
    int row = prow;
    BlFixed unidades ( mui_devolverTable->item ( row, 0 )->text() );
    QTableWidgetItem *tableItem;
    if ( unidades > 0 ) {
        int rowCount = 0;
        if ( mui_ticketTable->rowCount() > 0 )
            rowCount = mui_ticketTable->rowCount();

        int tickRow = rowCount;
        for ( int i = 0; i < rowCount; i++ ) {
            if ( BlFixed ( mui_devolverTable->item ( row, 4 )->text() ) == BlFixed ( mui_ticketTable->item ( i, 4 )->text() ) ) {
                tickRow = i;
                i = rowCount;
            }
        }

        if ( tickRow == rowCount ) {
            mui_ticketTable->insertRow ( tickRow );

            tableItem = new QTableWidgetItem ( BlFixed ( QString ( "1" ) ).toQString() );
            mui_ticketTable->setItem ( tickRow, 0, tableItem );

            tableItem = new QTableWidgetItem ( mui_devolverTable->item ( row, 1 )->text() );
            mui_ticketTable->setItem ( tickRow, 1, tableItem );

            tableItem = new QTableWidgetItem ( mui_devolverTable->item ( row, 2 )->text() );
            mui_ticketTable->setItem ( tickRow, 2, tableItem );

            tableItem = new QTableWidgetItem ( mui_devolverTable->item ( row, 2 )->text() );
            mui_ticketTable->setItem ( tickRow, 3, tableItem );

            tableItem = new QTableWidgetItem ( mui_devolverTable->item ( row, 4 )->text() );
            mui_ticketTable->setItem ( tickRow, 4, tableItem );
        } else {
            tableItem = new QTableWidgetItem ( ( BlFixed ( mui_ticketTable->item ( tickRow, 0 )->text() ) + 1 ).toQString() );
            mui_ticketTable->setItem ( tickRow, 0, tableItem );

            tableItem = new QTableWidgetItem ( ( BlFixed ( mui_ticketTable->item ( tickRow, 0 )->text() ) *BlFixed ( mui_ticketTable->item ( tickRow, 2 )->text() ) ).toQString() );
            mui_ticketTable->setItem ( tickRow, 3, tableItem );

        }

        if ( unidades > 1 ) {
            tableItem = new QTableWidgetItem ( ( unidades - 1 ).toQString() );
            mui_devolverTable->setItem ( row, 0, tableItem );
            tableItem = new QTableWidgetItem ( ( ( unidades - 1 ) *BlFixed ( mui_devolverTable->item ( row, 2 )->text() ) ).toQString() );
            mui_devolverTable->setItem ( row, 3, tableItem );
        } else {
            mui_devolverTable->removeRow ( row );
        }
    }
    refreshDevolver();
}

void Devolucion::on_mui_todoButton_clicked()
{

}
void Devolucion::on_mui_anadirButton_clicked()
{
    if ( mui_ticketTable->currentItem () != NULL ) {
        int row = mui_ticketTable->currentRow ();
        anadir ( row );
    }
}
void Devolucion::on_mui_sacarButton_clicked()
{

    if ( mui_devolverTable->currentItem () != NULL ) {
        int row = mui_devolverTable->currentRow ();
        quitar ( row );
    }
}
void Devolucion::on_mui_cancelarButton_clicked() {
    this->close();
}

void Devolucion::on_mui_valeButton_clicked() {
    QMessageBox::information ( this, _ ( "NO IMPLEMENTADO" ),
                                         QString ( "NO IMPLEMENTADO" ),
                                         QMessageBox::Ok,
                                         QMessageBox::Ok );
}

void Devolucion::on_mui_efectivoButton_clicked() {
    if ( m_ticket == NULL ) return;
    if ( m_ticket->dbValue ( "idalbaran" ).isEmpty() ) return;


    int ret = QMessageBox::information ( this, QString ( "Confirmacion" ),
                                         QString ( "A DEVOLVER\n" + mui_totalLineEdit->text() ),
                                         QMessageBox::Ok | QMessageBox::Cancel,
                                         QMessageBox::Ok );

    if ( ret == QMessageBox::Ok ) {
        for ( int i = 0; i < mui_devolverTable->rowCount();i++ ) {
            int numlinea = QString ( mui_devolverTable->item ( i, 4 )->text() ).toInt();
            BlDbRecord *item = m_ticket->listaLineas() ->at ( numlinea );
            BlDbRecord *nitem = m_ticket->agregarLinea();
            QList<BlDbField *> *lista = item->lista();
            for ( int j = 0; j < lista->size(); ++j ) {
                BlDbField * camp = lista->at ( j );
                if ( camp->fieldName() != "numlalbaran" ) {
                    nitem->setDbValue ( camp->fieldName(), camp->fieldValue() );
                }
                if ( camp->fieldName() == "cantlalbaran" && camp->fieldValue().toFloat() > 0 ) {
                    nitem->setDbValue ( camp->fieldName(), "-" + mui_devolverTable->item ( i, 0 )->text() );
                }
            } // end for
        } // end for

        m_ticket->save();

        ( ( QDialog* ) this )->close();
    } // end if
}

void Devolucion::ticketTableDoubleclick ( int row, int column ) {
    anadir ( row );
}

void Devolucion::devolverTableDoubleclick ( int row, int column ) {
    quitar ( row );
}

void Devolucion::refreshDevolver() {
    base basesimp;
    base basesimpreqeq;
    BlDbRecord *linea;
    BlFixed irpf ( "0" );
    BlFixed descuentolinea ( "0.00" );
    for ( int i = 0; i < mui_devolverTable->rowCount(); ++i ) {
        int numlinea = ( int ) ( QString ( mui_devolverTable->item ( i, 4 )->text() ).toInt() );
        linea = m_ticket->listaLineas() ->at ( numlinea );
        BlFixed cant ( mui_devolverTable->item ( i, 0 )->text() );
        BlFixed pvpund ( linea->dbValue ( "pvplalbaran" ) );
        BlFixed desc1 ( linea->dbValue ( "descuentolalbaran" ) );
        BlFixed cantpvp = cant * pvpund;
        BlFixed base = cantpvp - cantpvp * desc1 / 100;
        descuentolinea = descuentolinea + ( cantpvp * desc1 / 100 );
        basesimp[linea->dbValue ( "ivalalbaran" ) ] = basesimp[linea->dbValue ( "ivalalbaran" ) ] + base;
        basesimpreqeq[linea->dbValue ( "reqeqlalbaran" ) ] = basesimpreqeq[linea->dbValue ( "reqeqlalbaran" ) ] + base;
    } // end for

    BlFixed basei ( "0.00" );
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.value();
    } // end for

    /// Calculamos el total de los descuentos.
    /// De momento aqui no se usan descuentos generales en venta.
    BlFixed porcentt ( "0.00" );
    /*
        BlDbSubFormRecord *linea1;
        if (m_listadescuentos->rowCount()) {
            for (int i = 0; i < m_listadescuentos->rowCount(); ++i) {
                linea1 = m_listadescuentos->lineaat(i);
                BlFixed propor(linea1->dbValue("proporcion" + m_listadescuentos->tableName()).toAscii().constData());
                porcentt = porcentt + propor;
            } // end for
        } // end if
    */

/// Calculamos el total de base imponible.
    BlFixed totbaseimp ( "0.00" );
    BlFixed parbaseimp ( "0.00" );
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        if ( porcentt > BlFixed ( "0.00" ) ) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        //html1 += "Base Imp " + it.key() + "% " + parbaseimp.toQString() + "<BR>";
        totbaseimp = totbaseimp + parbaseimp;
    } // end for

    /// Calculamos el total de IVA.
    BlFixed totiva ( "0.00" );
    BlFixed pariva ( "0.00" );
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        BlFixed piva ( it.key().toAscii().constData() );
        if ( porcentt > BlFixed ( "0.00" ) ) {
            pariva = ( it.value() - it.value() * porcentt / 100 ) * piva / 100;
        } else {
            pariva = it.value() * piva / 100;
        } // end if
        //html1 += "IVA " + it.key() + "% " + pariva.toQString() + "<BR>";
        totiva = totiva + pariva;
    } // end for

    /// Calculamos el total de recargo de equivalencia.
    BlFixed totreqeq ( "0.00" );
    BlFixed parreqeq ( "0.00" );
    for ( it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it ) {
        BlFixed preqeq ( it.key().toAscii().constData() );
        if ( porcentt > BlFixed ( "0.00" ) ) {
            parreqeq = ( it.value() - it.value() * porcentt / 100 ) * preqeq / 100;
        } else {
            parreqeq = it.value() * preqeq / 100;
        } // end if
        //html1 += "R.Eq " + it.key() + "% " + parreqeq.toQString() + "<BR>";
        totreqeq = totreqeq + parreqeq;
    } // end for



    BlFixed totirpf = totbaseimp * irpf / 100;

    //html1 += "<B>Base Imp. " + totbaseimp.toQString() + "<BR>";
    //html1 += "<B>IVA. " + totiva.toQString() + "<BR>";
    //html1 += "<B>IRPF. " + totirpf.toQString() + "<BR>";

    BlFixed total = totiva + totbaseimp + totreqeq - totirpf;
    mui_totalLineEdit->setText ( total.toQString() + " €" );
    //html1 += "<B>Total: " + total.toQString() + "<BR>";
}

