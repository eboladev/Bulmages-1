
;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"
  !include InstallOptions.nsh

;--------------------------------
;General

  ;Name and file
  Name "Bulmages Database Update  Tool"
  OutFile "Bulmages_DatabaseUpdate_Tool.exe"

  ;Default installation folder

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Bulmages" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user

  BrandingText "Bulmages Database Update Tool (http://www.bulmages.com)"

  ;Seleccionamos el algoritmo de compresión utilizado para comprimir nuestra aplicación
  SetCompressor lzma

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING
  !define MUI_ICON "restore.ico"
  !define MUI_WELCOMEFINISHPAGE_BITMAP "welcome_bulmages_bitmap.bmp"
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "welcome_bulmages_bitmap.bmp"

;--------------------------------
;

;
;Pages

  ;Extra space for title.
  !define MUI_WELCOMEPAGE_TITLE "Actualizar Bases de Datos para Bulmages"
  !define MUI_WELCOMEPAGE_TITLE_3LINES
  !define MUI_WELCOMEPAGE_TEXT "Permite actualizar las empresas existentes"
  

  !define MUI_FINISHPAGE_TITLE_3LINES
  

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_INSTFILES

  ;Extra space for title.
  !define MUI_WELCOMEPAGE_TITLE_3LINES
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "Spanish"

;--------------------------------
;Installer Sections

;Include Auxiliar Functions
!include "..\NSIS_includes_comunes\include_functions.nsh"

!define StrLoc "!insertmacro StrLoc"
 
 !macro SPLIT_STRING INPUT PART
Push $R0
Push $R1
 
 StrCpy $R0 0
 StrCmp ${PART} 1 getpart1_loop_${PART}
 StrCmp ${PART} 2 getpart2_top_${PART}
Goto error_${PART}
 
getpart1_loop_${PART}:
 IntOp $R0 $R0 + 1
 StrCpy $R1 ${INPUT} 1 $R0
  StrCmp $R1 "" error1_${PART}
  StrCmp $R1 "$\n" 0 getpart1_loop_${PART}
 
 IntOp $R0 $R0 + 1
 StrCpy $R0 ${INPUT} "" $R0
Goto done_${PART}
 
getpart2_top_${PART}:
 StrLen $R7 ${INPUT}
; StrLen $R0 ${INPUT}
 StrCpy $R0 0
getpart2_loop_${PART}:
 IntOp $R0 $R0 + 1
 StrCpy $R1 ${INPUT} 1 $R0
  StrCmp $R1 "" error1_${PART}
  StrCmp $R1 "$\n" 0 getpart2_loop_${PART}
 
 
 IntOp $R4 $R0 - 1
 StrCpy $R6 ${INPUT} $R4 0
 IntOp $R5 $R7 - $R0
 IntOp $R5 $R5 - 1
 IntOp $R4 $R4 + 2 
 StrCpy $R2 ${INPUT} $R5 $R4 
; MessageBox MB_OK "$R7 $R5 $R0 -$R2- ---  -$R6-"
Goto done_${PART}
 
error_${PART}:
 StrCpy $R0 "error parte inexistente"

error1_${PART}:
 StrCpy $R0 "Comparacion de cadena vacia"

 
done_${PART}:
 
Pop $R1
Pop $R1
;Exch $R0
Push $R6
Push $R2
!macroend
 
 

 
 
!macro StrLoc ResultVar String SubString StartPoint
  Push "${String}"
  Push "${SubString}"
  Push "${StartPoint}"
  Call StrLoc
  Pop "${ResultVar}"
!macroend
 
