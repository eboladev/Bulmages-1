TEMPLATE = subdirs 

SUBDIRS += src
SUBDIRS += src/plugin

exists(src/plugingorgblau) {
    SUBDIRS += src/plugingorgblau
}


#CONFIG += release \
#          warn_on \
#          qte \
#          thread 

