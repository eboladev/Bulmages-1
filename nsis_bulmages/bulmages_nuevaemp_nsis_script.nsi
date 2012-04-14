
;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"
  !include InstallOptions.nsh

;--------------------------------
;General

  ;Name and file
  Name "Bulmages Create Company  Tool"
  OutFile "Bulmages_CreateCompany_Tool.exe"

  ;Default installation folder

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Bulmages" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user

  BrandingText "Bulmages Create Company  Tool (http://www.bulmages.com)"

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
  !define MUI_WELCOMEPAGE_TITLE "Crear empresa nueva de para Bulmages"
  !define MUI_WELCOMEPAGE_TITLE_3LINES
  !define MUI_WELCOMEPAGE_TEXT "Permite crear empresas nuevas"
  
  !define MUI_DIRECTORYPAGE_TEXT_TOP "Seleccione copia a restaurar"
  !define MUI_DIRECTORYPAGE_TEXT_DESTINATION "Seleccione el directorio donde se encuentra la copia de seguridad"

  !define MUI_FINISHPAGE_TITLE_3LINES
  

  !insertmacro MUI_PAGE_WELCOME
  page Custom CustomCreate CustomCreateProg
  !insertmacro MUI_PAGE_INSTFILES

  ;Extra space for title.
  !define MUI_WELCOMEPAGE_TITLE_3LINES
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "Spanish"

;--------------------------------
;Installer Sections

;Include Auxiliar Functions
!include "..\NSIS_includes_comunes\include_functions.nsh"

!define StrLoc "!insertmacro StrLoc"
 
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


Function CustomCreate
  !insertmacro MUI_HEADER_TEXT "Nueva Empresa de Facturación" "Introduzca el nombre para la Base de Datos de la nueva empresa. (Sin espacios)"
  !insertmacro INSTALLOPTIONS_DISPLAY "custom.ini"
FunctionEnd 



Function CustomCreateProg
  Var /GLOBAL EMPNAME
  Var /GLOBAL EMPTYPE
  Var /GLOBAL LOGOFILE
  
  !insertmacro INSTALLOPTIONS_READ $R0 'custom.ini' "Field 1" "HWND"
  # Check if text has been entered in field 1.
  !insertmacro INSTALLOPTIONS_READ $R1 'custom.ini' "Field 1" "State"
  StrCpy $EMPNAME "$R1"
   
  # El archivo del logotipo
  !insertmacro INSTALLOPTIONS_READ $R7 'custom.ini' "Field 7" "State"
  StrCpy $LOGOFILE "$R7"
  
  
  ${StrLoc} $R2 $R1 " " ">"
  ${If} $R2 != ""
		MessageBox MB_OK "El nombre de la empresa no puede contener espacios"
		Abort
  ${EndIf}
		
  !insertmacro INSTALLOPTIONS_READ $R4 'custom.ini' "Field 4" "State"
  !insertmacro INSTALLOPTIONS_READ $R5 'custom.ini' "Field 5" "State"
  !insertmacro INSTALLOPTIONS_READ $R6 'custom.ini' "Field 6" "State"

  ${If} $R4 == "1"
		StrCpy $EMPTYPE "template1"
  ${EndIf}

  ${If} $R5 == "1"
		StrCpy $EMPTYPE "template2"
  ${EndIf}

  ${If} $R6 == "1"
		StrCpy $EMPTYPE "template3"
  ${EndIf}
  
FunctionEnd

