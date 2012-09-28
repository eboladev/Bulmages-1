
Function accionFinal
  ;Create Icon on Desktop...
  SetOutPath "$INSTDIR\program"
  CreateShortcut "$DESKTOP\Facturacion.lnk" "$INSTDIR\program\bulmafact.vbs" "" "$INSTDIR\program\bulmages_fact_install.ico" 0 SW_SHOWMAXIMIZED
  CreateShortcut "$DESKTOP\Contabilidad.lnk" "$INSTDIR\program\bulmacont.vbs" "" "$INSTDIR\program\bulmages_cont_install.ico" 0 SW_SHOWMAXIMIZED
  CreateShortcut "$DESKTOP\TPV.lnk" "$INSTDIR\program\bulmatpv.vbs" "" "$INSTDIR\program\bulmages_tpv_bar_install.ico" 0 SW_SHOWMAXIMIZED
FunctionEnd


Section "Archivos Comunes" SecInstall
	; Instala comun
	; ----------------------------------------------------------- ;
  SetOutPath "$INSTDIR\etc"
  ;CONFIG FILES...
  ;File /oname=bulmages.conf "etc\bulmages.conf"
  
  ; Copiamos el bulmages.conf directamente desde el source.
  !insertmacro InstallFilesFromSrc "bulmages.conf" "\installbulmages\bulmages.conf.in"
  

  
  FileOpen $4 "$INSTDIR\etc\bulmages.conf" a
  FileSeek $4 0 END
  FileWrite $4 "$\r$\n" ; we write a new line
  FileWrite $4 "CONF_PDF			@CMAKE_INSTALL_PREFIX@\Evince\bin\evince.exe $\n"
  FileWrite $4 "CONF_FLIP			@CMAKE_INSTALL_PREFIX@\share\bulmages\bgtrml2pdf\flip.exe$\n"
  FileWrite $4 "CONF_SPOOL			@CMAKE_INSTALL_PREFIX@\program\spool.exe $\n"
  FileWrite $4 "CONF_PYTHON			@CMAKE_INSTALL_PREFIX@\Python26\python $\n"
  FileWrite $4 "CONF_PROGDATA		@CMAKE_INSTALL_PREFIX@\share\bulmages\  $\n"
  FileWrite $4 "$\r$\n" ; we write an extra line
  FileClose $4 ; and close the file
  
  ;Replace text from '.conf' files
  ;First bulmages.conf
  Push "@CMAKE_INSTALL_PREFIX@" #text to be replaced
  Push "$INSTDIR" #replace with
  Push all #replace all occurrences
  Push all #replace all occurrences
  Push "$INSTDIR\etc\bulmages.conf" #file to replace in
  Call AdvReplaceInFile
  
;  SetOutPath "$INSTDIR"
; Instalamos el bgtrml2pdf
  SetOutPath "$INSTDIR\share\bulmages\bgtrml2pdf"
  File /oname=flip.exe "flip.exe"    
  !insertmacro InstallFilesFromSrc bgtrml2pdf "\bin\bgtrml2pdf"
  

  
  ;Copiamos todo lo referente a bulmasetup
  ;SetOutPath "$INSTDIR\bulmasetup"
  SetOutPath "$INSTDIR"
  !insertmacro InstallFolderFromSrc bulmasetup "\bulmasetup"
  !insertmacro InstallFolderFromBuild bulmasetup "\bulmasetup"
  
  SetOutPath "$INSTDIR\program"
;  File Bulmages_Backup_Tool.exe
;  File Bulmages_Restore_Tool.exe
;  File Bulmages_CreateCompany_Tool.exe
;  File backup.ico
;  File restore.ico


  !insertmacro InstallFilesFromBuild libbulmalib.dll "\bulmalib\src\libbulmalib.dll"

  !include "NSIS_includes_comunes\qt_fijo\include_qt_fijo_install.nsh"
  !include "NSIS_includes_comunes\postgresql_fijo\include_postgresql_fijo_install.nsh"

  ;Install translation system
  !include "NSIS_includes_comunes\gettext_fijo\include_gettext_install.nsh"


  !include "NSIS_includes_comunes\python\include_python_install.nsh"
  !include "NSIS_includes_comunes\postgresql\include_postgresql_install.nsh"
  !include "NSIS_includes_comunes\evince\include_evince_install.nsh"

  ;Store installation folder
  WriteRegStr HKCU "Software\Bulmages" "BG_InstallDir" $INSTDIR
  WriteRegStr HKCU "Software\Bulmages" "BG_VersionProgram" "${VERSION}"
  WriteRegStr HKCU "Software\Bulmages" "BG_VersionDb" "${VERSION}"
  
  
