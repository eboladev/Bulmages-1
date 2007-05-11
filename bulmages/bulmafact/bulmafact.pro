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
SUBDIRS += src/plugincontratos

#Nota: Para implementar plugins que no van a ser liberados dentro del ambito de la asociacion IGLUES
# como por ejemplo implementacions especificas. La mejor opcion es incluirlos dentro del directorio
# src (sin tenerlos en el repositorio publico, es decir sin hacer commit de dichos plugins)
# e incluirlos dentro del archivo privatecode.pro dentro de este mismo directorio.
# de esta forma se podran combinar los ultimos cambios realizados por la asociacion con los
# plugins no liberados sin ningun esfuerzo administrativo.

# Dentro del archivo privatecode.pro basta tener una directiva SUBDIRS apuntando al directorio del
# plugin.
include(privatecode.pro)


