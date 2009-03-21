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
      self.mui_plugins.hideColumn(1)
      # Buscamos los Plugins
      self.buscaPlugins()
      # Ajustamos la presentacion
      self.mui_plugins.resizeColumnsToContents()


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
      self.revisiones = ["rev-0.5.3.sql", "rev-0.9.1.sql", "rev-0.9.3.sql", "rev-0.10.sql", "rev-0.11.sql"]
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
      print self.plugins
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
      print '-->' + plugin +  ' ' + libreria
      self.version = self.execQuery('SELECT valor FROM configuracion WHERE nombre = \'' + plugin +'\'').replace('\n','').replace(' ','')
      if (len(self.version) > 2):
         return self.version
   
      if (libreria == ''):
         return ''
   
      self.mfile = QFile(plugins.configfiles + 'bulmacont_' + self.database + '.conf')
      if (self.mfile.exists()):
         self.command = 'grep '+libreria+' '+ plugins.configfiles + 'bulmacont_' + self.database + '.conf'
         self.writecommand(self.command)
         self.process.start(self.command)
         self.process.waitForFinished(-1)
         self.version = self.process.readAllStandardOutput()

      if (self.version != ''):
         self.version = '0.11'
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
         self.writecommand('Tratando ' + self.pluginsbulmacont[self.i][0])
         if (self.mui_plugins.item(self.i, 0).checkState() == Qt.Checked):
            self.writecommand('Ha que actualizar ' + self.pluginsbulmacont[self.i][0])
            if (len(self.pluginsbulmacont[self.i][4]) > 0):
               self.command = 'su postgres -c \"psql -t -f ' + plugins.pathdbplugins + self.pluginsbulmacont[self.i][4] + ' ' + self.database + '\"'
               self.writecommand(self.command)
               self.process.start(self.command)
               self.process.waitForFinished(-1)
               self.writecommand(self.process.readAllStandardOutput())
         else:
            # Si no esta chequeado hacemos un borrado del plugin
            if (len(self.pluginsbulmacont[self.i][9]) > 0):
              # Aplicamos el parche  de borrado.
              self.command = 'su postgres -c \"psql -t -f  ' + plugins.pathdbplugins + self.pluginsbulmacont[self.i][9] +' '+ self.database +'\"'
              self.writecommand(self.command)
              self.process.start(self.command)
              self.process.waitForFinished(-1)
              self.writecommand(self.process.readAllStandardOutput())
         self.i = self.i + 1
         self.progress.setValue(self.progress.value() + 1)
      self.progress.hide()

   def marcar(self, plug):
      self.j = 0
      for self.it in self.pluginsbulmacont:
         if (self.pluginsbulmacont[self.j][1] == plug):
            self.mui_plugins.item(self.j,0).setCheckState(Qt.Checked)
         self.j = self.j + 1

   def desmarcar(self, plug):
      self.j = 0
      for self.it in self.pluginsbulmacont:
         if (self.pluginsbulmacont[self.j][1] == plug):
            self.mui_plugins.item(self.j,0).setCheckState(Qt.Unchecked)
         self.j = self.j + 1

   def on_mui_plugins_cellClicked(self, row, col):
      self.mui_descripcion.setText(self.mui_plugins.item(row,1).text())
      if (self.semaforo == 1):
         # Marcamos las dependencias
         self.i = 0
         while (self.i < self.mui_plugins.rowCount()):
            if (self.mui_plugins.item(self.i, 0).checkState() == Qt.Checked):
               self.arr = self.pluginsbulmacont[self.i][5].split(QString(","))
               for self.dep in self.arr:
                  self.marcar(self.dep)
            self.i = self.i +1
         # Desmarcamos las incompatibilidades
         self.arr = self.pluginsbulmacont[row][6].split(QString(","))
         for self.dep in self.arr:
            self.desmarcar(self.dep)
         self.i = 0
         while (self.i < self.mui_plugins.rowCount()):
            if (self.mui_plugins.item(self.i, 0).checkState() == Qt.Checked):
               self.arr = self.pluginsbulmacont[self.i][6].split(QString(","))
               for self.dep in self.arr:
                  self.desmarcar(self.dep)
            self.i = self.i +1


    
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
          if (self.pluginsbulmacont[self.i][7] == self.x ):
            self.writecommand('Tratando ' + self.pluginsbulmacont[self.i][0])
            # Si el plugin esta checked lo escribimos.
            if (self.mui_plugins.item(self.i, 0).checkState() == Qt.Checked and len(self.pluginsbulmacont[self.i][1]) > 3):
              if (self.nuevo == 1):
                 self.nuevo = 0
                 # Escribimos la configuracion de plugins.
                 self.terminador = ""
                 self.out << "CONF_PLUGINS_BULMACONT   "

              self.writecommand('Hay que actualizar ' + self.pluginsbulmacont[self.i][0])
              self.out << self.terminador << self.pluginsbulmacont[self.i][1]
              self.terminador = "; \\\n";
          self.i = self.i + 1
        self.x = self.x + 1
      self.out << "\n"
      
      # Terminamos de poner el resto de las linea.
      while (not (self.text.isNull()) ):
        self.out << self.text << "\n"
        self.text = self.vin.readLine()
        
      # Cerramos slo ficheros.
      self.file.close()
      self.file1.close()

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
        # VAmos a trabajar con el combo Box
        cat = self.mui_categoria.currentText()
        self.i = 0
        while (self.i < self.mui_plugins.rowCount()):
          text = QString(self.pluginsbulmacont[self.i][8])
          a = text.contains(cat)
          if (not a):
            self.mui_plugins.hideRow(self.i)
          self.i = self.i +1



