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

    def buscarEmpresas(self):
    
        self.conectar('template1')
        self.databases = self.execute("SELECT datname FROM pg_database ORDER BY datname")
        self.desconectar()
        
        for row in self.databases:
            if (str(row[0]) != 'template0' and str(row[0]) != 'template1' ):
                self.conectar(row[0])
                existconf = self.executeone('SELECT * FROM pg_tables WHERE tablename =\'configuracion\'')
                if (existconf != None):
                    tipo = self.executeone('SELECT valor FROM configuracion where nombre =\'Tipo\'')
                    nombre = self.executeone('SELECT valor FROM configuracion where nombre =\'NombreEmpresa\'')
                    if (str(tipo[0]) == 'BulmaCont'):
                        self.listWidgetCont.addItem(QString(nombre[0]))
                    if (str(tipo[0]) == 'BulmaFact'):
                        self.listWidgetFact.addItem(QString(nombre[0]))
                self.desconectar()

def main(args):
    app=QtGui.QApplication(args)
    win=EliminarEmpresa()
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)