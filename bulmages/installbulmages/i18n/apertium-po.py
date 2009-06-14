#!/usr/bin/python
# -*- coding: utf-8 -*-
# necessita els paquets apertium-dbus apertium-ca-es de debian
# necessita polib >=? 0.4.2 de  http://code.google.com/p/polib/ 
# (no està a debian, crec)
# i pot necessitar fixar la codificació de python a utf-8 
# segons explica http://wiki.apertium.org/wiki/Apertium-tolk#Python_encodings
#
# Se li passen dos fitxers .po
# Usa les cadenes traduïdes del primer fitxer po per traduir les que 
# no ho estiguin del segon, passant-les per apertium. No passa les 
# mesgids per aprtium perquè en el cas del bulmages tenen massa errades
# ortografiques. Simplement busca totes les cadenes sense traduir 
# del segon fitxer, i hi posa les traduccions que dóna l'apertium 
# a partir dels msgstr que hi ha la primer fitxer pel mateix msgid.
# grava el resultat en un nou fitxer que es diu com el segon amb un prefix auto
# les traduccions que posa les marca fuzzy per a que siguin revisades,
# perquè evidentment les traduccions automàtiques fallaran sovint. 
import os,sys
import time
import re
import polib
#import Lokalize.Editor
import dbus

pfitxer = re.compile("^.*((\\w{2})_\\w{2})\\.[^\\.]+\\.po$")
translator = dbus.Interface(dbus.SessionBus().get_object("org.apertium.mode", "/"), "org.apertium.Translate")
info  = dbus.Interface(dbus.SessionBus().get_object("org.apertium.info", "/"), "org.apertium.Info")

modes = info.modes()

def tradueix(fitxer_origen,fitxer_desti,parell):
   #polib.encoding='utf-8';
   po_origen=polib.pofile(fitxer_origen)#, autodetect_encoding=False)
   orig={}
   for entry in po_origen.translated_entries():
       orig[entry.msgid]=entry.msgstr
   po_desti=polib.pofile(fitxer_desti)#, autodetect_encoding=False)
   for entry in po_desti.untranslated_entries():
       orig_msgstr=orig[entry.msgid]
       if orig_msgstr :
          print "tradueixo %s\n" % orig_msgstr 
          entry.msgstr= translator.translate(parell, {}, orig_msgstr)
          print "dóna %s\n" % entry.msgstr
          if not 'fuzzy' in entry.flags:
             entry.flags.append('fuzzy')   
   po_desti.save('auto%s' % fitxer_desti)

def tradueix_comtoqui(fitxer_origen,fitxer_desti):
   m=pfitxer.match(fitxer_desti)
   if (m):
     destinacio=(m.group(1),m.group(2))
   else:
     raise 'No entenc el nom del fitxer  destinació %s' % fitxer_desti
   
   m=pfitxer.match(fitxer_origen)
   
   if (m):
     origen=(m.group(1),m.group(2))
   else:
     raise 'No entenc el nom del fitxer  origen %s' % fitxer_origen

   for o in origen:
      for d in destinacio:
         parell = '%s-%s' % (o,d)
         if parell in modes:
            print "traduirem %s\n" % parell
            tradueix(fitxer_origen,fitxer_desti,parell)
            return 

tradueix_comtoqui(sys.argv[1],sys.argv[2]);

