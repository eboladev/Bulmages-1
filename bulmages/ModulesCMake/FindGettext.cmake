# Try to find Gettext functionality
# Once done this will define
#
#  GETTEXT_FOUND - system has Gettext
#  GETTEXT_INCLUDE_DIR - Gettext include directory
#  GETTEXT_LIBRARIES - Libraries needed to use Gettext

# TODO: This will enable translations only if Gettext functionality is
# present in libc. Must have more robust system for release, where Gettext
# functionality can also reside in standalone Gettext library, or the one
# embedded within kdelibs (cf. gettext.m4 from Gettext source).
#
# Copyright (c) 2006, Chusslove Illich, <caslav.ilic@gmx.net>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
# Differences between CMake 2.4 and 2.6

if (LIBC_HAS_DGETTEXT OR LIBINTL_HAS_DGETTEXT)

  # in cache already
  SET(GETTEXT_FOUND TRUE)

else (LIBC_HAS_DGETTEXT OR LIBINTL_HAS_DGETTEXT)

  include(CheckIncludeFiles)
  include(CheckLibraryExists)
  include(CheckFunctionExists)
  
  check_include_files(libintl.h HAVE_LIBINTL_H)
  
  set(GETTEXT_INCLUDE_DIR)
  set(GETTEXT_LIBRARIES)
  
  if (HAVE_LIBINTL_H)
     check_function_exists(dgettext LIBC_HAS_DGETTEXT)
     if (LIBC_HAS_DGETTEXT)
        set(GETTEXT_SOURCE "built in libc")
        set(GETTEXT_FOUND TRUE)
     else (LIBC_HAS_DGETTEXT)
        FIND_LIBRARY(LIBINTL_LIBRARY NAMES intl libintl )

        CHECK_LIBRARY_EXISTS(${LIBINTL_LIBRARY} "dgettext" "" LIBINTL_HAS_DGETTEXT)
        if (LIBINTL_HAS_DGETTEXT)
           set(GETTEXT_SOURCE "in ${LIBINTL_LIBRARY}")
           set(GETTEXT_LIBRARIES ${LIBINTL_LIBRARY} CACHE FILEPATH "path to libintl library, used for gettext")
           set(GETTEXT_FOUND TRUE)
        endif (LIBINTL_HAS_DGETTEXT)
     endif (LIBC_HAS_DGETTEXT)
  endif (HAVE_LIBINTL_H)
  
  if (GETTEXT_FOUND)
     if (NOT Gettext_FIND_QUIETLY)
        message(STATUS "Found Gettext: ${GETTEXT_SOURCE}")
     endif (NOT Gettext_FIND_QUIETLY)
  else (GETTEXT_FOUND)
     if (Gettext_FIND_REQUIRED)
        message(FATAL_ERROR "Could NOT find Gettext")
     endif (Gettext_FIND_REQUIRED)
  endif (GETTEXT_FOUND)
  
  MARK_AS_ADVANCED(GETTEXT_INCLUDE_DIR GETTEXT_LIBRARIES)

endif (LIBC_HAS_DGETTEXT OR LIBINTL_HAS_DGETTEXT)


# - Find GNU gettext tools
# This module looks for the GNU gettext tools. This module defines the 
# following values:
#  GETTEXT_MSGMERGE_EXECUTABLE: the full path to the msgmerge tool.
#  GETTEXT_MSGFMT_EXECUTABLE: the full path to the msgfmt tool.
#  GETTEXT_FOUND: True if gettext has been found.
#
# Additionally it provides the following macros:
# GETTEXT_CREATE_TRANSLATIONS ( outputFile [ALL] file1 ... fileN )
#    This will create a target "translations" which will convert the 
#    given input po files into the binary output mo file. If the 
#    ALL option is used, the translations will also be created when
#    building the default target.
# GETTEXT_PROCESS_POT( <potfile> [ALL] [INSTALL_DESTINATION <destdir>] <lang1> <lang2> ... )
#     Process the given pot file to mo files.
#     If INSTALL_DESTINATION is given then automatically install rules will be created,
#     the language subdirectory will be taken into account (by default use share/locale/).
#     If ALL is specified, the pot file is processed when building the all traget.
#     It creates a custom target "potfile".
#
# GETTEXT_PROCESS_PO_FILES(<lang> [ALL] [INSTALL_DESTINATION <dir>] <po1> <po2> ...)
#     Process the given po files to mo files for the given language.
#     If INSTALL_DESTINATION is given then automatically install rules will be created,
#     the language subdirectory will be taken into account (by default use share/locale/).
#     If ALL is specified, the po files are processed when building the all traget.
#     It creates a custom target "pofiles".



