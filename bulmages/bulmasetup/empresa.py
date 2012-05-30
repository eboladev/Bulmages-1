# -*- coding: utf-8 -*-

import sys
import os
from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from plugins import PluginsBulmaSetup
import plugins
from tempfile import gettempdir
import functions
# Libreria de acceso a bases de datos PostgreSQL
import psycopg2

class Empresa(QtGui.QDialog, PluginsBulmaSetup):
    def __init__(self, database, parent = None):
        PluginsBulmaSetup.__init__(self)
        QtGui.QDialog.__init__(self,parent)
        self.database = database

        # Establecemos cual va a ser la base de datos con la que trabajaremos todo el rato
        self.process = QtCore.QProcess()
        
        #Establecemos un parametro que permite mostrar o no mostrar errores. En la actualizacon hay errores que son normales y no se deberian mostrar, por eso este parametro
        self.mostrarErrores = "TRUE"

        """Hacemos una conexion de prueba para comprobar si esta creado el usuario ROOT en PostgreSQL
        Ya que es necesario para poder administrar los permisos de las Bases de Datos.
        Si no existe, saltara un QMessageBox preguntando si se quiere crear el usuario ROOT."""

        try:
            conn = psycopg2.connect("dbname='template1' user='root' password='password'")
            conn.close()
        except:
            Yes = 'Si'
            No = 'No'
            message = QtGui.QMessageBox(self)
            message.setText(QtGui.QApplication.translate("Empresa", "Desea agregar el usuario <b>root</b> a PostgreSQL?<br>Este usuario es necesario para poder administrar PostgreSQL.<br>La contrase&ntilde;a de este usuario sera \'password\'.<br><u><b>Borre este usuario al terminar la configuracion de BulmaGes, ya que genera un grave agujero de seguridad.</b></u>", None, QtGui.QApplication.UnicodeUTF8))
            message.setWindowTitle('Atencion!')
            message.setIcon(QtGui.QMessageBox.Warning)
            message.addButton(Yes, QtGui.QMessageBox.AcceptRole)
            message.addButton(No, QtGui.QMessageBox.RejectRole)
            message.exec_()
            respuesta = message.clickedButton().text()
            if respuesta == Yes:
                comandoroot = functions.as_postgres + "createuser -s root" + functions.end_sql 
                self.process.start(comandoroot)
                self.process.waitForFinished(-1)
                self.execQuerySinConsola ("ALTER USER ROOT WITH password 'password'")
            else:
                print "Para poder modificar los permisos de las bases de datos es necesario agregar el usuario ROOT a PostgreSQL"
                print "Cerrando la aplicacion"
                sys.exit()

        try:
            conn = psycopg2.connect("dbname='template1' user='root' password='password'")
        except:
	    # Get the most recent exception
	    exceptionType, exceptionValue, exceptionTraceback = sys.exc_info()
	    # Exit the script and print an error telling what happened.
	    sys.exit("Database connection failed!\n ->%s" % (exceptionValue))


        conn.close()


        self.connect(self.process, SIGNAL("readyReadStandardOutput()"), self.readOutput)
        self.connect(self.process, SIGNAL("readyReadStandardError()"), self.readErrors)
        self.connect(self.process, SIGNAL("finished(int,QProcess::ExitStatus)"), self.finished)
        self.connect(self.process, SIGNAL("started()"), self.started)

    def conectar(self, db):
        try:
            self.conn = psycopg2.connect("dbname='" + db + "' user='root'" + "password='password'")
        except:
            print "Error en la conexion con la base de datos " + db
            sys.exit()
        self.cur = self.conn.cursor()

    def desconectar(self):
        self.conn.close()

    def execute(self, query):
        try:
            self.cur.execute(query)
        except:
            print "Fallo en la consulta: " + query
            return None
        return self.cur.fetchall()

    def executeprocess(self, proceso):
        try:
            self.cur.execute(proceso)
        except:
            print "Fallo al ejecutar: " + proceso

    def executeone(self, query):
        try:
            self.cur.execute(query)
        except:
            return None
        return self.cur.fetchone()

    def conectarIsolation(self, db):
        try:
            self.conn = psycopg2.connect("dbname='" + db + "' user='root'" + "password='password'")
            self.conn.set_isolation_level(0)
        except:
            print "Error en la conexion con la base de datos " + db
            sys.exit()

        self.cur = self.conn.cursor()

    def executeGrant(self, permiso):
        try:
            self.cur.execute(permiso)
            self.writecommand(permiso)
        except:
            self.writecommand("<font color =\"#FF0000\">Fallo al intentar cambiar los permisos: " + str(permiso) + "</font>")


    def guardaQuery(self, query):
        self.query = query
        self.fileHandle = open ( '%s/query.sql' % gettempdir(), 'w' )
        self.fileHandle.write ( query )
        self.fileHandle.close()

    def execQuery(self, query):
        if (self.database != ''):
            self.writecommand(QString("----") + self.database + QString("----"))
            self.subcomand = query
            self.guardaQuery(self.subcomand)
            if os.name != 'nt':
               os.chdir('/tmp')
            self.command = functions.psql+' -t -f ' + gettempdir() + '/query.sql' + self.database + functions.end_sql
            self.writecommand(self.command)
            self.process.start(self.command)
            self.process.waitForFinished(-1)
            return QString(self.process.readAllStandardOutput())
        return QString('')

    def execQuerySinConsola(self, query):
        if (self.database != ''):
            self.subcomand = query
            self.guardaQuery(self.subcomand)
            if os.name != 'nt':
               os.chdir('/tmp')
            self.command = functions.psql + ' -t -f ' + gettempdir() + '/query.sql template1' + functions.end_sql
            self.process.start(self.command)
            self.process.waitForFinished(-1)
        return QString('')


    def readOutput(self):
        self.mui_textBrowser.append("<font size=\"-2\">" + QString(self.process.readAllStandardOutput()) + "</font>")

    def readErrors(self):
        if (self.mostrarErrores == "TRUE"):
	    self.mui_textBrowser.append("<font color =\"#FF0000\">error: " + QString(self.process.readAllStandardError()) + "</font>")

    def finished(self, code, st):
        self.mui_textBrowser.append("<font color =\"#00FF00\">Done.</font><BR>")

    def started(self):
        self.mui_textBrowser.append("<BR><font color =\"#00FF00\">Started.</font>")

    def writecommand(self, comm):
        # self.mui_textBrowser.append("<font color =\"#0000FF\">" + QtGui.QApplication.translate("MainWindow", comm, None, QtGui.QApplication.UnicodeUTF8) + "</font>")
        self.mui_textBrowser.append("<font color =\"#0000FF\">" + QString(comm) + "</font>")


    def mensaje(self):
        Yes = 'Si'
        No = 'No'
        message = QtGui.QMessageBox(self)
        message.setText(QtGui.QApplication.translate("Empresa", "Desea agregar el usuario <b>root</b> a PostgreSQL?<br>Este usuario es necesario para poder administrar PostgreSQL.<br>La contrase&ntilde;a de este usuario sera \'password\'.<br><u><b>Borre este usuario al terminar la configuracion de BulmaGes, ya que genera un grave agujero de seguridad.</b></u>", None, QtGui.QApplication.UnicodeUTF8))
        message.setWindowTitle('Atencion!')
        message.setIcon(QtGui.QMessageBox.Warning)
        message.addButton(Yes, QtGui.QMessageBox.AcceptRole)
        message.addButton(No, QtGui.QMessageBox.RejectRole)
        message.exec_()
