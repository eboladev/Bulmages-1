TEMPLATE = subdirs 

SUBDIRS += src
SUBDIRS += src/plugin

exists(src/gorgblau) {
    SUBDIRS += src/gorgblau
}


#CONFIG += release \
#          warn_on \
#          qte \
#          thread 

