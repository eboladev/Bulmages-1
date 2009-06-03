# -*- coding: utf-8 -*-

import sys
import os
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from guardarbackupbase import *
from empresa import Empresa
from config import *
# Libreria de acceso a bases de datos PostgreSQL
import psycopg2

class GuardarBackup(Ui_GuardarBackup, Empresa):

    def __init__(self, parent = None):
	Empresa.__init__(self,parent)
        self.setupUi(self)
        self.proceso = QtCore.QProcess()
        self.initListaEmpresas()
	
    def initListaEmpresas(self):

	self.conectar('template1')
        self.databases = self.execute("SELECT datname FROM pg_database ORDER BY datname")
        self.desconectar()

        self.lista_empresas.setRowCount(0)
        self.i = 0
        for row in self.databases:
            if (str(row[0]) != 'template0' and str(row[0]) != 'template1' ):
                self.lista_empresas.hideRow(self.i)
                self.conectar(row[0])
                existconf = self.executeone('SELECT * FROM pg_tables where tablename =\'configuracion\'')
                if (existconf != None):
                    tipo = self.executeone('SELECT valor FROM configuracion where nombre =\'Tipo\'')
                    nombre = self.executeone('SELECT valor FROM configuracion where nombre =\'NombreEmpresa\'')
                    self.lista_empresas.setRowCount(self.lista_empresas.rowCount() + 1)
                    self.lista_empresas.setItem(self.i, 0 , QTableWidgetItem(str(nombre[0])))
                    self.lista_empresas.setItem(self.i, 1 , QTableWidgetItem(row[0]))
                    self.lista_empresas.setItem(self.i, 2 , QTableWidgetItem(str(tipo[0])))
                    self.lista_empresas.showRow(self.i)
                    self.i = self.i + 1
                self.desconectar()
	self.lista_empresas.resizeColumnsToContents()
		
    def on_guardar_backup_released(self):

	# Pasamos el nombre de la base de datos seleccionada en listWidget a la variable database
        i = 0
        while (i < self.lista_empresas.rowCount()):
            if self.lista_empresas.item(i,1).isSelected():
	      self.database = self.lista_empresas.item(i,1).text()
	      break
	    i = i + 1 

        self.savefile = QFileDialog.getSaveFileName(self,  QString("Guardar - Elija archivo destino"), QString("/home"), QString("SQL (*.sql *.pgdump)"))
	self.command = 'pg_dump -f ' + self.savefile + ' ' + self.database
        self.proceso.start(self.command)
        self.proceso.waitForFinished(-1)

def main(args):
    app=QtGui.QApplication(args)
    win=GuardarBackup()
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)
                