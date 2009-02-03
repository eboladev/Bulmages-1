# - Find GNU gettext tools
# This module looks for the GNU gettext tools. This module defines the 
# following values:

#
#  GETTEXT_FOUND - system has Gettext
#  GETTEXT_INCLUDE_DIR - Gettext include directory
#  GETTEXT_LIBRARIES - Libraries needed to use Gettext
#  GETTEXT_MSGMERGE_EXECUTABLE: the full path to the msgmerge tool.
#  GETTEXT_MSGFMT_EXECUTABLE: the full path to the msgfmt tool.

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

#
# GETTEXT_CREATE_TEMPLATE( template dirOUT dirIN sources)
# this macro creates the pot file template. It needs: 
# template, the file output with the name template.pot
# dirOUT, the directory to save the template.pot
# dirIN, the directory where to find the files to process.
# sources, a file with the list of the files to process 
# it's possible to define  MSGID_BUGS_ADDRESS

# GETTEXT_CREATE_TRANSLATIONS potFile INSTALLDIR langs)
#
# potFile with a potfile 
#
# INSTALLDIR where to install the translations
# in this way ${INSTALLDIR}/${_lang} RENAME ${_potBasename}.mo)
# if is empty install it on:
# share/locale/${_lang}/LC_MESSAGES RENAME  ${_potBasename}.mo)
#
# langs a list of languages to process
# it checks for 
# ${CMAKE_CURRENT_SOURCE_DIR}/${_potBasename}_${_lang})
# if the file potFile_lang doesn't exist create it in



#Code

#First of all we know if we have gettext
if (LIBC_HAS_DGETTEXT OR LIBINTL_HAS_DGETTEXT)
  # in cache already
  set(GETTEXT_FOUND TRUE)

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


#Finder of msgmerge
FIND_PROGRAM(GETTEXT_MSGMERGE_EXECUTABLE msgmerge)
#Finder of msgfmt)
FIND_PROGRAM(GETTEXT_MSGFMT_EXECUTABLE msgfmt)
#Finder of xgettext)
FIND_PROGRAM(GETTEXT_XGETTEXT_EXECUTABLE xgettext)

IF (GETTEXT_MSGMERGE_EXECUTABLE AND GETTEXT_MSGFMT_EXECUTABLE )
   SET(GETTEXT_FOUND TRUE)
ELSE (GETTEXT_MSGMERGE_EXECUTABLE AND GETTEXT_MSGFMT_EXECUTABLE )
   SET(GETTEXT_FOUND FALSE)
   IF (GetText_REQUIRED)
      MESSAGE(FATAL_ERROR "GetText not found")
   ENDIF (GetText_REQUIRED)
ENDIF (GETTEXT_MSGMERGE_EXECUTABLE AND GETTEXT_MSGFMT_EXECUTABLE )



# This are the two main targets
# they are empty. The macros fill them with dependencies
add_custom_target( messages_extract )
add_custom_target( translations )
add_custom_target( update_pots DEPENDS translations)

#To acomplish the messages extract it's needed some macros

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


# GETTEXT_CREATE_TEMPLATE( template dirOUT dirIN sources)
# this macro creates the pot file template. It needs: 
# template, the file output with the name template.pot
# dirOUT, the directory to save the template.pot
# dirIN, the directory where to find the files to process.
# sources, a file with the list of the files to process 
# it's possible to define  MSGID_BUGS_ADDRESS

macro(GETTEXT_CREATE_TEMPLATE template dirOUT dirIN sources )
   
   set (MSGID_BUGS_ADDRESS "info@iglues.org")
#  file(REMOVE ${CMAKE_CURRENT_SOURCE_DIR}/${template}.pot)
   #message (STATUS "Files to create list ${${sources}}")

   #we need a list with the files
   CREATE_FILE_LIST(${sources} ${CMAKE_CURRENT_BINARY_DIR}/${template}.pot_list)
   
   add_custom_command( 
   OUTPUT ${dirOUT}/${template}.pot
   COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE} 
   ARGS --add-comments=TRANSLATORS: 
   ARGS --qt