;  SetOutPath "$INSTDIR\openreports"
;  File openreports\*

  SetOutPath "$INSTDIR"
  !insertmacro InstallFolderFromSrc share "\share"
  
  ;Copia librerias especiales y vc++.
  SetOutPath "$INSTDIR\program"
  File /oname=libeay32.dll "NSIS_includes_comunes\postgresql\modified_version_libs\libeay32.dll"
  File /oname=ssleay32.dll "NSIS_includes_comunes\postgresql\modified_version_libs\ssleay32.dll"
  File /oname=Microsoft.VC80.CRT.manifest "NSIS_includes_comunes\vc++\Microsoft.VC80.CRT.manifest"
  File /oname=msvcm80.dll "NSIS_includes_comunes\vc++\msvcm80.dll"
  File /oname=msvcp80.dll "NSIS_includes_comunes\vc++\msvcp80.dll"
  File /oname=msvcr80.dll "NSIS_includes_comunes\vc++\msvcr80.dll"
  File /oname=msvcr71.dll "NSIS_includes_comunes\vc++\msvcr71.dll"

  ; Create file bulmafact.bat
  SetOutPath "$INSTDIR\bulmasetup"
  !include "NSIS_includes\bulmasetup.bat.tpt"
  File /oname=bulmasetup.vbs "bulmasetup.vbs"
  
  CreateDirectory "$SMPROGRAMS\Bulmages\Herramientas"
  CreateShortCut "$SMPROGRAMS\Bulmages\Herramientas\pgAdmin3.lnk" "$INSTDIR\pgsql\bin\pgAdmin3.vbs" "" "$INSTDIR\pgsql\bin\pgsql.ico" 0 SW_SHOWNORMAL
;  CreateShortCut "$SMPROGRAMS\Bulmages\Tools\BG_Backup.lnk" "$INSTDIR\program\Bulmages_Backup_Tool.exe" "" "$INSTDIR\program\backup.ico" 0 SW_SHOWNORMAL
;  CreateShortCut "$SMPROGRAMS\Bulmages\Tools\BG_Restore.lnk" "$INSTDIR\program\Bulmages_Restore_Tool.exe" "" "$INSTDIR\program\restore.ico" 0 SW_SHOWNORMAL
  CreateShortCut "$SMPROGRAMS\Bulmages\Herramientas\Desinstalar.lnk" "$INSTDIR\desinstalar.exe" "" "" 0
;  CreateShortCut "$SMPROGRAMS\Bulmages\Tools\BG_Nueva_Empresa.lnk" "$INSTDIR\program\Bulmages_CreateCompany_Tool.exe" "" "$INSTDIR\program\nuevaemp.ico" 0 SW_SHOWNORMAL
  CreateShortCut "$SMPROGRAMS\Bulmages\BulmaSetup.lnk" "$INSTDIR\bulmasetup\bulmasetup.vbs" "" "$INSTDIR\program\nuevaemp.ico" 0 SW_SHOWNORMAL
  
  ;Initialize PostgreSQL
  DetailPrint 'Iniciando base de datos...'
  nsExec::Exec '"$INSTDIR\pgsql\bin\initdb.exe" -D "$INSTDIR\pgsql\data"'

  DetailPrint 'Activando base de datos...'
  nsExec::Exec '"$INSTDIR\pgsql\bin\pg_ctl.exe" -D "$INSTDIR\pgsql\data" -w -l "$INSTDIR\pgsql\data\registro" start'

  ;Create user and demo database
  DetailPrint 'Creando usuario postgres...'
  nsExec::Exec '"$INSTDIR\pgsql\bin\createuser.exe" -s postgres'
  nsExec::Exec '"$INSTDIR\pgsql\bin\createuser.exe" -s root'
  nsExec::Exec '"$INSTDIR\pgsql\bin\psql.exe" -C "ALTER USER root WITH PASSWORD $\'password$\'" template1'

  DetailPrint 'Desactivando base de datos...'
  nsExec::Exec '"$INSTDIR\pgsql\bin\pg_ctl.exe" -D "$INSTDIR\pgsql\data" -w -l "$INSTDIR\pgsql\data\registro" stop'

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\desinstalar.exe"

SectionEnd


; ----------------------------------------------------------- ;
; Instala especifico fact
; ----------------------------------------------------------- ;
Section "BulmaFact" 

  ;SetOutPath "$INSTDIR\css"
  ;CSS (estilos)
  ;File /oname=bulmafact_es_ES.css "css\bulmafact_es_ES.css"


  SetOutPath "$INSTDIR\program"
  File nuevaemp.ico
  File /oname=bulmages_fact_install.ico "bulmages_fact_install.ico"

  SetOutPath "$INSTDIR\etc"
  ;CONFIG FILES...
  ; Copiamos el bulmafact.conf directamente desde el source.
  !insertmacro InstallFilesFromSrc "bulmafact.conf" "\installbulmages\bulmafact.conf.in"
  
  ;Replace text from '.conf' files
  ;First bulmages.conf
  Push "@CMAKE_INSTALL_PREFIX@" #text to be replaced
  Push "$INSTDIR" #replace with
  Push all #replace all occurrences
  Push all #replace all occurrences
  Push "$INSTDIR\etc\bulmafact.conf" #file to replace in
  Call AdvReplaceInFile

  SetOutPath "$INSTDIR\program"
  !insertmacro InstallFilesFromBuild bulmafact.exe "\bulmafact\src\bulmafact.exe"
  !insertmacro InstallFilesFromBuild libbulmafact.dll "\bulmafact\src\libbulmafact.dll"



  File /oname=splashbulmafact.png "NSIS_includes\splashbulmafact.png"
  File /oname=bulmafact-background.png "NSIS_includes\bulmafact-background.png"


  ; Create file bulmafact.bat
  !include "NSIS_includes\bulmafact.bat.tpt"
  File /oname=bulmafact.vbs "bulmafact.vbs"


  !include "NSIS_includes\include_bulmafact_plugins_install.nsh"
  !include "NSIS_includes\include_bulmafact_es_ES_install.nsh"

  ;Shortcuts
  CreateDirectory "$SMPROGRAMS\Bulmages"
  SetOutPath "$INSTDIR\program"
  CreateShortCut "$SMPROGRAMS\Bulmages\Facturacion.lnk" "$INSTDIR\program\bulmafact.vbs" "" "$INSTDIR\program\Bulmages_fact_install.ico" 0 SW_SHOWMAXIMIZED

  
