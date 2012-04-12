
;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General
  !define VERSION "2.00"
  
  ;Name and file
  Name "Bulmages"
  OutFile "bulmages-${VERSION}.exe"


  ;Default installation folder
  InstallDir "$LOCALAPPDATA\Bulmages"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Bulmages" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user

  ;Seleccionamos el algoritmo de compresión utilizado para comprimir nuestra aplicación
  SetCompressor lzma
  
  BrandingText "Bulmages [${VERSION}] (http://www.bulmages.com)"


;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING
  !define MUI_ICON "bulmages_install.ico"
  !define MUI_UNICON "bulmages_uninstall.ico"
  !define MUI_WELCOMEFINISHPAGE_BITMAP "welcome_bitmap.bmp"
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "welcome_bitmap.bmp"

;--------------------------------
;

  !define MUI_FINISHPAGE_SHOWREADME ""
  ;!define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
  !define MUI_FINISHPAGE_SHOWREADME_TEXT "Crea iconos en el escritorio."
  !define MUI_FINISHPAGE_SHOWREADME_FUNCTION accionFinal


;
;Pages

  ;Extra space for title.
  !define MUI_WELCOMEPAGE_TITLE_3LINES
  !define MUI_FINISHPAGE_TITLE_3LINES
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "licencia_bulmages.txt"
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  ;Extra space for title.
  !define MUI_WELCOMEPAGE_TITLE_3LINES
  !define MUI_FINISHPAGE_TITLE_3LINES
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
;  !insertmacro MUI_UNPAGE_COMPONENTS
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

!macro InstallFolderFromBuild Dest Origin
  ;File "/oname=${Dest}" "C:\bulmages\build${origin}"
  SetOutPath "$INSTDIR\${Dest}"
  File /r "C:\bulmages\build\${Origin}\*"
!macroend

!macro InstallFolderFromSrc Dest Origin
  ;File "/oname=${Dest}" "C:\bulmages${origin}"
  SetOutPath "$INSTDIR\${Dest}"
  File /r "C:\bulmages\${Origin}\*"

!macroend

Function .onInit
    ClearErrors
	ReadRegStr $0 HKCU "Software\Bulmages" "BG_InstallDir"

        IfErrors 0 +3
        StrCpy $INSTDIR "$PROFILE\Bulmages"
	goto sigue

	StrCpy $INSTDIR $0

sigue:

FunctionEnd

;Include Auxiliar Functions
!include "NSIS_includes_comunes\include_functions.nsh"

;Install Section
!include "NSIS_includes\install_section.nsh"

;UnInstall Section
!include "NSIS_includes\uninstall_section.nsh"