FIND_PROGRAM(GETTEXT_MSGMERGE_EXECUTABLE msgmerge)

FIND_PROGRAM(GETTEXT_MSGFMT_EXECUTABLE msgfmt)

FIND_PROGRAM(GETTEXT_XGETTEXT_EXECUTABLE xgettext)

IF (GETTEXT_MSGMERGE_EXECUTABLE AND GETTEXT_MSGFMT_EXECUTABLE )
   SET(GETTEXT_FOUND TRUE)
ELSE (GETTEXT_MSGMERGE_EXECUTABLE AND GETTEXT_MSGFMT_EXECUTABLE )
   SET(GETTEXT_FOUND FALSE)
   IF (GetText_REQUIRED)
      MESSAGE(FATAL_ERROR "GetText not found")
   ENDIF (GetText_REQUIRED)
ENDIF (GETTEXT_MSGMERGE_EXECUTABLE AND GETTEXT_MSGFMT_EXECUTABLE )






MACRO(GETTEXT_CREATE_TRANSLATIONS _potFile _firstPoFile)

   SET(_gmoFiles)
   GET_FILENAME_COMPONENT(_potBasename ${_potFile} NAME_WE)
   GET_FILENAME_COMPONENT(_absPotFile ${_potFile} ABSOLUTE)

   SET(_addToAll)
   IF(${_firstPoFile} STREQUAL "ALL")
      SET(_addToAll "ALL")
      SET(_firstPoFile)
   ENDIF(${_firstPoFile} STREQUAL "ALL")

   FOREACH (_currentPoFile ${ARGN})
      GET_FILENAME_COMPONENT(_absFile ${_currentPoFile} ABSOLUTE)
      GET_FILENAME_COMPONENT(_abs_PATH ${_absFile} PATH)
      GET_FILENAME_COMPONENT(_lang ${_absFile} NAME_WE)
      SET(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_lang}.gmo)

      ADD_CUSTOM_COMMAND( 
         OUTPUT ${_gmoFile} 
         COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} --quiet --update --backup=none -s ${_absFile} ${_absPotFile}
         COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${_gmoFile} ${_absFile}
         DEPENDS ${_absPotFile} ${_absFile} 
      )

      INSTALL(FILES ${_gmoFile} DESTINATION share/locale/${_lang}/LC_MESSAGES RENAME ${_potBasename}.mo) 
      SET(_gmoFiles ${_gmoFiles} ${_gmoFile})

   ENDFOREACH (_currentPoFile )

   ADD_CUSTOM_TARGET(translations ${_addToAll} DEPENDS ${_gmoFiles})

ENDMACRO(GETTEXT_CREATE_TRANSLATIONS )

# GETTEXT_PROCESS_POT( <potfile> [ALL] [INSTALL_DESTINATION <destdir>] <lang1> <lang2> ... )
MACRO(GETTEXT_PROCESS_POT_FILE _potFile)

   SET(_gmoFiles)
   SET(_args ${ARGN})
   SET(_addToAll)
   SET(_installDest)

   IF(_args)
      LIST(GET _args 0 _tmp)
      IF("${_tmp}" STREQUAL "ALL")
         SET(_addToAll ALL)
         LIST(REMOVE_AT _args 0)
      ENDIF("${_tmp}" STREQUAL "ALL")
   ENDIF(_args)

   IF(_args)
      LIST(GET _args 0 _tmp)
      IF("${_tmp}" STREQUAL "INSTALL_DESTINATION")
         LIST(GET _args 1 _installDest )
         LIST(REMOVE_AT _args 0 1)
      ENDIF("${_tmp}" STREQUAL "INSTALL_DESTINATION")
   ENDIF(_args)

   GET_FILENAME_COMPONENT(_potBasename ${_potFile} NAME_WE)
   GET_FILENAME_COMPONENT(_absPotFile ${_potFile} ABSOLUTE)

