# -*- coding: utf-8 -*-

import sys
import os
from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from plugins import PluginsBulmaSetup
import plugins
# Libreria de acceso a bases de datos PostgreSQL
import psycopg2

class Empresa(QtGui.QDialog, PluginsBulmaSetup):
    def __init__(self, database, parent = None):
        PluginsBulmaSetup.__init__(self)
        QtGui.QDialog.__init__(self,parent)
        self.database = database
        
        # Establecemos cual va a ser la base de datos con la que trabajaremos todo el rato
        self.process = QtCore.QProcess()
        
        """Hacemos una conexion de prueba para comprobar si esta creado el usuario ROOT en PostgreSQL
        Ya que es necesario para poder administrar los permisos de las Bases de Datos.
        Si no existe, saltara un QMessageBox preguntando si se quiere crear el usuario ROOT."""
        
        try:
            conn = psycopg2.connect("dbname='template1' user='root'")
            conn.close()
        except:
            reply = QtGui.QMessageBox.question(self, 'Atencion!', "Desea agregar el usuario Root a PostgreSQL?\n\nEste usuario es necesario para poder administrar los permisos.", QtGui.QMessageBox.Yes, QtGui.QMessageBox.No)
            if reply == QtGui.QMessageBox.Yes:
                comandoroot = "su postgres -c \"createuser -s root\""
                self.process.start(comandoroot)
                self.process.waitForFinished(-1)
            else:
                print "Para poder modificar los permisos de las bases de datos es necesario agregar el usuario ROOT a PostgreSQL"
                print "Cerrando la aplicacion"
                sys.exit()
                
        try:
            self.conntemplate1 = psycopg2.connect("dbname='template1' user='root'")
        except:
            print "Error"
            sys.exit()
            
        self.curtemplate1 = self.conntemplate1.cursor()
        
        #self.connect(self.process, SIGNAL("readyReadStandardOutput()"), self.readOutput)
        self.connect(self.process, SIGNAL("readyReadStandardError()"), self.readErrors)
        self.connect(self.process, SIGNAL("finished()"), self.finished)
        self.connect(self.process, SIGNAL("started()"), self.started)

    def conectar(self, db):
        try:
            self.conn = psycopg2.connect("dbname='" + db + "' user='root'")
        except:
            print "Error en la conexion con la base de datos " + db
            sys.exit()
        self.cur = self.conn.cursor()

    def querytemplate1(self, query):
        try:
            self.curtemplate1.execute(query)
        except:
            print "Fallo en la consulta: " + query
            sys.exit()
        return self.curtemplate1.fetchall()

    def execute(self, query):
        try:
            self.cur.execute(query)
        except:
            print "Fallo en la consulta: " + query
            sys.exit()
        return self.cur.fetchall()
        
    def executeone(self, query):
        try:
            self.cur.execute(query)
        except:
            print "Fallo en la consulta: " + query
            sys.exit()
        return self.cur.fetchone()
        

    def guardaQuery(self, query):
        self.query = query
        self.fileHandle = open ( '/tmp/query.sql', 'w' )
        self.fileHandle.write ( query )
        self.fileHandle.close()

    def execQuery(self, query):
        if (self.database != ''):
            self.writecommand(QString("----") + self.database + QString("----"))
            self.subcomand = query
            self.guardaQuery(self.subcomand)
            self.command = 'su postgres -c \"psql -t -f /tmp/query.sql ' + self.database + '\"'
            self.writecommand(self.command)
            self.process.start(self.command)
            self.process.waitForFinished(-1)
            return QString(self.process.readAllStandardOutput())
        return QString('')
   
    def readOutput(self):
        self.mui_textBrowser.append(QString(self.process.readAllStandardOutput()))

    def readErrors(self):
        self.mui_textBrowser.append("<font color =\"#FF0000\">error: " + QString(self.process.readAllStandardError()) + "</font>")

    def finished(self):
        self.mui_textBrowser.append("<font color =\"#00FF00\">Done.</font>")

    def started(self):
        self.mui_textBrowser.append("<font color =\"#00FF00\">Started.</font>")

    def writecommand(self, comm):
        self.mui_textBrowser.append("<font color =\"#0000FF\">"+comm+"</font>")
