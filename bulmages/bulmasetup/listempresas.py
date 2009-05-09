# -*- coding: utf-8 -*-
import sys
import os
from config import *
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from listempresasbase import *
from empresa import Empresa
from modificarfacturacion import ModificarFacturacion
from modificarcontabilidad import ModificarContabilidad

class ListEmpresas(Ui_ListEmpresasBase, Empresa):
    def __init__(self, parent = None):
        Empresa.__init__(self,parent)
        self.setupUi(self)
        self.show()
        self.buscarEmpresas()
        self.mui_listado.resizeColumnsToContents()

    def buscarEmpresas(self):
    
        # Ponemos la pestanya de consola como la visible
        self.tabWidget.setCurrentIndex(1)
        
        self.conectar('template1')
        self.databases = self.execute("SELECT datname FROM pg_database ORDER BY datname")
        #Desconectamos la base de datos
        self.desconectar()
        
        self.i = 0
        for row in self.databases:
            self.i = self.i + 1
        self.mui_listado.setRowCount(self.i -4)
        self.i = 0
        
        for row in self.databases:
            if (str(row[0]) != 'template0') and (str(row[0]) != 'template1') and (str(row[0]) != 'mustopalma') and (str(row[0]) != 'talvarez'):
                self.conectar(str(row[0]))
                nombre = self.executeone('SELECT valor FROM configuracion where nombre =\'NombreEmpresa\'')
                tipo = self.executeone('SELECT valor FROM configuracion where nombre =\'Tipo\'')
                databaserevision = self.executeone('SELECT valor FROM configuracion where nombre =\'DatabaseRevision\'')
                self.mui_listado.setItem(self.i, 0 , QTableWidgetItem(str(nombre[0])))
                self.mui_listado.setItem(self.i, 1 , QTableWidgetItem(row[0]))
                self.mui_listado.setItem(self.i, 2 , QTableWidgetItem(str(tipo[0])))
                self.mui_listado.setItem(self.i, 3 , QTableWidgetItem(str(databaserevision[0])))
                self.i = self.i + 1
                if (tipo == ''):
                    self.mui_listado.hideRow(self.i)
                self.desconectar()
                    
        # Ponemos la pestanya principal como la visible
        self.tabWidget.setCurrentIndex(0)


  #def buscarEmpresas(self):
    #self.command = 'su - postgres -c \"echo \'SELECT datname FROM pg_database\' | psql -t template1\"'
    #self.writecommand(self.command)
    #self.process.start(self.command)
    #self.process.waitForFinished(-1)
    #self.databases = QString(self.process.readAllStandardOutput())
    #self.writecommand(self.databases)
    #self.arrdatabase = self.databases.split(QString(" "))
    #self.mui_listado.setRowCount(self.arrdatabase.count() -1)
    #self.i = 1
    #self.progress = QtGui.QProgressBar(self)
  ##   self.progress.setGeometry(self.width /2, self.height/2, 200, 20)
    #self.progress.setGeometry(self.width() / 2 -100, self.height() /2 -10, 200, 40)
    #self.progress.setRange(0, self.arrdatabase.count())
    #self.progress.show()
    #while (self.i < self.arrdatabase.count()):
        #self.writecommand(self.arrdatabase[self.i])
        #self.mui_listado.setItem(self.i-1 , 1 , QTableWidgetItem(self.arrdatabase[self.i].replace('\n', '')))
        #self.nombre = self.execQuery('SELECT valor FROM configuracion where nombre =\'NombreEmpresa\';').replace('\n', '')
        #self.tipo = self.execQuery('SELECT valor FROM configuracion where nombre =\'Tipo\';').replace('\n', '').replace(' ','')
        #self.databaserevision = self.execQuery('SELECT valor FROM configuracion where nombre =\'DatabaseRevision\';').replace('\n', '')
        #self.mui_listado.setItem(self.i-1 , 0 , QTableWidgetItem(self.nombre))
        #self.mui_listado.setItem(self.i-1 , 2 , QTableWidgetItem(self.tipo))
        #self.mui_listado.setItem(self.i-1 , 3 , QTableWidgetItem(self.databaserevision))
        #if (self.tipo == ''):
          #self.mui_listado.hideRow(self.i-1)
        #self.i = self.i + 1
        #self.progress.setValue(self.progress.value() + 1)
    #self.progress.hide()

    def on_mui_listado_cellDoubleClicked(self, row, col):
        if (self.mui_listado.item(row,2).text() == QString('BulmaFact')):
            self.fact = ModificarFacturacion(str(self.mui_listado.item(row,1).text()))
            self.fact.exec_()
        if (self.mui_listado.item(row,2).text() == QString('BulmaCont')):
            self.cont = ModificarContabilidad(str(self.mui_listado.item(row,1).text()))
            self.cont.exec_()

def main(args):
    app=QtGui.QApplication(args)
    win=ListEmpresas()
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)