#include "bgserver.h"

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include <QtCore/QThread>
#include <time.h>
#include <QtCore/QTimer>

#include "blfunctions.h"
#include "blpostgresqlclient.h"

bool g_semafor = false;

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



BgServer::BgServer() : QObject() {
    createServer();


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tiempo()));
    timer->start(1000);
}

BgServer::~BgServer()
{
    m_tcpServer->close();
    delete m_tcpServer;
}


void BgServer::createServer() {
    qDebug() << "createServer" << endl;
  
    m_tcpServer = new QTcpServer(this);

    if (!m_tcpServer->listen(QHostAddress::Any, 5899)) {
         qDebug() << m_tcpServer->errorString();
	 exit(1);
         return;
    }
    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(conection()));
}






void BgServer::conection()
{
    QTcpSocket *socket = m_tcpServer->nextPendingConnection();
    QHostAddress conectadofrom = socket->peerAddress();
    m_listaSockets.append(socket);

    qDebug() << "Nueva Conexion: " + conectadofrom.toString() << endl ;

    connect (socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect (socket, SIGNAL(readChannelFinished()), this, SLOT(readChannelFinished()));

    send("<COMCLEAN></COMCLEAN>\n", socket);

    qDebug() << "Envio COMCLEAN" << endl;
}


void BgServer::procesaCommand( const QString &texto, QTcpSocket *sock) {
   qDebug() << "procesaCommand " << texto << endl;
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
    nodosdb = docElem.elementsByTagName ( "USERNAME" );
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
    
    QString tablename="";
    nodosdb = docElem.elementsByTagName ( "TABLENAME" );
    for (int j = 0; j < nodosdb.count(); j++) {
	    tablename = nodosdb.item (j).toElement().text();
    } // end for
    
    QDomNodeList nodos = docElem.elementsByTagName ( "QUERY" );
    
    qDebug() << "Parametros: " << tablename << user << endl;
	    
	    
    BlPostgreSqlClient db; 
    db.inicializa(database, host, port, user, password, "", "");
    
    for (int j = 0; j < nodos.count(); j++) {
	QString query = nodos.item (j).toElement().text();
	db.runQuery(query);
    } // end for
    
    qDebug() << "END procesaCommand" << endl;
}

void BgServer::procesaDemand(const QString &texto, QTcpSocket *sock) {
    qDebug() << "procesaDemand " << texto << endl;
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
    nodosdb = docElem.elementsByTagName ( "USERNAME" );
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
    
    QString tablename="";
    nodosdb = docElem.elementsByTagName ( "TABLENAME" );
    for (int j = 0; j < nodosdb.count(); j++) {
	    tablename = nodosdb.item (j).toElement().text();
    } // end for
    
    QDomNodeList nodos = docElem.elementsByTagName ( "QUERY" );
    
    qDebug() << "Parametros: " << tablename << user << endl;
	    
	    
    BlPostgreSqlClient db; 
    db.inicializa(database, host, port, user, password, "", "");
    QString teto = "<RESPONSE><CLEARTABLE>"+tablename+"</CLEARTABLE><"+tablename+">\n";   
    int muylargo = 0;
    
    for (int j = 0; j < nodos.count(); j++) {
	    QString query = nodos.item (j).toElement().text();
	    
	    qDebug() << query << endl;
	    qDebug() << "Enviando ultima <RESPONSE>" << endl;
	    qDebug() << teto << endl;
	    
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
		  teto += "</"+tablename+"></RESPONSE>\n";
		  qDebug() << "Truncamos el mensaje " << endl;
		  send(teto, sock);
		  muylargo = 0;
		  teto = "<RESPONSE><"+tablename+">\n";
		} // end if
		muylargo++;
		
	    } // end while
	    delete cur;
    } // end for
    
    teto += "</"+tablename+"></RESPONSE>\n";
    send(teto, sock);
//    db.terminar(); // La destruccion de la variable la termina.
    qDebug() << "END procesaDemand" << endl;
}
  

void BgServer::readyRead() {
      QTcpSocket *socket = (QTcpSocket *) sender();
      readyRead(socket);
}  
  
void BgServer::readyRead(QTcpSocket *socket) {
    static QByteArray array = "";
    
    qDebug() << "readyRead" << endl;

    if (g_semafor) return;
    array += socket->readAll();
    qDebug() << "Lectura realizada" << endl;
    
    QString mensaje = "";
    QString texto(array);
    QString res = "";
    
    qDebug() << "Recibido: " << texto.left(60) << "...." << texto.right(10) << endl;

    if ( texto.contains("</DEMAND>")) {
            array = "";
            procesaDemand( texto, socket );
            // Enviado el ultimo pakete preparamos un nuevo pack.
            qApp->processEvents();
            send("<COMCLEAN></COMCLEAN>\n", socket);
    } else if (texto.contains("</COMMAND>")) {
            array = "";
            procesaCommand( texto, socket);
            qApp->processEvents();
            send("<COMCLEAN></COMCLEAN>\n", socket);
    } else if (texto.contains("</ACK>")) {
            mensaje = "Solicitud de cierre: "+ socket->peerAddress().toString() + "\n";
            mensaje += texto;
            array = "";
            m_listaSockets.removeAll(socket);
            socket->close();
            delete socket;
            qDebug() << mensaje << "Comunicacion terminada" << endl;
    } else {
            mensaje = "Mensaje desde: "+ socket->peerAddress().toString() + texto.left(10) + "..." + texto.right(10) + "\n";
    }// end if
    qApp->processEvents();
    qDebug() << "END readyRead" << endl;
}


void BgServer::readChannelFinished() {
    qDebug() << "Fin de la comunicacion" << endl;
    QTcpSocket *socket = (QTcpSocket *) sender();
    QString mensaje = "Fin de la comunicacion: "+ socket->peerAddress().toString() + "\n";
    qDebug() << mensaje << endl;
    m_listaSockets.removeAll( socket);


}

int BgServer::send(const QString & texto, QTcpSocket *sock) {
  
  g_semafor = true;
  qDebug() << "send" << texto.left(50) << "...." << texto.right(50) << endl;
          sock->write(texto.toLatin1());
          sock->flush();

          QString arr;
          int i = 0;
	  qDebug() << "Esperando PACK" << endl;
          do {
                // esperamos a que haya retorno de informacion. Segun el protocolo siempre va a haberla.
                while(sock->bytesAvailable() < 4) {
                    if (i++ == 50000) {
                            i = 0;
                    } // end if
                    qApp->processEvents();
                } // end while
                arr += sock->readLine();
		qDebug() << arr << endl;
		if (!arr.contains("</PACK>"))
		    qDebug() << "Linea Descartada" << arr << endl;
          } while (!arr.contains("</PACK>"));
	  
	  
  qDebug() << "END send" << endl;
  g_semafor = false;
  readyRead(sock);
return 0;
}


bool BgServer::conectaDB() {
    return true;
}

void BgServer::tiempo() {

}

void BgServer::on_mui_queryb_released() {
    conectaDB();

                        
#ifdef __DIALMA__
                        QSqlQueryModel *model = new QSqlQueryModel;
                        model->setQuery(ui->mui_query->toPlainText(), m_db);

                        QTableView *view = new QTableView;
                        view->setModel(model);
                        view->show();
#endif
}
