# -*- coding: utf-8 -*-

import sys
import os
from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from plugins import PluginsBulmaSetup
from facturacion import Facturacion
import plugins
from tempfile import gettempdir
import functions

class NuevaFacturacion(Facturacion):
    def __init__(self, parent = None):
        Facturacion.__init__(self,None, parent)
        self.mui_nomdb.setReadOnly(False)
        self.mui_soporteTPV.setCheckState(Qt.Unchecked)
        # Oculto los campos que solo pertenecen a modificacion
        self.mui_databaserevision.hide()
        self.label_3.hide()
        self.mui_actualizarDB.hide()
        self.mui_hacerbackup.hide()


    def on_mui_aceptar_released(self):

        # Ponemos la pestanya de consola como la visible
        self.tabWidget.setCurrentIndex(3)

        self.mui_textBrowser.clear()

        self.database = self.mui_nomdb.text()

        # Verificamos el nombre de la base de datos
        if (self.database == None or self.database == 'postgres' or self.database == 'template0' or self.database == 'template1' or self.database == ''):
            print "Nombre de la base de datos incorrecto"
            message = QtGui.QMessageBox(self)
            message.setText(QtGui.QApplication.translate("NuevaFacturacion", "Debe proporcionar un nombre a la base de datos que va a crearse", None, QtGui.QApplication.UnicodeUTF8))
            message.setWindowTitle('Atencion!')
            message.setIcon(QtGui.QMessageBox.Warning)
            message.addButton('Cerrar', QtGui.QMessageBox.AcceptRole)
            message.exec_()
            return

        os.chdir(gettempdir())
        # Creamos la base de datos
        self.command = functions.createdb + self.database + functions.end_sql
        self.writecommand(self.command)
        self.process.start(self.command)
        self.process.waitForFinished(-1)

        # Cargamos la esquematica de la base de datos
        self.command = functions.psql + ' ' + ' -f '+ plugins.pathdbbulmafact +'bulmafact_schema.sql ' + self.database + functions.end_sql
        self.writecommand(self.command)
        self.process.start(self.command)
        self.process.waitForFinished(-1)

        # Cargamos los datos minimos
        self.command = functions.psql + ' ' + ' -f ' + plugins.pathdbbulmafact + 'bulmafact_data.sql ' + self.database + functions.end_sql
        self.writecommand(self.command)
        self.process.start(self.command)
        self.process.waitForFinished(-1)

        # Aplicamos el parche de bulmatpv si es necesario
        if (self.mui_soporteTPV.isChecked()):
            self.command = functions.psql + ' ' + ' -f ' + plugins.pathdbbulmatpv + 'bulmatpv_schema.sql ' + self.database + functions.end_sql
            self.writecommand(self.command)
            self.process.start(self.command)
            self.process.waitForFinished(-1)

        # Cambiamos el nombre de la empresa
        self.nomempresa = unicode(self.mui_nomempresa.text()).encode('utf8')
        if os.name == 'posix':
            self.subcomand = 'UPDATE configuracion set valor=\'\"\'' +self.nomempresa +'\'\"\' WHERE nombre = \'\"\'NombreEmpresa\'\"\';'
        else:
            self.subcomand = 'UPDATE configuracion set valor=\'' +self.nomempresa +'\' WHERE nombre = \'NombreEmpresa\';'
        
        self.command = functions.psql2 + ' -c \"' +self.subcomand+ '\"' + ' ' + self.database + functions.end_sql2
        self.writecommand(self.command)
        os.system(self.command.toAscii().data())

        self.actualizarPlugins()

        # Escribimos la configuracion
        self.writeConfig()

        self.mui_textBrowser.append("Done.")




def main(args):
    app=QtGui.QApplication(args)
    win=NuevaFacturacion()
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)
