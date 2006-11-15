TEMPLATE = subdirs 

CONFIG += ordered

SUBDIRS += src
SUBDIRS += src/plugin
SUBDIRS += src/plugintarifas
#SUBDIRS += src/pluginstockmin
SUBDIRS += src/comercialbf
SUBDIRS += src/efacturabf
SUBDIRS += src/pluginvehiculosbf

include(privatecode.pro)
