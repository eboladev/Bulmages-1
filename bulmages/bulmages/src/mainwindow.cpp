#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include <QtWidgets/QTableView>

#include <QtCore/QThread>
#include <time.h>
#include <QtCore/QTimer>

#include "blfunctions.h"
#include "blpostgresqlclient.h"

/// Escapa los minimos caracteres necesarios para
/// escribir a cadena a XML, sin usar CDATA, para ahorrar bytes y porque (me suena que) puede no
/// estar permitido en todos los lugares de un fichero xml segun esquemas o dtds especificos
QString blXMLEscape ( const QString& param )
{
    QString text = param;
    text.replace ( "&", "&amp;" );
    text.replace ( '"', "&quot;" );
    text.replace ( "<", "&lt;" );
    text.replace ( ">", "&gt;" );
    text.replace ( "\n", "<br />" );
    text.replace ( "'", "&apos;" );
    return text;
}

void sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}



MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createTrayIcon();
//    conectaDB();
    createServer();


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tiempo()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    m_tcpServer->close();
    delete m_tcpServer;
    delete ui;
}


void MainWindow::createServer() {

    m_tcpServer = new QTcpServer(this);

    if (!m_tcpServer->listen(QHostAddress::Any, 5899)) {
         qDebug() << m_tcpServer->errorString();
         close();
         return;
    }
    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(conection()));
}


void MainWindow::createTrayIcon()
{

    m_trayIcon = new QSystemTrayIcon ( this );
    m_trayIcon->setIcon ( QIcon ( ":/MAIN/bsyncserver.ico" ) );
    m_trayIcon->show();

    connect ( m_trayIcon, SIGNAL ( activated ( QSystemTrayIcon::ActivationReason ) ), this, SLOT ( activado ( QSystemTrayIcon::ActivationReason ) ) );
}


void MainWindow::activado ( QSystemTrayIcon::ActivationReason reason )
{

    if ( reason == QSystemTrayIcon::Trigger ) {
        // Click boton izquierdo
        if (isHidden())
            show();
        else
            hide();
    } // end if
}