#    message(STATUS "1 all ${_addToAll} dest ${_installDest} args: ${_args}")

   FOREACH (_lang ${_args})
      SET(_poFile  "${CMAKE_CURRENT_BINARY_DIR}/${_lang}.po")
      SET(_gmoFile "${CMAKE_CURRENT_BINARY_DIR}/${_lang}.gmo")

      ADD_CUSTOM_COMMAND( 
         OUTPUT "${_poFile}" 
         COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} --quiet --update --backup=none -s ${_poFile} ${_absPotFile}
         DEPENDS ${_absPotFile}
      )

      ADD_CUSTOM_COMMAND( 
         OUTPUT "${_gmoFile}"
         COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${_gmoFile} ${_poFile}
         DEPENDS ${_absPotFile} ${_poFile} 
      )

      IF(_installDest)
         INSTALL(FILES ${_gmoFile} DESTINATION ${_installDest}/${_lang}/LC_MESSAGES RENAME ${_potBasename}.mo) 
      ENDIF(_installDest)
      LIST(APPEND _gmoFiles ${_gmoFile})

   ENDFOREACH (_lang )

   ADD_CUSTOM_TARGET(potfile ${_addToAll} DEPENDS ${_gmoFiles})

ENDMACRO(GETTEXT_PROCESS_POT_FILE)


# GETTEXT_PROCESS_PO_FILES(<lang> [ALL] [INSTALL_DESTINATION <dir>] <po1> <po2> ...)
MACRO(GETTEXT_PROCESS_PO_FILES _lang)
   SET(_gmoFiles)
   SET(_args ${ARGN})
   SET(_addToAll)
   SET(_installDest)

   LIST(GET _args 0 _tmp)
   IF("${_tmp}" STREQUAL "ALL")
      SET(_addToAll ALL)
      LIST(REMOVE_AT _args 0)
   ENDIF("${_tmp}" STREQUAL "ALL")
   
   LIST(GET _args 0 _tmp)
   IF("${_tmp}" STREQUAL "INSTALL_DESTINATION")
      LIST(GET _args 1 _installDest )
      LIST(REMOVE_AT _args 0 1)
   ENDIF("${_tmp}" STREQUAL "INSTALL_DESTINATION")
   
#    message(STATUS "2 all ${_addToAll} dest ${_installDest} args: ${_args}")
   
   FOREACH(_current_PO_FILE ${_args})
      GET_FILENAME_COMPONENT(_basename ${_current_PO_FILE} NAME_WE)
      SET(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.gmo)
      add_custom_command(OUTPUT ${_gmoFile}
            COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${_gmoFile} ${_current_PO_FILE}
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            DEPENDS ${_current_PO_FILE}
         )

      IF(_installDest)
         INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.gmo DESTINATION ${_installDest}/${_lang}/LC_MESSAGES/ RENAME ${_basename}.mo)
      ENDIF(_installDest)
      LIST(APPEND _gmoFiles ${_gmoFile})
   ENDFOREACH(_current_PO_FILE)
   ADD_CUSTOM_TARGET(pofiles ${_addToAll} DEPENDS ${_gmoFiles})
ENDMACRO(GETTEXT_PROCESS_PO_FILES)


# This macro creates a file in file_list this the contens of
# the var lsource. Each parameter is in a line.
# the macro erase the file previously
macro(CREATE_FILE_LIST lsource file_list)

if(WIN32)
	string(ASCII 10 13 line_r)
else(WIN32)
	string(ASCII 10 line_r)
endif(WIN32)

file(REMOVE ${file_list})

foreach(_file ${${lsource}})
    GET_FILENAME_COMPONENT(_FileName ${_file} NAME)
    file(APPEND ${file_list} ${_FileName})
    file(APPEND ${file_list} ${line_r})
endforeach(_file ${lsource}) 

endmacro(CREATE_FILE_LIST)



