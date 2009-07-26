# -*- coding: utf-8 -*-

import sys
import os
from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from plugins import PluginsBulmaSetup
from modificarcontabilidadbase import *
from empresa import Empresa
import plugins

class Contabilidad(Ui_ModificarContabilidadBase, Empresa):
   def __init__(self, database, parent = None):
      Empresa.__init__(self, database)
      self.setupUi(self)

      # Ocultamos la columna de las descripciones.
      self.mui_plugins.setColumnCount(12)
      self.mui_plugins.hideColumn(1)
      self.mui_plugins.hideColumn(3)
      self.mui_plugins.hideColumn(4)
      self.mui_plugins.hideColumn(5)
      self.mui_plugins.hideColumn(6)
      self.mui_plugins.hideColumn(7)
      self.mui_plugins.hideColumn(8)
      self.mui_plugins.hideColumn(9)
      self.mui_plugins.hideColumn(10)

      # Desabilitamos el sorting para que se rellenen bien las tablas.
      self.mui_plugins.setSortingEnabled(False)

      # Buscamos los Plugins
      self.buscaPlugins()
      # Ajustamos la presentacion
      self.mui_plugins.resizeColumnsToContents()
      
      # Mostramos la ventana para que el padre tome el control
      self.show()
      # iniciialzamos
      self.inicializar()
      self.mui_plugins.setSortingEnabled(True)
      
   def inicializar(self):
      # Inicializamos el combo de categoria.
      self.mui_categoria.clear()
      self.i =0 ;
      self.cadcategorias = '';
      while (self.i < len(self.pluginsbulmacont)):
         self.cadcategorias = self.cadcategorias + self.pluginsbulmacont[self.i][8]
         self.i = self.i + 1
      # Tratamos toda la cadena de categorias.
      self.cadcategorias = self.cadcategorias.replace('; ',';')
      self.cadcategorias = self.cadcategorias.replace(' ;',';')
      self.arra = self.cadcategorias.split(';')
      self.arra.sort()
      self.j = 0
      self.mui_categoria.addItem('--Todas las Categorias--')
      while (self.j < len ( self.arra)):
         if (self.j < 1 or self.arra[self.j] <> self.arra[self.j-1]):
            self.mui_categoria.addItem( self.arra[self.j])
         self.j = self.j + 1

   def actualizarDatabase(self):
      self.revisiones = ["rev-0.5.3.sql", "rev-0.9.1.sql", "rev-0.9.3.sql", "rev-0.10.sql", "rev-0.11.sql", "rev-0.12.sql"]
      #Parcheamos todo lo que hay que parchear
      for self.parche in self.revisiones:
         self.command = 'su postgres -c \"psql -t -f ' + plugins.pathdbparches + self.parche + ' ' + self.database  + '\"'
         self.writecommand(self.command)
         self.process.start(self.command)
         self.process.waitForFinished(-1)
         self.writecommand(self.process.readAllStandardOutput())
         self.actualizarPlugins()
   

   def buscaPlugins1(self):
      self.plugins = self.execQuery('SELECT nombre, valor FROM configuracion WHERE nombre LIKE \'DBRev-%\'')
      self.writecommand(self.plugins)
      self.arrplugins = self.plugins.split(QString("\n"))
      self.mui_plugins.setRowCount(self.arrplugins.count() -3)
      self.i = 0
      while (self.i < self.arrplugins.count() ):
         self.writecommand(self.arrplugins[self.i])
         self.valores = self.arrplugins[self.i].split(QString("|"))
         if (self.valores.count() >= 2):
            self.mui_plugins.setItem(self.i-1 , 1 , QTableWidgetItem(self.valores[1].replace('\n', '')))
            self.mui_plugins.setItem(self.i-1 , 0 , QTableWidgetItem(self.valores[0].replace('\n', '')))
   
         self.i = self.i + 1
   
   def buscaPluginInstalado(self, plugin, libreria):
      self.version = self.execQuery('SELECT valor FROM configuracion WHERE nombre = \'' + plugin +'\'').replace('\n','').replace(' ','')
      if (len(self.version) > 2):
         return self.version
   
      if (libreria == ''):
         return ''

      self.process1 = QtCore.QProcess()
      self.mfile = QFile(plugins.configfiles + 'bulmacont_' + self.database + '.conf')
      if (self.mfile.exists()):
         self.command = 'grep '+libreria+' '+ plugins.configfiles + 'bulmacont_' + self.database + '.conf'
         self.writecommand(self.command)
         self.process1.start(self.command)
         self.process1.waitForFinished(-1)
         self.version = self.process1.readAllStandardOutput()

      if (self.version != ''):
         self.version = '0.12'
      return QString(self.version)
   
   
   def buscaPlugins(self):
      self.writecommand("Buscando Plugins")
      
      #Creamos la bara de progreso
      self.progress = QtGui.QProgressBar()
      self.progress.setGeometry(self.width() / 2 -100, self.height() /2 -10, 200, 40)
      self.progress.setRange(0, len(self.pluginsbulmacont))
      self.progress.show()
      
      self.semaforo = 0
   
      self.mui_plugins.setRowCount(len(self.pluginsbulmacont))
      self.i = 0
      while (self.i < len(self.pluginsbulmacont)):
         self.versioninst = self.buscaPluginInstalado(self.pluginsbulmacont[self.i][3], self.pluginsbulmacont[self.i][1])
         self.check = QTableWidgetItem(QtGui.QApplication.translate("MainWindow", self.pluginsbulmacont[self.i][0], None, QtGui.QApplication.UnicodeUTF8))
         self.check.setFlags(Qt.ItemIsUserCheckable | Qt.ItemIsEnabled)
         self.check.setCheckState(Qt.Unchecked)
         if (self.versioninst != ''):
            self.check.setCheckState(Qt.Checked)
         self.mui_plugins.setItem(self.i, 0, self.check)
         self.mui_plugins.setItem(self.i, 2, QTableWidgetItem(self.versioninst))
         self.mui_plugins.setItem(self.i , 1 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmacont[self.i][2], None, QtGui.QApplication.UnicodeUTF8)))
         self.mui_plugins.setItem(self.i , 3 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmacont[self.i][3], None, QtGui.QApplication.UnicodeUTF8)))
         self.mui_plugins.setItem(self.i , 4 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmacont[self.i][4], None, QtGui.QApplication.UnicodeUTF8)))
         self.mui_plugins.setItem(self.i , 5 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmacont[self.i][5], None, QtGui.QApplication.UnicodeUTF8)))
         self.mui_plugins.setItem(self.i , 6 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmacont[self.i][6], None, QtGui.QApplication.UnicodeUTF8)))
         self.mui_plugins.setItem(self.i , 7 , QTableWidgetItem(QtCore.QString.number(self.pluginsbulmacont[self.i][7])))
         self.mui_plugins.setItem(self.i , 8 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmacont[self.i][8], None, QtGui.QApplication.UnicodeUTF8)))
         self.mui_plugins.setItem(self.i , 9 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmacont[self.i][9], None, QtGui.QApplication.UnicodeUTF8)))
         self.mui_plugins.setItem(self.i , 10 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmacont[self.i][1], None, QtGui.QApplication.UnicodeUTF8)))
         self.mui_plugins.setItem(self.i , 11 , QTableWidgetItem(QtGui.QApplication.translate("MainWindow",self.pluginsbulmacont[self.i][10], None, QtGui.QApplication.UnicodeUTF8)))


         self.i = self.i + 1
         self.progress.setValue(self.progress.value() + 1)
      self.progress.hide()
        


   def actualizarPlugins(self):
      self.writecommand('ACTUALIZANDO PLUGINS')
      
      #Creamos la bara de progreso
      self.progress = QtGui.QProgressBar(self)
      self.progress.setGeometry(self.width() / 2 -100, self.height() /2 -10, 200, 40)
      self.progress.setRange(0, self.mui_plugins.rowCount())
      self.progress.show()
      
      
      self.i = 0
      while (self.i < self.mui_plugins.rowCount()):
         self.writecommand('Tratando ' + self.mui_plugins.item(self.i, 0).text())
         if (self.mui_plugins.item(self.i, 0).checkState() == Qt.Checked):
            self.writecommand('Ha que actualizar ' + self.mui_plugins.item(self.i,0).text())
	    # En realidad deberia comprobar que el archivo existe.
            if (len(self.mui_plugins.item(self.i, 4).text()) > 4):
               self.command = 'su postgres -c \"psql -t -f ' + plugins.pathdbplugins + self.mui_plugins.item(self.i,4).text() + ' ' + self.database + '\"'
               self.writecommand(self.command)
               self.process.start(self.command)
               self.process.waitForFinished(-1)
               self.writecommand(self.process.readAllStandardOutput())
         else:
            # Si no esta chequeado hacemos un borrado del plugin
            if (len(self.mui_plugins.item(self.i,9).text()) > 4 ):
              # Aplicamos el parche  de borrado.
              self.command = 'su postgres -c \"psql -t -f  ' + plugins.pathdbplugins + self.mui_plugins.item(self.i,9).text() +' '+ self.database +'\"'
              self.writecommand(self.command)
              self.process.start(self.command)
              self.process.waitForFinished(-1)
              self.writecommand(self.process.readAllStandardOutput())
         self.i = self.i + 1
         self.progress.setValue(self.progress.value() + 1)
      self.progress.hide()

   def marcar(self, plug):
      self.i = 0
      while (self.i < self.mui_plugins.rowCount()):
         if (self.mui_plugins.item(self.i,10).text() == plug):
            self.mui_plugins.item(self.i,0).setCheckState(Qt.Checked)
         self.i = self.i + 1

   def desmarcar(self, plug):
      self.i = 0
      while (self.i < self.mui_plugins.rowCount()):
         if (self.mui_plugins.item(self.i,10).text() == plug):
            self.mui_plugins.item(self.i,0).setCheckState(Qt.Unchecked)
         self.i = self.i + 1

   def on_mui_plugins_cellClicked(self, row, col):
      # Escribimos la descripcion 
      self.mui_descripcion.setText(self.mui_plugins.item(row,1).text() + "<b>" + self.mui_plugins.item(row,10).text() + "</b><br>"+ self.mui_plugins.item(row,3).text() + "<br><b>Categorias:</b> " + self.mui_plugins.item(row,8).text()+ "<br><br><b>Dependencias:</b> " + self.mui_plugins.item(row,5).text() + "<br><br><b>Incompatibilidades:</b> " + self.mui_plugins.item(row,6).text() + "<br><br><b>Parches SQL:</b><br>" + self.mui_plugins.item(row,4).text() + "<br>" + self.mui_plugins.item(row,9).text())
     
      if (self.semaforo == 1):
         # Marcamos las dependencias
         self.i = 0
         while (self.i < self.mui_plugins.rowCount()):
            if (self.mui_plugins.item(self.i, 0).checkState() == Qt.Checked):
               self.arr = self.mui_plugins.item(self.i,5).text().split(QString(","))
               for self.dep in self.arr:
                  self.marcar(self.dep)
            self.i = self.i +1
         # Desmarcamos las incompatibilidades
         self.arr = self.mui_plugins.item(row,6).text().split(QString(","))
         for self.dep in self.arr:
            self.desmarcar(self.dep)
         self.i = 0
         while (self.i < self.mui_plugins.rowCount()):
            if (self.mui_plugins.item(self.i, 0).checkState() == Qt.Checked):
               self.arr = self.mui_plugins.item(self.i,6).text().split(QString(","))
               for self.dep in self.arr:
                  self.desmarcar(self.dep)
            self.i = self.i +1


   def trataOpenReports(self):
      self.writecommand('Generando plantillas RML y PYS')
      # Creamos el directorio especifico para guardar las plantillas
      self.string = "mkdir -p /opt/bulmages/openreports_" + self.database 
      self.process.start(self.string)
      self.process.waitForFinished(-1)

      # Hacemos un backup de openreports
      self.string = "cp -R /opt/bulmages/openreports_" + self.database + " /opt/bulmages/openreports_" + self.database + "_old"
      self.process.start(self.string)
      self.process.waitForFinished(-1)

      # Copiamos los archivos genericos
      # Copiamos las plantillas
      self.string = "cp /usr/local/share/bulmages/openreports/plantilla.rml" + self.arra[self.j] + " /opt/bulmages/openreports_" + self.database
      self.writecommand(self.string)
      self.process.start(self.string)
      self.process.waitForFinished(-1)
      self.string = "cp /usr/local/share/bulmages/openreports/estilos.rml" + self.arra[self.j] + " /opt/bulmages/openreports_" + self.database
      self.writecommand(self.string)
      self.process.start(self.string)
      self.process.waitForFinished(-1)
      self.string = "cp /usr/local/share/bulmages/openreports/listado.rml" + self.arra[self.j] + " /opt/bulmages/openreports_" + self.database
      self.writecommand(self.string)
      self.process.start(self.string)
      self.process.waitForFinished(-1)
      self.string = "cp /usr/local/share/bulmages/openreports/logo.jpg" + self.arra[self.j] + " /opt/bulmages/openreports_" + self.database
      self.writecommand(self.string)
      self.process.start(self.string)
      self.process.waitForFinished(-1)
      self.string = "cp /usr/local/share/bulmages/openreports/ficha.rml" + self.arra[self.j] + " /opt/bulmages/openreports_" + self.database
      self.writecommand(self.string)
      self.process.start(self.string)
      self.process.waitForFinished(-1)

      # Iteramos sobre la lista de plugins disponibles en bulmafact para copiar sus plantillas
      self.i = 0
      while (self.i < self.mui_plugins.rowCount()):
	# Si el plugin tiene el orden adecuado lo consideramos.
	self.writecommand('Tratando ' + self.mui_plugins.item(self.i,0).text())
	# Si el plugin esta checked lo escribimos.
	if (self.mui_plugins.item(self.i, 0).checkState() == Qt.Checked and len(self.mui_plugins.item(self.i,11).text()) > 3):
	  # Si hay que aplicar un plugin entonces lo escribimos
	  if (self.mui_plugins.item(self.i,11).text() != 'None' and len(self.mui_plugins.item(self.i,11).text()) > 3):
	    # Tratamos toda la cadena de categorias.
	    self.cadreports = self.mui_plugins.item(self.i,11).text().replace('; ',';')
	    self.cadreports = self.cadreports.replace(' ;',';')
	    self.arra = self.cadreports.split(';')
	    self.arra.sort()
	    self.j = 0
	    while (self.j < len ( self.arra)):
	      # Copiamos las plantillas
	      self.string = "cp /usr/local/share/bulmages/openreports/" + self.arra[self.j] + " /opt/bulmages/openreports_" + self.database
	      self.writecommand(self.string)
	      self.process.start(self.string)
	      self.process.waitForFinished(-1)
	      self.j = self.j + 1
	self.i = self.i + 1

      # Abrimos el backup para lectura
      self.file1 = QFile( plugins.configfiles + "bulmacont_" + self.database + ".conf~");
      if not(self.file1.open(QIODevice.ReadOnly | QIODevice.Text)):
        return;
      self.vin = QTextStream(self.file1)
      
      # Abrimos el archivo para escritura.
      self.file = QFile( plugins.configfiles + "bulmacont_" + self.database + ".conf");
      if not(self.file.open(QIODevice.WriteOnly | QIODevice.Text)):
        return;
      self.out = QTextStream(self.file)
      
      # Leemos las lineas iniciales (hasta el parametro deseado) y las ponemos de nuevo.
      self.text = self.vin.readLine()
      while (not (self.text.isNull()) and not(self.text.contains("CONF_DIR_OPENREPORTS")) ):
        self.out << self.text << "\n"
        self.text = self.vin.readLine()

      # Escribimos el parametro como lo deseamos
      self.out << "\n\nCONF_DIR_OPENREPORTS /opt/bulmages/openreports_" + self.database +"/\n\n"


      # Terminamos de poner el resto de las linea.
      if (not (self.text.isNull()) ):
        self.text = self.vin.readLine()
      while (not (self.text.isNull()) ):
        self.out << self.text << "\n"
        self.text = self.vin.readLine()
        
      # Cerramos los ficheros.
      self.file.close()
      self.file1.close()

    
   def writeConfig(self):
      self.writecommand('ESCRIBIENDO CONFIGURACION')
      self.writecommand("Escribiendo configuracion en "+ plugins.configfiles)
      
      # TRATAMOS EL ARCHIVO DE BULMAFACT
      # ================================

      # Tocamos el archivo por si no existe
      self.string = "touch " + plugins.configfiles + "bulmacont_" + self.database + ".conf "
      self.process.start(self.string)
      self.process.waitForFinished(-1)
      self.writecommand(self.process.readAllStandardOutput())

      # Hacemos un backup del archivo
      self.string = "cp " + plugins.configfiles + "bulmacont_" + self.database + ".conf " + plugins.configfiles + "bulmacont_" + self.database + ".conf~ "
      self.process.start(self.string)
      self.process.waitForFinished(-1)
      self.writecommand(self.process.readAllStandardOutput())
      
      # Abrimos el backup para lectura
      self.file1 = QFile( plugins.configfiles + "bulmacont_" + self.database + ".conf~");
      if not(self.file1.open(QIODevice.ReadOnly | QIODevice.Text)):
        return;
      self.vin = QTextStream(self.file1)
      
      # Abrimos el archivo para escritura.
      self.file = QFile( plugins.configfiles + "bulmacont_" + self.database + ".conf");
      if not(self.file.open(QIODevice.WriteOnly | QIODevice.Text)):
        return;
      self.out = QTextStream(self.file)
      
      # Leemos las lineas iniciales (hasta el parametro deseado) y las ponemos de nuevo.
      self.text = self.vin.readLine()
      while (not (self.text.isNull()) and not(self.text.contains("CONF_PLUGINS_BULMACONT")) ):
        self.out << self.text << "\n"
        self.text = self.vin.readLine()
        
      # Quitamos todo lo que es el parametro porque no debe estar
      while (not (self.text.isNull()) and self.text.contains("lib")):
        self.text = self.vin.readLine()
      
      self.nuevo = 1
   
      # Como los plugins van por orden iteramos sobre el orden para arreglarlo.
      self.x = 1
      while (self.x < 1000) :
        # Iteramos sobre la lista de plugins disponibles en bulmafact
        self.i = 0
        while (self.i < self.mui_plugins.rowCount()):
          # Si el plugin tiene el orden adecuado lo consideramos.
          if (str(self.mui_plugins.item(self.i,7).text()) == str(self.x )):
            self.writecommand('Tratando ' + self.mui_plugins.item(self.i,0).text())
            # Si el plugin esta checked lo escribimos.
            if (self.mui_plugins.item(self.i, 0).checkState() == Qt.Checked and len(self.mui_plugins.item(self.i,1).text()) > 3):
              if (self.nuevo == 1):
                 self.nuevo = 0
                 # Escribimos la configuracion de plugins.
                 self.terminador = ""
                 self.out << "CONF_PLUGINS_BULMACONT   "
	      # Si hay que aplicar un plugin entonces lo escribimos
	      if (self.mui_plugins.item(self.i,10).text() != 'None' and len(self.mui_plugins.item(self.i,10).text()) > 3):
		self.writecommand('Hay que actualizar ' + self.mui_plugins.item(self.i,0).text())
		self.out << self.terminador << self.mui_plugins.item(self.i,10).text()
		self.terminador = "; \\\n";
          self.i = self.i + 1
        self.x = self.x + 1
      self.out << "\n"
      
      # Terminamos de poner el resto de las linea.
      while (not (self.text.isNull()) ):
        self.out << self.text << "\n"
        self.text = self.vin.readLine()
        
      # Cerramos los ficheros.
      self.file.close()
      self.file1.close()

      self.trataOpenReports()

   def on_mui_categoria_currentIndexChanged(self, index):
      self.presentar()
  
  
   def on_mui_vertodos_stateChanged(self, status):
      self.presentar()
  
   def presentar(self):
      status = self.mui_vertodos.checkState()
      
      # Hacemos todas las lineas visibles para luego ocultarlas.
      self.i = 0
      while (self.i < self.mui_plugins.rowCount()):
        self.mui_plugins.showRow(self.i)
        self.i = self.i + 1
      
      if (status == 2):
        # Establezco la tabla de bulmafact
        self.i = 0
        while (self.i < self.mui_plugins.rowCount()):
          if (self.mui_plugins.item(self.i, 0).checkState() != Qt.Checked ):
            self.mui_plugins.hideRow(self.i)
          self.i = self.i + 1

      if (self.mui_categoria.currentIndex() > 0):
        # Vamos a trabajar con el combo Box
        cat = self.mui_categoria.currentText()
        self.i = 0
        while (self.i < self.mui_plugins.rowCount()):
	  if self.mui_plugins.item(self.i,8) <> None:
            text = QString(self.mui_plugins.item(self.i,8).text())
            a = text.contains(cat)
            if (not a):
              self.mui_plugins.hideRow(self.i)
          self.i = self.i +1



