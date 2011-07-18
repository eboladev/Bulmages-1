# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(quazip)

set(quazip_SRCS ioapi.c
           quazip.cpp
           quazipfile.cpp
           quazipnewinfo.cpp
           unzip.c
           zip.c )

QT4_WRAP_CPP(quazip_SRCS quazipfile.h)

add_library(quazip SHARED ${quazip_SRCS})

set_target_properties(quazip PROPERTIES	VERSION ${QUAZIP_VERSION}
                                        SOVERSION ${QUAZIP_VERSION})


target_link_libraries(quazip ${QT_QTCORE_LIBRARY} ${ZLIB_LIBRARY} )

install(TARGETS quazip LIBRARY DESTINATION lib)
install(FILES crypt.h  ioapi.h  quazipfile.h  quazipfileinfo.h  quazip.h  quazipnewinfo.h  unzip.h  zip.h
 DESTINATION include/quazip COMPONENT Devel)
