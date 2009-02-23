#!/usr/bin/python
# -*- coding: utf-8 -*-

__version__ = "$Revision: 0.1 $"[11:-2]
__date__ = "$Date: 2008/03/21 08:09:13 $"[7:-2]
__author__ = "Tomeu Borras <tborras@conetxia.com>"
__all__ = ["bulmasetup"]
__doc__ = "Sistema de instalacion y mantenimiento de Bases de Datos para BulmaGes.\r\n"

import commands
import sys

sys.path.append('@CMAKE_INSTALL_PREFIX@/share/bulmages/bulmasetup')
#sys.path.append('/home/tborras/bulmages/trunk/bulmages/bulmasetup')


from config import *
from bulmasetupbase import *
from PyQt4 import *
from nuevousuario import NuevoUsuario
from nuevafacturacion import NuevaFacturacion
from nuevacontabilidad import NuevaContabilidad
from listempresas import ListEmpresas
from restbackup   import RestBackup


#Variables para la version de produccion
#plugins.pathdbbulmafact = '@CMAKE_INSTALL_PREFIX@/share/bulmages/dbmodels/crear/bulmafact/'
#plugins.pathdbbulmacont = '@CMAKE_INSTALL_PREFIX@/share/bulmages/dbmodels/crear/bulmacont/'
#plugins.pathdbbulmatpv = '@CMAKE_INSTALL_PREFIX@/share/bulmages/dbmodels/crear/bulmatpv/'
#plugins.pathdbparches = '@CMAKE_INSTALL_PREFIX@/share/bulmages/dbmodels/actualizar/'
#plugins.pathdbplugins = '@CMAKE_INSTALL_PREFIX@/share/bulmages/dbmodels/plugins/'
#plugins.configfiles = '@BULMAGES_CONFIG_DIR@/'
#plugins.confsharebulmages = '@CMAKE_INSTALL_PREFIX@/share/bulmages/'

# Variables para la version de desarrollo
#plugins.pathdbbulmafact = '/usr/local/share/bulmages/dbmodels/crear/bulmafact/'
#plugins.pathdbbulmacont = '/usr/local/share/bulmages/dbmodels/crear/bulmacont/'
#plugins.pathdbbulmatpv = '/usr/local/share/bulmages/dbmodels/crear/bulmatpv/'
#plugins.pathdbparches = '/usr/local/share/bulmages/dbmodels/actualizar/'
#plugins.configfiles = '/etc/'
#plugins.confsharebulmages = '/usr/local/share/bulmages/'


class HelloWindow(QtGui.QMainWindow, Ui_MainWindow, PluginsBulmaSetup):
    def __init__(self, *args):
        apply(QtGui.QMainWindow.__init__, (self,) + args)
	self.setupUi(self)
	
	# Detecta si hemos accedido como 'root' o no y deshabilita las opciones que necesitan esos permisos.
	if commands.getstatusoutput("whoami")[1] == 'root':
	    self.mui_crearusuario.setEnabled(True)
	    self.mui_crearbulmafact.setEnabled(True)
	    self.mui_crearbulmacont.setEnabled(True)
	    self.mui_adminempresas.setEnabled(True)
	    self.mui_restbackup.setEnabled(True)
	else:
	    self.mui_crearusuario.setEnabled(False)
	    self.mui_crearbulmafact.setEnabled(False)
	    self.mui_crearbulmacont.setEnabled(False)
	    self.mui_adminempresas.setEnabled(False)
	    self.mui_restbackup.setEnabled(False)
	
    def on_mui_crearusuario_released(self):
	    win = NuevoUsuario()
	    win.exec_()
	    
    def on_mui_crearbulmafact_released(self):
	    win = NuevaFacturacion()
	    win.exec_()
	    
    def on_mui_crearbulmacont_released(self):
	    win = NuevaContabilidad()
	    win.exec_()
	    
    def on_mui_adminempresas_released(self):
	    win = ListEmpresas()
	    win.exec_()
	    
    def on_mui_restbackup_released(self):
	    win = RestBackup()
	    win.exec_()
	    
def main(args):
    app=QtGui.QApplication(args)
    win=HelloWindow()
    win.show()
    app.connect(app, QtCore.SIGNAL("lastWindowClosed()"),
                app, QtCore.SLOT("quit()"))
    app.exec_()

if __name__=="__main__":
    main(sys.argv)
