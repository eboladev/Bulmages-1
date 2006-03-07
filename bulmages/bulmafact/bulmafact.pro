TEMPLATE = subdirs 

SUBDIRS += src
SUBDIRS += src/plugin
SUBDIRS += src/plugintarifas

exists(src/gorgblau) {
    SUBDIRS += src/gorgblau
}

exists(src/mserra) {
    SUBDIRS += src/mserra
}


#CONFIG += release \
#          warn_on \
#          qte \
#          thread 