# GETTEXT_CREATE_TEMPLATE( template directory sources)
# this macro creates the pot file template. It needs: 
# template, the file output with the name template.pot
# dirIN, the directory where to find the files to process.
# sources, a file with the list of the files to process 
macro(GETTEXT_CREATE_TEMPLATE template dirIN sources )
	
	set (MSGID_BUGS_ADDRESS "bulmages@bulma.net")
	file(REMOVE ${CMAKE_CURRENT_SOURCE_DIR}/po/${template}.pot)
	#message (STATUS "Files to create list ${${sources}}")

	CREATE_FILE_LIST(${sources} ${CMAKE_CURRENT_BINARY_DIR}/pot_list)
	
	add_custom_command( 
	OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/po/${template}.pot
	COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE} 
   ARGS --add-comments=TRANSLATORS: 
   ARGS --qt
   ARGS --keyword=tr --flag=tr:1:pass-c-format --flag=tr:1:pass-qt-format
   ARGS --keyword=trUtf8 --flag=tr:1:pass-c-format --flag=tr:1:pass-qt-format
   ARGS --keyword=translate:2 --flag=translate:2:pass-c-format --flag=translate:2:pass-qt-format
   ARGS --keyword=QT_TR_NOOP --flag=QT_TR_NOOP:1:pass-c-format --flag=QT_TR_NOOP:1:pass-qt-format
   ARGS --keyword=QT_TRANSLATE_NOOP:2 --flag=QT_TRANSLATE_NOOP:2:pass-c-format --flag=QT_TRANSLATE_NOOP:2:pass-qt-format
   ARGS --keyword=_ --flag=_:1:pass-c-format --flag=_:1:pass-qt-format
   ARGS --keyword=N_ --flag=N_:1:pass-c-format --flag=N_:1:pass-qt-format
   ARGS --from-code=utf-8
   ARGS --directory=${dirIN} --directory=${CMAKE_CURRENT_BINARY_DIR} 
   ARGS --output=${CMAKE_CURRENT_SOURCE_DIR}/po/${template}.pot
   ARGS --msgid-bugs-address=${MSGID_BUGS_ADDRESS} --files-from=${CMAKE_CURRENT_BINARY_DIR}/pot_list
   DEPENDS ${template} VERBATIM)
           
   add_custom_target( messages_extract DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/po/${template}.pot)

endmacro( GETTEXT_CREATE_TEMPLATE)






# $XGETTEXT --default-domain=$DOMAIN --directory=. \
# 	      --add-comments=TRANSLATORS: $XGETTEXT_OPTIONS \
# 	      --copyright-holder='$COPYRIGHT_HOLDER' \
# 	      --package-version='$VERSION' \
# 	      --msgid-bugs-address="$msgid_bugs_address" \
# 	      *.cpp 


# .po to .gmo stuff
# FILE(GLOB _pofiles *.po)
# 
# FOREACH(_file ${_pofiles})
# 	GET_FILENAME_COMPONENT(_file_we ${_file} NAME_WE)
# 	SET(_out "${_file_we}.gmo")
# 	SET(_in "${_file_we}.po")
# 	
# 	ADD_CUSTOM_COMMAND(
# 		OUTPUT ${_out}
# 		COMMAND ${MSGFMT_EXECUTABLE} -o ${_out} ${_in}
# 		DEPENDS ${_in}
# 	)
# 	
# 	INSTALL(
# 		FILES ${_out}
# 		DESTINATION share/locale/${_file_we}/LC_MESSAGES/
# 		RENAME gtkqtengine.mo
# 	)
# 	
# 	SET(_outputs ${_outputs} ${_out})
# ENDFOREACH(_file)
# 
# ADD_CUSTOM_TARGET(
# 	pofiles ALL
# 	DEPENDS ${_outputs}
# )
# 
# 
# # Stuff to generate the .pot
# FILE(GLOB POT_UIFILES ../kcm_gtk/*.ui)
# SET(POT_SOURCES ../kcm_gtk/kcmgtk.cpp)
# SET(POT_OUTPUT gtkqtengine.pot)
# 
# ADD_CUSTOM_COMMAND(
# 	OUTPUT _${POT_OUTPUT}
# 	COMMAND ${EXTRACTRC_PATH} ${POT_UIFILES} > rc.cpp
# 	COMMAND ${XGETTEXT_PATH} --foreign-user -C -ci18n -ki18n -ktr2i18n
# 		-kI18N_NOOP -kI18N_NOOP2 -kaliasLocale -o ${POT_OUTPUT} rc.cpp ${POT_SOURCES}
# 	COMMAND rm rc.cpp
# )
# 
# ADD_CUSTOM_TARGET(
# 	extract_messages
# 	DEPENDS _${POT_OUTPUT}
# )
# 
# 
# 
# FILE ( GLOB_RECURSE TELLICO_SOURCES *.h *.h *.cpp )
# IF ( ${XGETTEXT} STREQUAL "XGETTEXT-NOTFOUND" )
# ELSE ( ${XGETTEXT} STREQUAL "XGETTEXT-NOTFOUND" )
#   ADD_CUSTOM_TARGET( messages
#     COMMAND ${XGETTEXT} --join-existing --add-comments="TRANSLATORS:" -o ${TELLICO_POT_FILE} ${TELLICO_SOURCES}
#   )
# ENDIF ( ${XGETTEXT} STREQUAL "XGETTEXT-NOTFOUND" )
