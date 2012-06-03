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
from tempfile import gettempdir
import functions


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


        # Ponemos la pestanya de consola como la visible
        self.tabWidget.setCurrentIndex(2)

        self.mui_textBrowser.clear()

        self.nomdb = self.mui_nomdb.text()
        self.database = self.nomdb

        # Verificamos el nombre de la base de datos
        if (self.nomdb == None or self.nomdb == 'postgres' or self.nomdb == 'template0' or self.nomdb == 'template1' or self.nomdb == ''):
            print "Nombre de la base de datos incorrecto"
            message = QtGui.QMessageBox(self)
            message.setText(QtGui.QApplication.translate("NuevaFacturacion", "Debe proporcionar un nombre a la base de datos que va a crearse", None, QtGui.QApplication.UnicodeUTF8))
            message.setWindowTitle('Atencion!')
            message.setIcon(QtGui.QMessageBox.Warning)
            message.addButton('Cerrar', QtGui.QMessageBox.AcceptRole)
            message.exec_()
            return

        # Creamos la base de datos
        self.command = functions.createdb + self.nomdb + functions.end_sql
        self.writecommand(self.command)
        self.process.start(self.command)
        self.process.waitForFinished(-1)

        # Cargamos la esquematica de la base de datos
        self.command = functions.psql + ' ' + ' -f '+ functions.multios().correctFileName(plugins.pathdbbulmacont +'bulmacont_schema.sql') + ' ' + self.nomdb + functions.end_sql
        self.writecommand(self.command)
        self.process.start(self.command)
        self.process.waitForFinished(-1)

        # Cargamos los datos minimos
        self.command = functions.psql + ' ' + ' -f '+ functions.multios().correctFileName(plugins.pathdbbulmacont +'t_configuracion_data.sql') + ' ' + self.nomdb + functions.end_sql
        self.writecommand(self.command)
        self.process.start(self.command)
        self.process.waitForFinished(-1)

        # Cambiamos el nombre de la empresa
        self.nomempresa = self.mui_nomempresa.text()
        if os.name == 'posix':
            self.subcomand = 'UPDATE configuracion set valor=\'\"\'' +self.nomempresa +'\'\"\' WHERE nombre = \'\"\'NombreEmpresa\'\"\';'
        else:
            self.subcomand = 'UPDATE configuracion set valor=\'' +self.nomempresa +'\' WHERE nombre = \'NombreEmpresa\';'
        self.command = functions.psql2 + ' -c \"' +self.subcomand+ '\"' + ' ' + self.nomdb + functions.end_sql2
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
