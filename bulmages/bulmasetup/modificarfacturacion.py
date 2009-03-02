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

class ModificarFacturacion( Facturacion):
  def __init__(self, database, parent = None):
    Facturacion.__init__(self, database, parent)
    self.mui_nomdb.setText(self.database)

    self.nombre = self.execQuery('SELECT valor FROM configuracion where nombre =\'NombreEmpresa\';').replace('\n', '')
    self.databaserevision = self.execQuery('SELECT valor FROM configuracion where nombre =\'DatabaseRevision\';').replace('\n', '')
    self.mui_nomempresa.setText(self.nombre.replace('\n', ''))
    self.mui_databaserevision.setText(self.databaserevision.replace('\n', ''))
    
    # Comprobamos la existencia de la parte de TPV
    self.tpvexists = self.execQuery('SELECT valor FROM configuracion WHERE nombre = \'DBRev-BulmaTPV\';').replace('\n','')
    if (len(self.tpvexists) > 3):
      self.mui_soporteTPV.setCheckState(Qt.Checked)
    else:
      self.mui_soporteTPV.setCheckState(Qt.Unchecked)


  
  def on_mui_hacerbackup_released(self):
    self.writecommand("Backup")
    self.savefile = QFileDialog.getSaveFileName(self,  QString("Guardar  Elija archivo destino"), QString("/home"), QString("SQL (*.sql *.pgdump)") )
    self.command = 'su postgres -c \"pg_dump -f ' + self.savefile + ' ' + self.database  + '\"'
    self.writecommand(self.command)
    self.process.start(self.command)
    self.process.waitForFinished(-1)
    self.writecommand(self.process.readAllStandardOutput())


  def on_mui_aceptar_released(self):
    if (self.mui_actualizarDB.checkState() == Qt.Checked ):
      self.actualizarDatabase()
    self.mui_textBrowser.clear()
    self.actualizarPlugins()
    self.writeConfig()

def main(args):
  app=QtGui.QApplication(args)
  win=ModificarFacturacion('bulmafact')
  win.exec_()
  sys.exit(app.exec_())

if __name__=="__main__":
  main(sys.argv)
