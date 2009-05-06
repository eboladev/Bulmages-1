# -*- coding: utf-8 -*-

import sys
import os
from config import *
from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from plugins import PluginsBulmaSetup
from contabilidad import Contabilidad
from modificarusuario import ModificarUsuario
import plugins

class ModificarContabilidad(Contabilidad):
    def __init__(self, database, parent = None):
        Contabilidad.__init__(self,database, parent)
        # Establecemos cual va a ser la base de datos con la que trabajaremos todo el rato
        self.nombre = self.execQuery('SELECT valor FROM configuracion where nombre =\'NombreEmpresa\';').replace('\n', '')
        self.databaserevision = self.execQuery('SELECT valor FROM configuracion where nombre =\'DatabaseRevision\';').replace('\n', '')
        self.mui_nomempresa.setText(self.nombre.replace('\n', ''))
        self.mui_databaserevision.setText(self.databaserevision.replace('\n', ''))
        self.mui_nomdb.setText(self.database)


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
    
    def on_mui_permisos_released(self):
        win = ModificarUsuario()
        win.exec_()


def main(args):
    app=QtGui.QApplication(args)
    win=ModificarContabilidad('bulmacont')
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)
