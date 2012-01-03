# -*- coding: utf-8 -*-

import sys
import os,fnmatch
import re
import locale

global pathdbbulmafact
global pathdbbulmacont
global pathdbparches
global pathdbplugins
global configfiles
global confsharebulmages
global confopenreports

class PluginsBulmaSetup:
    def __init__(self):

        self.pluginsbulmafact = []
        self.pluginsbulmacont = []
        self.pluginsbulmatpv  = []

        self.readfiles("bulmafact-plugins",self.pluginsbulmafact)
        self.readfiles("bulmalib-plugins",self.pluginsbulmafact)
        self.readfiles("bulmacont-plugins",self.pluginsbulmacont)
        self.readfiles("bulmalib-plugins",self.pluginsbulmacont)
        self.readfiles("bulmatpv-plugins",self.pluginsbulmatpv)


    def readfiles(self, folder, plugins):
        if os.path.exists(confsharebulmages+folder):
            for fileName in os.listdir ( confsharebulmages+folder ):
	        #Intentamos coger el idioma. Si resulta imposible hacerlo ponemos por defecto el castellano.
	        idioma = "es"
	        if locale.getdefaultlocale()[0] != None:
		    idioma = locale.getdefaultlocale()[0][:2]
                if fnmatch.fnmatch ( fileName, 'README.'+idioma+'*' ):
                    f = open(confsharebulmages+folder+"/"+fileName)
                    cont = ""
                    for char in f.read():
                        cont = cont + char
                    nombre = ""
                    biblioteca = ""
                    descripcion = ""
                    parmdb = ""
                    archivosqlpatch = ""
                    archivosqldeins = ""
                    dependencias = ""
                    incompatibles = ""
                    orden = ""
                    categorias = ""
                    reports = ""
                    autoformfile = ""
                    m = re.search('Nombre: (.*)', cont)
                    if m <> None:
                        nombre = m.group(1)
                    m = re.search('Biblioteca: (.*)', cont)
                    if m <> None:
                        biblioteca = m.group(1)
                    m = re.search("(?smu)Descripción: (.*)\n\nParm", cont)
                    if m <> None:
                        descripcion = m.group(1)
                    m = re.search('ParmBD: (.*)', cont)
                    if m <> None:
                        parmdb = m.group(1)
                    m = re.search('ArchivoSQLpatch: (.*)', cont)
                    if m <> None:
                        archivosqlpatch = m.group(1)
                    m = re.search('ArchivoSQLdeins: (.*)', cont)
                    if m <> None:
                        archivosqldeins = m.group(1)
                    m = re.search('Dependencias: (.*)', cont)
                    if m <> None:
                        dependencias = m.group(1)
                    m = re.search('Categorias: (.*)', cont)
                    if m <> None:
                        categorias = m.group(1)
                    m = re.search('Incompatibles: (.*)', cont)
                    if m <> None:
                        incompatibles = m.group(1)
                    m = re.search('Reports: (.*)', cont)
                    if m <> None:
                        reports = m.group(1)
                    m = re.search('AutoFormFile: (.*)', cont)
                    if m <> None:
                        autoformfile = m.group(1)
                    m = re.search('Orden: (.*)', cont)
                    orden = 999
                    if m <> None:
                        if (m.group(1).isdigit()):
                            orden = int(m.group(1))
                    plugins.append([nombre, biblioteca, descripcion, parmdb, archivosqlpatch, dependencias, incompatibles, orden, categorias, archivosqldeins, reports, autoformfile])


if __name__=="__main__":
    pathdbbulmafact = ""
    pathdbbulmacont = ""
    pathdbparches = ""
    configfiles = ""
    confsharebulmages = "@BULMAGES_OPTWARE_DIR@/share/bulmages/"
    win=PluginsBulmaSetup()