Function .onInit
	
	!insertmacro INSTALLOPTIONS_EXTRACT "custom.ini"
   
	Var /GLOBAL ORIGENDIR

        ClearErrors
	; Buscamos el lugar donde se supone esta instalado bulmages
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

  DetailPrint 'Creando base de datos de la empresa...'
  nsExec::Exec '"$ORIGENDIR\pgsql\bin\createdb.exe" $EMPNAME'

  ;Load data to demo database
  DetailPrint 'Cargando la base de datos con datos...'

  nsExec::Exec '"$ORIGENDIR\pgsql\bin\psql.exe" -f "$ORIGENDIR\db\$EMPTYPE.sql" -d $EMPNAME'

  
  ;Aplicamos los parches de actualizacion
  ${If} $EMPTYPE != "template4"
	  nsExec::Exec '"$ORIGENDIR\pgsql\bin\psql.exe" -f "$ORIGENDIR\share\bulmages\dbmodels\actualizar\revf-0.13.sql" -d $EMPNAME'
  ${Else}
	  nsExec::Exec '"$ORIGENDIR\pgsql\bin\psql.exe" -f "$ORIGENDIR\share\bulmages\dbmodels\actualizar\rev-0.13.sql" -d $EMPNAME'
  ${EndIf}
  
  
  DetailPrint 'Desactivando base de datos...'
  nsExec::Exec '"$ORIGENDIR\pgsql\bin\pg_ctl.exe" -D "$ORIGENDIR\pgsql\data" -w -l "$ORIGENDIR\pgsql\data\registro" stop'
  
  ${If} $EMPTYPE != "template4"
	  ;CONFIG FILES...
	  ; Creamos el archivo de configuracion
	  CopyFiles "$ORIGENDIR\etc\bulmafact_$EMPTYPE.conf" "$ORIGENDIR\etc\bulmafact_$EMPNAME.conf"
	  CopyFiles "$ORIGENDIR\openreports" "$ORIGENDIR\openreports_$EMPNAME"
	  CreateDirectory "$ORIGENDIR\pgsql\data\img_articles_$EMPNAME"
	  CreateDirectory "$ORIGENDIR\pgsql\data\img_personal_$EMPNAME"

	  ; Ponemos el logotipo seleccionado en su sitio.
	  ${If} $LOGOFILE != ""
		CopyFiles "$LOGOFILE" "$ORIGENDIR\openreports_$EMPNAME\logo.jpg"
	  ${EndIf}
	  
	  
	  Push "[[EMPNAME]]" #text to be replaced
	  Push "$EMPNAME" #replace with
	  Push all #replace all occurrences
	  Push all #replace all occurrences
	  Push "$ORIGENDIR\etc\bulmafact_$EMPNAME.conf" #file to replace in
	  Call AdvReplaceInFile
	  
	  ;Replace text from '.conf' files
	  ;First bulmacont.conf
	  Push "[[INSTDIR]]" #text to be replaced
	  Push "$ORIGENDIR" #replace with
	  Push all #replace all occurrences
	  Push all #replace all occurrences
	  Push "$ORIGENDIR\etc\bulmafact_$EMPNAME.conf" #file to replace in
	  Call AdvReplaceInFile
	  
	  
	  ; Tambien tratamos el tema del TPV
	  ;CONFIG FILES...
	  ; Creamos el archivo de configuracion
	  CopyFiles "$ORIGENDIR\etc\bulmatpv_$EMPTYPE.conf" "$ORIGENDIR\etc\bulmatpv_$EMPNAME.conf"

		  
	  Push "[[EMPNAME]]" #text to be replaced
	  Push "$EMPNAME" #replace with
	  Push all #replace all occurrences
	  Push all #replace all occurrences
	  Push "$ORIGENDIR\etc\bulmatpv_$EMPNAME.conf" #file to replace in
	  Call AdvReplaceInFile
	  
	  ;Replace text from '.conf' files
	  ;First bulmacont.conf
	  Push "[[INSTDIR]]" #text to be replaced
	  Push "$ORIGENDIR" #replace with
	  Push all #replace all occurrences
	  Push all #replace all occurrences
	  Push "$ORIGENDIR\etc\bulmatpv_$EMPNAME.conf" #file to replace in
	  Call AdvReplaceInFile
	  
   ${Else}
	  ;CONFIG FILES...
	  ; Creamos el archivo de configuracion
	  CopyFiles "$ORIGENDIR\etc\bulmacont_$EMPTYPE.conf" "$ORIGENDIR\etc\bulmacont_$EMPNAME.conf"
	  CopyFiles "$ORIGENDIR\openreports" "$ORIGENDIR\openreports_$EMPNAME"

	  ; Ponemos el logotipo seleccionado en su sitio.
	  ${If} $LOGOFILE != ""
		CopyFiles "$LOGOFILE" "$ORIGENDIR\openreports_$EMPNAME\logo.jpg"
	  ${EndIf}
	  
	  
	  Push "[[EMPNAME]]" #text to be replaced
	  Push "$EMPNAME" #replace with
	  Push all #replace all occurrences
	  Push all #replace all occurrences
	  Push "$ORIGENDIR\etc\bulmacont_$EMPNAME.conf" #file to replace in
	  Call AdvReplaceInFile
	  
	  ;Replace text from '.conf' files
	  ;First bulmacont.conf
	  Push "[[INSTDIR]]" #text to be replaced
	  Push "$ORIGENDIR" #replace with
	  Push all #replace all occurrences
	  Push all #replace all occurrences
	  Push "$ORIGENDIR\etc\bulmacont_$EMPNAME.conf" #file to replace in
	  Call AdvReplaceInFile
  ${EndIf}
  

  
SectionEnd



