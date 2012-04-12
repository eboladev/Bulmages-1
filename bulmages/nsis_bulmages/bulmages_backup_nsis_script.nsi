
;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  ;Name and file
  Name "Bulmages Backup Tool"
  OutFile "Bulmages_Backup_Tool.exe"

  ;Default installation folder
;  InstallDir "$LOCALAPPDATA\bulmages"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Bulmages" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user

  BrandingText "Bulmages Backup (http://www.bulmages.com)"

  ;Seleccionamos el algoritmo de compresión utilizado para comprimir nuestra aplicación
  SetCompressor lzma

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING
  !define MUI_ICON "backup.ico"
  !define MUI_WELCOMEFINISHPAGE_BITMAP "welcome_bulmages_bitmap.bmp"
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "welcome_bulmages_bitmap.bmp"

;--------------------------------
;

;  !define MUI_FINISHPAGE_SHOWREADME ""
  ;!define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
;  !define MUI_FINISHPAGE_SHOWREADME_TEXT "Crea iconos en el escritorio."
;  !define MUI_FINISHPAGE_SHOWREADME_FUNCTION accionFinal


;
;Pages

  ;Extra space for title.
  ;Extra space for title.
  !define MUI_WELCOMEPAGE_TITLE "Creación de Backups de Bulmages"
  !define MUI_WELCOMEPAGE_TITLE_3LINES
  !define MUI_WELCOMEPAGE_TEXT "Esta utilidad permite crear Backups de los datos"
  
  !define MUI_DIRECTORYPAGE_TEXT_TOP "Seleccione Destino de la Copia"
  !define MUI_DIRECTORYPAGE_TEXT_DESTINATION "Seleccione el directorio donde se realizará la copia de seguridad"

  
  !insertmacro MUI_PAGE_WELCOME
;  !insertmacro MUI_PAGE_LICENSE "licencia_bulmages_3_fact.txt"
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
;  !insertmacro MUI_PAGE_FINISH

  ;Extra space for title.
  !define MUI_WELCOMEPAGE_TITLE_3LINES
;  !define MUI_FINISHPAGE_TITLE_3LINES
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_COMPONENTS
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "Spanish"

;--------------------------------
;Installer Sections


!macro InstallFilesFromBuild Dest Origin
  File "/oname=${Dest}" "C:\bulmages\build${origin}"
!macroend

!macro InstallFilesFromSrc Dest Origin
  File "/oname=${Dest}" "C:\bulmages${origin}"
!macroend

Function .onInit
	Var /GLOBAL ORIGENDIR

        ClearErrors
	; Buscamos el lugar donde se supone esta instalado bulmages
	ReadRegStr $ORIGENDIR HKCU "Software\Bulmages" "BG_InstallDir"
    StrCmp $ORIGENDIR "" 0 NoAbort
	ReadRegStr $ORIGENDIR HKCU "Software\Bulmages" "BG_InstallDir"
    StrCmp $ORIGENDIR "" 0 NoAbort
	ReadRegStr $ORIGENDIR HKCU "Software\Bulmages" "BG_InstallDir"
    StrCmp $ORIGENDIR "" 0 NoAbort
	MessageBox MB_YESNO "No se ha encontrado ninguna instalacion del programa" IDYES 0
	Abort
    NoAbort:

sigue:

FunctionEnd


Section "BulBackup" SecInstall

  Var /GLOBAL FOLDERBACKUP
  StrCpy $FOLDERBACKUP "$INSTDIR/backup"

  DetailPrint 'Activando base de datos...'
;  nsExec::Exec ''"$ORIGENDIR\program\backup_all.bat" $ORIGENDIR $INSTDIR
;  !system  "$ORIGENDIR\program\backup_all.bat"
  
	nsExec::Exec '"$ORIGENDIR\pgsql\bin\pg_ctl.exe" -D "$ORIGENDIR\pgsql\data" -w -l "$ORIGENDIR\pgsql\data\registro" start'
	nsExec::Exec 'mkdir $FOLDERBACKUP'
	nsExec::Exec 'xcopy "$ORIGENDIR\pgsql\data\img_articles" "$FOLDERBACKUP\img_articles" /S /I /Y'
	nsExec::Exec 'xcopy "$ORIGENDIR\openreports" "$FOLDERBACKUP\openreports" /S /I /Y'
	nsExec::Exec '"$ORIGENDIR\pgsql\bin\pg_dumpall.exe" -c  -f "$FOLDERBACKUP\dbbackup.sql"'
	nsExec::Exec '"$ORIGENDIR\pgsql\bin\pg_ctl.exe" -D "$ORIGENDIR\pgsql\data" -w -l "$ORIGENDIR\pgsql\data\registro" stop'

  
  
SectionEnd


