# -*- coding: utf-8 -*-

import sys
import os
from config import *
from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from plugins import PluginsBulmaSetup
import plugins
from facturacion import Facturacion
from modificarusuario import ModificarUsuario

class ModificarFacturacion( Facturacion):
    def __init__(self, database, parent = None):
        Facturacion.__init__(self, database, parent)
        # Establecemos cual va a ser la base de datos con la que trabajaremos y conectamos con ella
        self.conectar(self.database)
        # Sacamos el nombre de la empresa y el numero de revision de la base de datos
        self.nombre = self.executeone('SELECT valor FROM configuracion where nombre =\'NombreEmpresa\'')
        self.databaserevision = self.executeone('SELECT valor FROM configuracion where nombre =\'DatabaseRevision\'')
        # Comprobamos la existencia de la parte de TPV
        self.tpvexists = self.executeone('SELECT valor FROM configuracion WHERE nombre = \'DBRev-BulmaTPV\'')
        # Desconectamos
        self.desconectar()
        # Rellenamos las campos con los datos obtenido en las consultas anteriores
        self.mui_nomdb.setText(self.database)
        self.mui_nomempresa.setText(self.nombre[0])
        self.mui_databaserevision.setText(self.databaserevision[0])
        if (self.tpvexists != None):
            if (len(self.tpvexists[0]) > 3):
                self.mui_soporteTPV.setCheckState(Qt.Checked)
            else:
                self.mui_soporteTPV.setCheckState(Qt.Unchecked)

    def on_mui_hacerbackup_released(self):
        # Ponemos la pestanya de consola como la visible
        self.tabWidget.setCurrentIndex(2)
        
        self.writecommand("Backup")
        self.savefile = QFileDialog.getSaveFileName(self,  QString("Guardar  Elija archivo destino"), QString("/home"), QString("SQL (*.sql *.pgdump)") )
        self.command = 'su postgres -c \"pg_dump -f ' + self.savefile + ' ' + self.database  + '\"'
        self.writecommand(self.command)
        self.process.start(self.command)
        self.process.waitForFinished(-1)
        self.writecommand(self.process.readAllStandardOutput())

    def on_mui_aceptar_released(self):
        # Ponemos la pestanya de consola como la visible
        self.tabWidget.setCurrentIndex(2)
    
        if (self.mui_actualizarDB.checkState() == Qt.Checked ):
            self.actualizarDatabase()
        self.mui_textBrowser.clear()
        self.actualizarPlugins()
        self.writeConfig()

    def on_mui_permisos_released(self):
            win = ModificarUsuario()
            win.exec_()

def main(args):
    app=QtGui.QApplication(args)
    win=ModificarFacturacion('bulmafact')
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)
