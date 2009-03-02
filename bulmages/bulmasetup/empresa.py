# -*- coding: utf-8 -*-

import sys
import os
from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from plugins import PluginsBulmaSetup
import plugins

class Empresa(QtGui.QDialog, PluginsBulmaSetup):
   def __init__(self, database, parent = None):
      PluginsBulmaSetup.__init__(self)
      QtGui.QDialog.__init__(self,parent)
      self.database = database

      # Establecemos cual va a ser la base de datos con la que trabajaremos todo el rato
      self.process = QtCore.QProcess()
#      self.connect(self.process, SIGNAL("readyReadStandardOutput()"), self.readOutput)
      self.connect(self.process, SIGNAL("readyReadStandardError()"), self.readErrors)
      self.connect(self.process, SIGNAL("finished()"), self.finished)
      self.connect(self.process, SIGNAL("started()"), self.started)

   def guardaQuery(self, query):
      self.query = query
      self.fileHandle = open ( '/tmp/query.sql', 'w' )
      self.fileHandle.write ( query )
      self.fileHandle.close()

   def execQuery(self, query):
      if (self.database != ''):
        self.writecommand(QString("----") + self.database + QString("----"))
        self.subcomand = query
        self.guardaQuery(self.subcomand)
        self.command = 'su postgres -c \"psql -t -f /tmp/query.sql ' + self.database + '\"'
        self.writecommand(self.command)
        self.process.start(self.command)
        self.process.waitForFinished(-1)
        return QString(self.process.readAllStandardOutput())
      return QString('')
   
   def readOutput(self):
      self.mui_textBrowser.append(QString(self.process.readAllStandardOutput()))

   def readErrors(self):
      self.mui_textBrowser.append("<font color =\"#FF0000\">error: " + QString(self.process.readAllStandardError()) + "</font>")

   def finished(self):
      self.mui_textBrowser.append("<font color =\"#00FF00\">Done.</font>")

   def started(self):
      self.mui_textBrowser.append("<font color =\"#00FF00\">Start.</font>")

   def writecommand(self, comm):
      self.mui_textBrowser.append("<font color =\"#0000FF\">"+comm+"</font>")