#   ARGS --keyword=tr --flag=tr:1:pass-c-format --flag=tr:1:pass-qt-format
   ARGS --keyword=i18n --flag=tr:1:pass-c-format --flag=tr:1:pass-qt-format
   ARGS --keyword=translate:2 --flag=translate:2:pass-c-format --flag=translate:2:pass-qt-format
   ARGS --keyword=QT_TR_NOOP --flag=QT_TR_NOOP:1:pass-c-format --flag=QT_TR_NOOP:1:pass-qt-format
   ARGS --keyword=QT_TRANSLATE_NOOP:2 --flag=QT_TRANSLATE_NOOP:2:pass-c-format --flag=QT_TRANSLATE_NOOP:2:pass-qt-format
   ARGS --keyword=_ --flag=_:1:pass-c-format --flag=_:1:pass-qt-format
   ARGS --keyword=N_ --flag=N_:1:pass-c-format --flag=N_:1:pass-qt-format
   ARGS --from-code=utf-8
   ARGS --directory=${dirIN} --directory=${CMAKE_CURRENT_BINARY_DIR} 
   ARGS --output=${dirOUT}/${template}.pot
   ARGS --msgid-bugs-address=${MSGID_BUGS_ADDRESS} --files-from=${CMAKE_CURRENT_BINARY_DIR}/${template}.pot_list
   DEPENDS ${template} VERBATIM)
           
   add_custom_target(${template}.pot DEPENDS ${dirOUT}/${template}.pot)
   add_dependencies(${template}.pot  ${template})
   add_dependencies(messages_extract ${template}.pot)

   GETTEXT_UPDATE_POT(${dirOUT}/${template}.pot ${CMAKE_CURRENT_SOURCE_DIR}/po)

endmacro( GETTEXT_CREATE_TEMPLATE)


macro (GETTEXT_UPDATE_POT inputPot dirOUT)
   get_filename_component(_PotFile ${inputPot} NAME)
   add_custom_command(
   OUTPUT ${dirOUT}/${_PotFile}
   COMMAND ${CMAKE_COMMAND} copy ${inputPot} ${dirOUT}/${_PotFile}
   DEPENDS ${inputPot}
   )
   add_dependencies(update_pots ${inputPot})
endmacro (GETTEXT_UPDATE_POT inputPot dirOUT)





# GETTEXT_CREATE_TRANSLATIONS potFile INSTALLDIR langs)
#
# potFile with a potfile 
#
# INSTALLDIR where to install the translations
# in this way ${INSTALLDIR}/${_lang} RENAME ${_potBasename}.mo)
# if is empty install it on:
# share/locale/${_lang}/LC_MESSAGES RENAME  ${_potBasename}.mo)
#
# langs a list of languages to process
# it checks for 
# ${CMAKE_CURRENT_SOURCE_DIR}/${_potBasename}_${_lang})
# if the file potFile_lang doesn't exist create it in

macro(GETTEXT_CREATE_TRANSLATIONS potFile INSTALLDIR langs)

   set(_gmoFiles)
   get_filename_component(_potBasename ${potFile} NAME_WE)
   get_filename_component(_absPotFile ${potFile} ABSOLUTE)
   get_filename_component(_PotFile ${potFile} NAME)


#   MESSAGE(STATUS "Programmed translations")

   foreach(_lang ${ARGN})

      # Copy _lang.po file to binary directory
#      CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/po/${_potBasename}_${_lang}.po ${CMAKE_CURRENT_BINARY_DIR}/${_potBasename}_${_lang}.po COPYONLY)

      set(_absPoFile ${CMAKE_CURRENT_BINARY_DIR}/po/${_potBasename}_${_lang}.po)
      set(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_potBasename}_${_lang}.gmo)

      get_filename_component(_gmoBasename ${_gmoFile} NAME)

      if(NOT EXISTS ${_absPoFile})
         configure_file( ${_absPotFile} ${_absPoFile} COPYONLY)
         file(WRITE ${_absPoFile} "")
      endif(NOT EXISTS ${_absPoFile})

      add_custom_command( 
         OUTPUT  "${_gmoFile}"
         COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} --quiet --update --backup=none -s ${_absPoFile} ${_absPotFile}
         COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${_gmoFile} ${_absPoFile}
         DEPENDS ${_absPotFile} ${_absPoFile}
      )
      if(INSTALLDIR)
         install (FILES ${_gmoFile} DESTINATION ${INSTALLDIR}/${_lang}/ RENAME ${_potBasename}.mo) 
      else(INSTALLDIR)
         install (FILES ${_gmoFile} DESTINATION share/locale/${_lang}/LC_MESSAGES RENAME ${_potBasename}.mo) 
      endif(INSTALLDIR)

      list (APPEND _gmoFiles ${_gmoFile})

   endforeach (_lang ${${langs}})

   add_custom_target( ${_potBasename}_pos DEPENDS ${_absPotFile} ${_gmoFiles} )
   add_dependencies(translations ${_potBasename}_pos  ${potFile})

endmacro(GETTEXT_CREATE_TRANSLATIONS )