void MainWindow::conection()
{
    QTcpSocket *socket = m_tcpServer->nextPendingConnection();
    QHostAddress conectadofrom = socket->peerAddress();
    m_listaSockets.append(socket);
    ui->mui_plainText->clear();
    ui->mui_plainText->appendPlainText("Nueva Conexion: " + conectadofrom.toString() + "\n");

    connect (socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect (socket, SIGNAL(readChannelFinished()), this, SLOT(readChannelFinished()));

/*    while(!conectaDB()) { // Conectamos con la base de datos al inicio del socket para no ocupar siempre el canal de base de datos.
        sleep(200);
        qApp->processEvents();
    } // end while
*/


    socket->write("<RESPONSE><AACK></AACK></RESPONSE>\n");
    socket->flush();
    qDebug() << "Envio AACK" << endl;
}


void MainWindow::procesaDemand(const QString &texto, QTcpSocket *sock) {
    qDebug() << texto << endl;
    QDomDocument doc ( "mydocument" );
    doc.setContent(texto);

    /// Iteramos para cada AutoForm y lo creamos haciendo todo lo necesario para que este funcione.
    QDomElement docElem = doc.documentElement();

    QString database="";
    QDomNodeList nodosdb = docElem.elementsByTagName ( "DATABASE" );
    for (int j = 0; j < nodosdb.count(); j++) {
	    database = nodosdb.item (j).toElement().text();
    } // end for

    QString user="";
    nodosdb = docElem.elementsByTagName ( "USER" );
    for (int j = 0; j < nodosdb.count(); j++) {
	    user = nodosdb.item (j).toElement().text();
    } // end for
    
    QString password="";
    nodosdb = docElem.elementsByTagName ( "PASSWORD" );
    for (int j = 0; j < nodosdb.count(); j++) {
	    password = nodosdb.item (j).toElement().text();
    } // end for
    
    QString host="localhost";
    nodosdb = docElem.elementsByTagName ( "HOST" );
    for (int j = 0; j < nodosdb.count(); j++) {
	    host = nodosdb.item (j).toElement().text();
    } // end for
    
    QString port="5433";
    nodosdb = docElem.elementsByTagName ( "PORT" );
    for (int j = 0; j < nodosdb.count(); j++) {
	    port = nodosdb.item (j).toElement().text();
    } // end for
    
    QDomNodeList nodos = docElem.elementsByTagName ( "QUERY" );
    BlPostgreSqlClient db; 
    db.inicializa(database, host, port, user, password, "", "");
    QString teto = "<RESPONSE>\n";   
    int muylargo = 0;
    
    for (int j = 0; j < nodos.count(); j++) {
	    QString query = nodos.item (j).toElement().text();
	    qDebug() << query << endl;
	    ui->mui_plainText->appendPlainText("Enviando ultima <RESPONSE>\n");
	    ui->mui_plainText->appendPlainText(teto+"\n");
	    qApp->processEvents();
	    BlDbRecordSet *cur = db.loadQuery(query);
	    while (!cur->eof()) {
		teto += "<RECORD>";
		for (int i=0; i < cur->numcampos(); i++) {
		  teto += "<"+ cur->fieldName(i) + ">";
		  teto += cur->value(cur->fieldName(i));
		  teto += "</"+ cur->fieldName(i) + ">\n";
		} // end for
		cur->nextRecord();
		teto += "</RECORD>\n";
		
		if (muylargo > 100) {
		  teto += "</RESPONSE>\n";
		  send(teto, sock);
		  muylargo = 0;
		  teto = "<RESPONSE>\n";
		} // end if
		muylargo++;
		
	    } // end while
	    delete cur;
    } // end for
    
    teto += "</RESPONSE>\n";
    // db.terminar();
    send(teto, sock);
}
  

void MainWindow::readyRead() {

    ui->mui_plainText->appendPlainText("Llego un mensaje\n");
    static QByteArray array = "";
    QTcpSocket *socket = (QTcpSocket *) sender();
    array += socket->readAll();
    QString mensaje = "";
    QString texto(array);
    ui->mui_plainText->appendPlainText(texto);
    QString res = "";

 

    if ( texto.contains("</DEMAND>")) {
	    procesaDemand( texto, socket );
            // Enviado el ultimo pakete preparamos un nuevo pack.

            qApp->processEvents();

            send("<NACK></NACK>\n", socket);
            array = "";
    } else if (texto.contains("</ACK>")) {
            mensaje = "Confirmacion completada desde: "+ socket->peerAddress().toString() + "\n";
            mensaje += texto;
            array = "";
            m_listaSockets.removeAll(socket);
            socket->close();
            delete socket;
    } else {
        mensaje = "Mensaje desde: "+ socket->peerAddress().toString() + texto.left(10) + "..." + texto.right(10) + "\n";
    }// end if
    // ui->mui_plainText->appendPlainText(mensaje);
    ui->mui_plainText->appendPlainText("Fin de llego un mensaje\n");
    qApp->processEvents();
}


void MainWindow::readChannelFinished() {

    QTcpSocket *socket = (QTcpSocket *) sender();
    QString mensaje = "Fin de la comunicacion: "+ socket->peerAddress().toString() + "\n";
    ui->mui_plainText->appendPlainText(mensaje);
    m_listaSockets.removeAll( socket);


}

int MainWindow::send(const QString & texto, QTcpSocket *sock) {
          sock->write(texto.toLatin1());
          sock->flush();

          QString arr;
          int i = 0;
          do {
                // esperamos a que haya retorno de informacion. Segun el protocolo siempre va a haberla.
                while(sock->bytesAvailable() < 4) {
                    if (i++ == 50000) {
                            ui->mui_plainText->appendPlainText("Esperando PACK:\n");
			    qDebug() << "Esperando PACK" << endl;
                            i = 0;
                    } // end if
                    qApp->processEvents();
                } // end while
                arr += sock->readLine();
                qDebug() << "Linea Descartada" << arr << endl;
          } while (!arr.contains("</PACK>"));

return 0;
}


bool MainWindow::conectaDB() {
    bool configPruebas = false;
#ifdef __DIALMA__
    if (configPruebas) {

            m_db = QSqlDatabase::addDatabase("QODBC","ttabloide");
            m_db.setHostName("192.168.10.46");
            m_db.setDatabaseName("Tabloide");

            m_db.setUserName("tb");
            m_db.setPassword("tb");
    } else {
            /*m_db = QSqlDatabase::addDatabase("QODBC","Dialma_SQL");
            m_db.setHostName("192.168.0.2");
            m_db.setDatabaseName("Dialma_SQL");
            m_db.setUserName("LogicPalma");
            m_db.setPassword("Cigol070");
*/

            QString server = "192.168.0.2";
            QString dbName = "Dialma_SQL";
            QString user = "pda";
            QString password = "D4r12m4n";
            QString connectionTemplate = "DRIVER={SQL SERVER};SERVER=%1;DATABASE=%2;";

            QString connectionString = connectionTemplate.arg(server).arg(dbName);
            m_db = QSqlDatabase::addDatabase("QODBC", "DialmaSQL");

            m_db.setDatabaseName(connectionString);
            m_db.setUserName(user);
            m_db.setPassword(password);


    }



    if (m_db.open()) {
        qDebug() << "Base de datos abierta\n";


// ==============================================
        return true;
    } else {
        m_db.close();
        QSqlDatabase::removeDatabase("DialmaSQL"); // will output a warning
        qDebug() << "Base de datos no abierta\n";
        return false;
    }

    qDebug() << QSqlDatabase::drivers();
    qDebug() <<  m_db.lastError().text();
#endif


}

void MainWindow::tiempo() {
    //qDebug() << "PAso el tiempo\n" << QString::number(cuenta++);
    // ui->mui_plainText->appendPlainText("Paso el tiempo\n");

}

void MainWindow::on_mui_queryb_released() {
    conectaDB();
    // ==============================================
            // Prueba de conexion
            /*
                        QSqlQuery query1("SELECT * FROM ARTDAT ", m_db);
                        int a=0;
                        while (query1.next()) {
                            ui->mui_plainText->appendPlainText(query1.value(0).toString()+ "\n");
                            a ++;
                        } // end while
                        ui->mui_plainText->appendPlainText("Valor de a:" + QString::number(a) + "\n");
                        ui->mui_plainText->appendPlainText(query1.executedQuery());
                        ui->mui_plainText->appendPlainText(query1.lastError().databaseText());
    */

        //                model->setQuery("SELECT * FROM TARDAT WHERE TAR_CODART NOT LIKE '35%' AND TAR_CODTAR  IN (SELECT DISTINCT CLI_TARIFA FROM CLIDAT)", m_db);
        //                model->setQuery("SELECT  strftime('%d/%m/%Y %H:%M', '01/01/2012 12:00' ) AS tbl ", m_db);

                        /// Este query saca los precios correctos para los 531 precios y descuentos especiales.
                        //model->setQuery("SELECT * FROM DCADAT LEFT JOIN ARTDAT ON DCADAT.DCA_CODART = ARTDAT.ART_CODIGO LEFT JOIN CLIDAT ON DCADAT.DCA_CODCLI = CLIDAT.CLI_CODIGO WHERE DCA_FECFIN > GETDATE() AND DCA_CODART= ARTDAT.ART_CODIGO AND DCA_CODART NOT LIKE '35%' ORDER BY CLI_CODIGO", m_db);


                       // model->setQuery("SELECT * FROM (SELECT * FROM ARTDAT WHERE ART_CODIGO ='0101005') AS T6 " \
                                        " LEFT JOIN (SELECT * FROM TARDAT WHERE TAR_CODTAR IN (SELECT CLI_TARIFA FROM CLIDAT WHERE CLI_CODIGO='00096')) AS T2 ON T2.TAR_CODART = T6.ART_CODIGO " \
                                        " LEFT JOIN (SELECT * FROM CLIDAT WHERE CLI_CODIGO='00096') AS T3 ON T3.CLI_TARIFA = T2.TAR_CODTAR " \
                                        " LEFT JOIN DCADAT ON DCA_CODART = T6.ART_CODIGO "\
                                        " ORDER BY ART_CODIGO", m_db);

                        // model->setQuery("SELECT * FROM DCADAT LEFT JOIN ARTDAT ON DCADAT.DCA_CODART = ARTDAT.ART_CODIGO LEFT JOIN CLIDAT ON DCADAT.DCA_CODCLI = CLIDAT.CLI_CODIGO WHERE DCA_FECFIN > GETDATE() AND DCA_CODART NOT LIKE '35%' AND DCA_CODCLI='00096' ORDER BY CLI_CODIGO", m_db);

                        // model->setQuery("SELECT * FROM DCADAT LEFT JOIN ARTDAT ON DCADAT.DCA_CODART = ARTDAT.ART_CODIGO LEFT JOIN CLIDAT ON DCADAT.DCA_CODCLI = CLIDAT.CLI_CODIGO WHERE DCA_FECFIN > GETDATE() AND DCA_CODART NOT LIKE '35%' AND DCA_DESCUE <> 0 ORDER BY CLI_CODIGO", m_db);


                        //model->setQuery("SELECT * FROM CLIDAT WHERE CLI_CODIGO IN (SELECT VIS_CODCLI FROM VISDAT WHERE VIS_CODVEN='00001')", m_db);
                        //model->setQuery("SELECT * FROM CLIDAT WHERE  CLI_CODIGO = '03980'", m_db);
                        //model->setQuery("SELECT * FROM CLIDAT WHERE  CLI_CODIGO = '04109'", m_db);
                        //model->setQuery("SELECT * FROM CLIDAT WHERE CLI_CODIGO IN (SELECT VIS_CODCLI FROM VISDAT WHERE VIS_CODVEN='00001') ", m_db);

                        //model->setQuery("SELECT * FROM VISDAT  LEFT JOIN CLIDAT ON VISDAT.VIS_CODCLI = CLIDAT.CLI_CODIGO WHERE VIS_CODVEN='00001' ", m_db);
                        /*
                        QString query = "SELECT *, (LPC_UNIDAD + LPC_LOTES * ART_LOTE1U)*COALESCE(pesp, TAR_PRECIO *(100-DCA_DESCUE)/100 )  AS total FROM LinPedidosPda "; //  ---
                            query += " LEFT JOIN ARTDAT ON LinPedidosPda.LPC_CODART = ARTDAT.ART_CODIGO ";
                            query += " LEFT JOIN (SELECT * FROM TARDAT WHERE TAR_CODTAR IN (SELECT CLI_TARIFA FROM CLIDAT WHERE CLI_CODIGO='01264')) AS t3 ON t3.TAR_CODART=LPC_CODART";
                            query += " LEFT JOIN (SELECT DCA_DESCUE, DCA_CODART FROM DCADAT WHERE DCA_CODCLI='01264' AND DCA_TIPDTO=1) AS t4  ON t4.DCA_CODART= SUBSTRING(LPC_CODART,0,5)"; //SUBSTR(LPC_CODART,0,5)";
                            query += " LEFT JOIN (SELECT DCA_DESCUE AS pesp FROM DCADAT WHERE DCA_CODCLI='01264' AND DCA_TIPDTO=3 ) AS t5 ON DCA_CODART=LPC_CODART";
                            query += " WHERE LPC_PEDIDO='0000001779'";
                            */
        /*
                            QString query = "SELECT *, (1)*COALESCE(pesp, TAR_PRECIO *(100-DCA_DESCUE)/100, ultpre )  AS total FROM ARTDAT "; //  ---
                                query += " LEFT JOIN (SELECT * FROM TARDAT WHERE TAR_CODTAR IN (SELECT CLI_TARIFA FROM CLIDAT WHERE CLI_CODIGO='00620')) AS t3 ON t3.TAR_CODART=ART_CODIGO";
                                query += " LEFT JOIN (SELECT DCA_DESCUE, DCA_CODART FROM DCADAT WHERE DCA_CODCLI='00620' AND DCA_TIPDTO=1) AS t4  ON t4.DCA_CODART= SUBSTRING(ART_CODIGO,0,5)"; //SUBSTR(LPC_CODART,0,5)";
                                query += " LEFT JOIN (SELECT DCA_DESCUE AS pesp FROM DCADAT WHERE DCA_CODCLI='00620' AND DCA_TIPDTO=3 ) AS t5 ON DCA_CODART=ART_CODIGO";
                                query += " LEFT JOIN (SELECT TAR_CODART, TAR_PRECIO AS ultpre FROM TARDAT WHERE TAR_CODTAR ='01') AS t6 ON t6.TAR_CODART= ART_CODIGO ";
                            query += " WHERE ARTDAT.ART_CODIGO='2201210' ";
        */
                            //query = "SELECT * FROM LinPedidosPda";
                            // QString query= "SELECT * FROM VENDAT";
                        //QString query = "SELECT * FROM LinPedidosPda";
                         //  QString query = "SELECT * FROM CHCDAT WHERE CHC_CODART NOT LIKE '35%' AND CHC_CODCLI IN (SELECT VIS_CODCLI FROM VISDAT WHERE VIS_CODVEN='00001')";
                        //                 QString query = "SELECT * FROM ARTDAT";
                        // QString query = "SELECT * FROM CLIDAT";
//                        QString query = "SELECT * FROM IOGDATPDA ORDER BY IOG_NUMERO";
        //                QString query = "SELECT * FROM cabpedidospda";
                        //QString query = "SELECT * FROM VISDAT";
                        //Query
                        
#ifdef __DIALMA__
                        QSqlQueryModel *model = new QSqlQueryModel;
                        model->setQuery(ui->mui_query->toPlainText(), m_db);

                        //model->setQuery("SELECT * FROM INCDAT", m_db);
                        //model->setQuery("SELECT * FROM DCADAT WHERE DCA_CODCLI = '04109'", m_db);
                        //Display
                        QTableView *view = new QTableView;
                        view->setModel(model);
                        view->show();
#endif
}
