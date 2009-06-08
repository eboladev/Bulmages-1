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
	
        self.mui_textBrowser.clear()
	
        self.writecommand("===== Backup =====<br>")
	self.command = 'pg_dump -f ' + '/etc/bulmages/' + self.database + '.sql ' + self.database
        self.writecommand(self.command)
        self.process.start(self.command)
        self.process.waitForFinished(-1)
        self.writecommand(self.process.readAllStandardOutput())
	
	self.directorio = QFileDialog.getExistingDirectory(self, "Selecciona la carpeta de destino del Backup","/home")
	
	self.conffact = 'bulmafact_' + self.database + '.conf'
	self.conftpv = 'bulmatpv_' + self.database + '.conf'
	self.quehehecho = 0
	
	if os.path.exists('/etc/bulmages/' + self.conffact):
	  self.writecommand('Buscando archivo de configuracion: /etc/bulmages/' + self.conffact + '<br>')
	  self.quehehecho = 1
	else:
	  self.writecommand('<font color =\"#FF0000\">No existe el archivo de configuracion: /etc/bulmages/' + self.conffact + '</font>')
	if os.path.exists('/etc/bulmages/' + self.conftpv):
	  self.writecommand('Buscando archivo de configuracion: /etc/bulmages/' + self.conftpv + '<br>')
	  self.quehehecho = 2
	
	if self.quehehecho == 1:
	  self.writecommand('Empaquetando archivos de configuracion y datos de la BD<br>')
	  self.command = 'tar czf ' + self.directorio + '/' + self.database + '.tar.gz ' + '/etc/bulmages/' + self.conffact + ' ' + '/etc/bulmages/' + self.database + '.sql'
	  self.writecommand(self.command)
	  self.process.start(self.command)
	  self.process.waitForFinished(-1)
	if self.quehehecho == 2:
	  self.writecommand('Empaquetando archivos de configuracion y datos de la BD<br>')
	  self.command = 'tar czf ' + self.directorio + '/' + self.database + '.tar.gz ' + '/etc/bulmages/' + self.conffact + ' ' + '/etc/bulmages/' + self.conftpv + ' ' + '/etc/bulmages/' + self.database + '.sql'
	  self.writecommand(self.command)
	  self.process.start(self.command)
	  self.process.waitForFinished(-1)
	  
	self.writecommand("===== Backup Guardado Satisfactoriamente En : " + self.directorio + " =====<br><br>")
	
	self.command = 'rm /etc/bulmages/' + self.database + '.sql'
	self.writecommand('Eliminando archivos temporales')
	self.process.start(self.command)
	self.process.waitForFinished(-1)
	
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