Function StrLoc
/*After this point:
  ------------------------------------------
   $R0 = StartPoint (input)
   $R1 = SubString (input)
   $R2 = String (input)
   $R3 = SubStringLen (temp)
   $R4 = StrLen (temp)
   $R5 = StartCharPos (temp)
   $R6 = TempStr (temp)*/
 
  ;Get input from user
  Exch $R0
  Exch
  Exch $R1
  Exch 2
  Exch $R2
  Push $R3
  Push $R4
  Push $R5
  Push $R6
 
  ;Get "String" and "SubString" length
  StrLen $R3 $R1
  StrLen $R4 $R2
  ;Start "StartCharPos" counter
  StrCpy $R5 0
 
  ;Loop until "SubString" is found or "String" reaches its end
  ${Do}
    ;Remove everything before and after the searched part ("TempStr")
    StrCpy $R6 $R2 $R3 $R5
 
    ;Compare "TempStr" with "SubString"
    ${If} $R6 == $R1
      ${If} $R0 == `<`
        IntOp $R6 $R3 + $R5
        IntOp $R0 $R4 - $R6
      ${Else}
        StrCpy $R0 $R5
      ${EndIf}
      ${ExitDo}
    ${EndIf}
    ;If not "SubString", this could be "String"'s end
    ${If} $R5 >= $R4
      StrCpy $R0 ``
      ${ExitDo}
    ${EndIf}
    ;If not, continue the loop
    IntOp $R5 $R5 + 1
  ${Loop}
 
  ;Return output to user
  Pop $R6
  Pop $R5
  Pop $R4
  Pop $R3
  Pop $R2
  Exch
  Pop $R1
  Exch $R0
FunctionEnd



!macro InstallFilesFromBuild Dest Origin
  File "/oname=${Dest}" "C:\bulmages\build${origin}"
!macroend

!macro InstallFilesFromSrc Dest Origin
  File "/oname=${Dest}" "C:\bulmages${origin}"
!macroend





Function .onInit
	
	!insertmacro INSTALLOPTIONS_EXTRACT "custom.ini"
   
	Var /GLOBAL ORIGENDIR

        ClearErrors
	; Buscamos el lugar donde se supone esta instalado bulmageswin
	ReadRegStr $ORIGENDIR HKCU "Software\Bulmages" "BG_InstallDir"
    StrCmp $ORIGENDIR "" 0 NoAbort
	ReadRegStr $ORIGENDIR HKCU "Software\Bulmages" "BG_InstallDir"
    StrCmp $ORIGENDIR "" 0 NoAbort
	ReadRegStr $ORIGENDIR HKCU "Software\Bulmages" "BG_InstallDir"
    StrCmp $ORIGENDIR "" 0 NoAbort
	MessageBox MB_OK "No se ha encontrado ninguna instalacion del programa" 
	Abort
    NoAbort:

FunctionEnd


Section "BulBackup" SecInstall

  Var /GLOBAL FOLDERBACKUP
  StrCpy $FOLDERBACKUP "$INSTDIR"

  DetailPrint 'Activando base de datos...'
  
  DetailPrint 'Activando base de datos...'
  nsExec::Exec '"$ORIGENDIR\pgsql\bin\pg_ctl.exe" -D "$ORIGENDIR\pgsql\data" -w -l "$ORIGENDIR\pgsql\data\registro" start'

  nsExec::ExecToStack '"$ORIGENDIR\pgsql\bin\psql.exe" -A -q -t -c "SELECT datname FROM pg_database WHERE datistemplate IS FALSE AND datallowconn IS TRUE" template1'
  Pop $0
  Pop $1

  StrCpy $4 $1
top:
  !insertmacro SPLIT_STRING $4 2
  Pop $4
  Pop $7
;  MessageBox MB_OK "Primera parte: -$4-"
;  MessageBox MB_OK "Tratando Base de Datos: -$7-"
  
  nsExec::ExecToLog '"$ORIGENDIR\pgsql\bin\psql.exe" -f "$ORIGENDIR\share\bulmages\dbmodels\actualizar\revf-0.13.sql" -d $7'

  nsExec::ExecToLog '"$ORIGENDIR\pgsql\bin\psql.exe" -f "$ORIGENDIR\share\bulmages\dbmodels\actualizar\rev-0.13.sql" -d $7'

  
  StrCmp $4 "" 0 top
  
  
  DetailPrint 'Desactivando base de datos...'
  nsExec::Exec '"$ORIGENDIR\pgsql\bin\pg_ctl.exe" -D "$ORIGENDIR\pgsql\data" -w -l "$ORIGENDIR\pgsql\data\registro" stop'
  

  

  
SectionEnd