SectionEnd

; ----------------------------------------------------------- ;

; Instala especifico cont
; ----------------------------------------------------------- ;
Section "BulmaCont" 

  SetOutPath "$INSTDIR\etc"
  ;CONFIG FILES...
  
    SetOutPath "$INSTDIR\etc"
  ;CONFIG FILES...
  ; Copiamos el bulmafact.conf directamente desde el source.
  !insertmacro InstallFilesFromSrc "bulmacont.conf" "\installbulmages\bulmacont.conf.in"
  
  ;Replace text from '.conf' files
  ;First bulmages.conf
  Push "@CMAKE_INSTALL_PREFIX@" #text to be replaced
  Push "$INSTDIR" #replace with
  Push all #replace all occurrences
  Push all #replace all occurrences
  Push "$INSTDIR\etc\bulmacont.conf" #file to replace in
  Call AdvReplaceInFile
  
 
  SetOutPath "$INSTDIR\program"
  !insertmacro InstallFilesFromBuild bulmacont.exe "\bulmacont\src\bulmacont.exe"
  !insertmacro InstallFilesFromBuild libbulmacont.dll "\bulmacont\src\libbulmacont.dll"

  File /oname=splashbulmacont.png "NSIS_includes\splashbulmacont.png"
  File /oname=bulmacont-background.png "NSIS_includes\bulmacont-background.png"

  ; Create file bulmacont.bat
  !include "NSIS_includes\bulmacont.bat.tpt"
  File /oname=bulmacont.vbs "bulmacont.vbs"
  File Bulmages_cont_install.ico

  !include "NSIS_includes\include_bulmacont_plugins_install.nsh"
  !include "NSIS_includes\include_bulmacont_es_ES_install.nsh"



  ;Shortcuts
  CreateDirectory "$SMPROGRAMS\Bulmages"
  SetOutPath "$INSTDIR\program"
  CreateShortCut "$SMPROGRAMS\Bulmages\Contabilidad.lnk" "$INSTDIR\program\bulmacont.vbs" "" "$INSTDIR\program\Bulmages_cont_install.ico" 0 SW_SHOWMAXIMIZED


SectionEnd



Section "BulmaTPV" 
  SetOutPath "$INSTDIR\etc"

    ;CONFIG FILES...
  ; Copiamos el bulmafact.conf directamente desde el source.
  !insertmacro InstallFilesFromSrc "bulmatpv.conf" "\installbulmages\bulmatpv.conf.in"
  
  ;Replace text from '.conf' files
  ;First bulmages.conf
  Push "@CMAKE_INSTALL_PREFIX@" #text to be replaced
  Push "$INSTDIR" #replace with
  Push all #replace all occurrences
  Push all #replace all occurrences
  Push "$INSTDIR\etc\bulmatpv.conf" #file to replace in
  Call AdvReplaceInFile
  

  SetOutPath "$INSTDIR\program"
  !insertmacro InstallFilesFromBuild bulmatpv.exe "\bulmatpv\src\bulmatpv.exe"
  !insertmacro InstallFilesFromBuild libbulmatpv.dll "\bulmatpv\src\libbulmatpv.dll"
  
  File /oname=splashbulmatpv.png "NSIS_includes\splashbulmatpv.png"
  File /oname=bulmatpv-background.png "NSIS_includes\bulmatpv-background.png"
  
  File Bulmages_tpv_bar_install.ico
  
  !include "NSIS_includes\bulmatpv.bat.tpt"
  File /oname=bulmatpv.vbs "bulmatpv.vbs"

  
  ; Modulos de bulmaTPV
  !include "NSIS_includes\include_bulmatpv_plugins_install.nsh"
  !include "NSIS_includes\include_bulmatpv_es_ES_install.nsh"

  
  SetOutPath "$INSTDIR\program"
  
  ;Shortcuts
  CreateDirectory "$SMPROGRAMS\Bulmages"
  CreateShortCut "$SMPROGRAMS\Bulmages\TPV .lnk" "$INSTDIR\program\bulmatpv.vbs" "" "$INSTDIR\program\Bulmages_tpv_bar_install.ico" 0 SW_SHOWMAXIMIZED


  SetOutPath "$INSTDIR"


  
SectionEnd