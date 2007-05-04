
SUBDIRS += src/plugincatalogo
SUBDIRS += src/plugininformeclientes

r1964{
include(privatecode.pro)
TEMPLATE = subdirs 

CONFIG += ordered

SUBDIRS += src
SUBDIRS += src/plugin
SUBDIRS += src/plugintarifas
#SUBDIRS += src/pluginstockmin
SUBDIRS += src/comercialbf
SUBDIRS += src/efacturabf
SUBDIRS += src/pluginvehiculosbf
SUBDIRS += src/plugintrazabilidad
SUBDIRS += src/pluginbarcodeopen
SUBDIRS += src/plugincatalogo
SUBDIRS += src/plugininformeclientes

include(privatecode.pro)
