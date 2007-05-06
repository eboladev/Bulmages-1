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
SUBDIRS += src/plugininventario

include(privatecode.pro)
