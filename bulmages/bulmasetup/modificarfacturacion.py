# -*- coding: utf-8 -*-

import sys
import os
from config import *
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from modificarfacturacionbase import *
from plugins import PluginsBulmaSetup
import plugins
from facturacion import Facturacion

class ModificarFacturacion( Ui_ModificarFacturacionBase, Facturacion):
  def __init__(self, database, parent = None):
    Facturacion.__init__(self)
    self.setupUi(self)
    
    self.process = QtCore.QProcess()
#   self.connect(self.process, SIGNAL("readyReadStandardOutput()"), self.readOutput)
    self.connect(self.process, SIGNAL("readyReadStandardError()"), self.readErrors)
    self.connect(self.process, SIGNAL("finished()"), self.finished)
    self.connect(self.process, SIGNAL("started()"), self.started)
    self.mui_nomdb.setText(database)
    # Establecemos cual va a ser la base de datos con la que trabajaremos todo el rato
    self.database = database
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


    # Ocultamos la columna de las descripciones.
    self.mui_plugins.hideColumn(1)
    self.mui_plugins1.hideColumn(1)

    # Buscamos los Plugins
    self.buscaPlugins()
    # Ajustamos la presentacion
    self.mui_plugins.resizeColumnsToContents()
    self.mui_plugins1.resizeColumnsToContents()
    self.mui_plugins.resizeRowsToContents()
    self.mui_plugins1.resizeRowsToContents()
  
  def on_mui_actualizardatabase_released(self):
    self.actualizarDatabase()

  def on_mui_hacerbackup_released(self):
    self.writecommand("Backup")

    self.savefile = QFileDialog.getSaveFileName(self,  QString("Guardar  Elija archivo destino"), QString("/home"), QString("SQL (*.sql *.pgdump)") )
    self.command = 'su postgres -c \"pg_dump -f ' + self.savefile + ' ' + self.database  + '\"'
    self.writecommand(self.command)
    self.process.start(self.command)
    self.process.waitForFinished(-1)
    self.writecommand(self.process.readAllStandardOutput())


  def on_mui_aceptar_released(self):
    self.mui_textBrowser.clear()
    self.actualizarPlugins()
    self.writeConfig()

  def on_mui_vertodos_stateChanged(self, status):
    if (status == 2):
      # Establezco la tabla de bulmafact
      self.i = 0
      while (self.i < self.mui_plugins.rowCount()):
        if (self.mui_plugins.item(self.i, 0).checkState() != Qt.Checked ):
          self.mui_plugins.hideRow(self.i)
        self.i = self.i + 1
      
      # Establezco la tabla de bulmatpv
      self.i = 0
      while (self.i < self.mui_plugins1.rowCount()):
        if (self.mui_plugins1.item(self.i, 0).checkState() != Qt.Checked ):
          self.mui_plugins1.hideRow(self.i)
        self.i = self.i + 1
    else:
      self.i = 0
      while (self.i < self.mui_plugins1.rowCount()):
        self.mui_plugins1.showRow(self.i)
        self.i = self.i + 1

      self.i = 0
      while (self.i < self.mui_plugins.rowCount()):
        self.mui_plugins.showRow(self.i)
        self.i = self.i + 1



def main(args):
  app=QtGui.QApplication(args)
  win=ModificarFacturacion('bulmafact')
  win.exec_()
  sys.exit(app.exec_())

if __name__=="__main__":
  main(sys.argv)
