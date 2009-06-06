# -*- coding: utf-8 -*-

import sys
import os
from config import *
from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from plugins import PluginsBulmaSetup
from contabilidad import Contabilidad
import plugins


class NuevaContabilidad(Contabilidad):
  def __init__(self, parent = None):
    Contabilidad.__init__(self,'', parent)
    # Oculto los campos que solo pertenecen a modificacion
    self.mui_databaserevision.hide()
    self.label_3.hide()
    self.mui_actualizarDB.hide()
    self.mui_hacerbackup.hide()
    self.mui_nomdb.setReadOnly(False)
    
  def on_mui_aceptar_released(self):
  
    # Verificamos el nombre de la base de datos
    if (self.database == None or self.database == 'postgres' or self.database == 'template0' or self.database == 'template1' or self.database == ''):
	print "Nombre de la base de datos incorrecto"
	return

#OR self.database == 'postgres' OR self.database == 'template0' OR self.database == 'template1' OR self.database == ''

    # Ponemos la pestanya de consola como la visible
    self.tabWidget.setCurrentIndex(1)

    self.mui_textBrowser.clear()

    self.nomdb = self.mui_nomdb.text()
    self.database = self.nomdb

    # Creamos la base de datos
    self.command = 'su postgres -c "createdb -E UNICODE ' + self.nomdb +'"'
    self.writecommand(self.command)
    self.process.start(self.command)
    self.process.waitForFinished(-1)
    
    # Cargamos la esquematica de la base de datos
    self.command = 'su postgres -c "psql ' + self.nomdb + ' < '+ plugins.pathdbbulmacont +'bulmacont_schema.sql"'
    self.writecommand(self.command)
    self.process.start(self.command)
    self.process.waitForFinished(-1)

    # Cargamos los datos minimos
    self.command = 'su postgres -c "psql ' + self.nomdb + ' < ' + plugins.pathdbbulmacont + 't_configuracion_data.sql"'
    self.writecommand(self.command)
    self.process.start(self.command)
    self.process.waitForFinished(-1)

    # Cambiamos el nombre de la empresa
    self.nomempresa = self.mui_nomempresa.text()
    self.subcomand = 'UPDATE configuracion set valor=\'\"\'' +self.nomempresa +'\'\"\' WHERE nombre = \'\"\'NombreEmpresa\'\"\';'
    self.command = 'su postgres -c \'psql ' + self.nomdb + ' -c \"' +self.subcomand+ '\"\''
    self.writecommand(self.command)
    os.system(self.command.toAscii().data())


    self.actualizarPlugins()
    self.writeConfig()
        
    self.mui_textBrowser.append("Done.")


def main(args):
    app=QtGui.QApplication(args)
    win=NuevaContabilidad()
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)
