# -*- coding: utf-8 -*-
import sys
import os
from config import *
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from eliminarempresabase import *
from empresa import Empresa
from config import *
# Libreria de acceso a bases de datos PostgreSQL
import psycopg2

class EliminarEmpresa(Ui_EliminarEmpresa, Empresa):
    def __init__(self, parent = None):
        Empresa.__init__(self,parent)
        self.setupUi(self)
        self.show()
        self.buscarEmpresas()
        self.lista_empresas.resizeColumnsToContents()

    def buscarEmpresas(self):
    
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
                
    def on_lista_empresas_cellDoubleClicked(self, row, col):
    
        Yes = 'Si'
        No = 'No'
        respuesta = False        
        message = QtGui.QMessageBox(self)
        message.setText('Seguro que deseas eliminar esta empresa ?')
        message.setWindowTitle('Confirmacion')
        message.setIcon(QtGui.QMessageBox.Question)
        message.addButton(Yes, QtGui.QMessageBox.AcceptRole)
        message.addButton(No, QtGui.QMessageBox.RejectRole)
        message.exec_()        
        respuesta = message.clickedButton().text()
        
        if respuesta == Yes:
            self.proceso = QtCore.QProcess()
            command = 'su postgres -c "dropdb ' + str(self.lista_empresas.item(row,1).text()) + '"'
            print command
            self.proceso.start(command)
            self.proceso.waitForFinished(-1)
                    
            if (self.lista_empresas.item(row,2).text() == QString('BulmaFact')):
                if os.path.exists('/etc/bulmages/bulmafact_' + str(self.lista_empresas.item(row,1).text()) + '.conf'):
                    os.remove('/etc/bulmages/bulmafact_' + str(self.lista_empresas.item(row,1).text()) + '.conf')
                if os.path.exists('/etc/bulmages/bulmatpv_' + str(self.lista_empresas.item(row,1).text()) + '.conf'):
                    os.remove('/etc/bulmages/bulmatpv_' + str(self.lista_empresas.item(row,1).text()) + '.conf')
            if (self.lista_empresas.item(row,2).text() == QString('BulmaCont')):
                if os.path.exists('/etc/bulmages/bulmacont_' + str(self.lista_empresas.item(row,1).text()) + '.conf'):
                    os.remove('/etc/bulmages/bulmacont_' + str(self.lista_empresas.item(row,1).text()) + '.conf')
                    
            self.lista_empresas.clear()
            self.buscarEmpresas()
                    
def main(args):
    app=QtGui.QApplication(args)
    win=EliminarEmpresa()
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)