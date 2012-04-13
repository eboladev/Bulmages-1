
Function accionFinal

  ;Create Icon on Desktop...
  SetOutPath "$INSTDIR\program"
  CreateShortcut "$DESKTOP\BulmaFact.lnk" "$INSTDIR\program\bulmafact.vbs"

FunctionEnd



Section "BulmaFact" SecInstall


  SetOutPath "$INSTDIR\etc"
  ;CONFIG FILES...
  File /oname=bulmages.conf "etc\bulmages.conf"
  File /oname=bulmafact.conf "etc\bulmafact.conf"

  ;Replace text from '.conf' files
  ;First bulmages.conf
  Push "[[INSTDIR]]" #text to be replaced
  Push "$INSTDIR" #replace with
  Push all #replace all occurrences
  Push all #replace all occurrences
  Push "$INSTDIR\etc\bulmages.conf" #file to replace in
   Call AdvReplaceInFile

  ;Replace text from '.conf' files
  ;First bulmafact.conf
  Push "[[INSTDIR]]" #text to be replaced
  Push "$INSTDIR" #replace with
  Push all #replace all occurrences
  Push all #replace all occurrences
  Push "$INSTDIR\etc\bulmafact.conf" #file to replace in
   Call AdvReplaceInFile


  SetOutPath "$INSTDIR\db"
  ;DB FILES...
  File /oname=demo_fact.sql "database\demo_fact.sql"


  SetOutPath "$INSTDIR\bgtrml2pdf"
  ;CONFIG FILES...
  File /oname=bgtrml2pdf "bgtrml2pdf\bgtrml2pdf"
  File /oname=bgcolor.py "bgtrml2pdf\bgcolor.py"
  File /oname=bgutils.py "bgtrml2pdf\bgutils.py"
  File /oname=svgimage.py "bgtrml2pdf\svgimage.py"
  File /oname=flip.exe "flip.exe"

  SetOutPath "$INSTDIR\openreports"
  ;OPENREPORTS FILES...
  File /oname=albaran.rml "openreports\albaran.rml"
  File /oname=articulos.rml "openreports\articulos.rml"
  File /oname=estilos.rml "openreports\estilos.rml"
  File /oname=familias.rml "openreports\familias.rml"
  File /oname=ficha.rml "openreports\ficha.rml"
  File /oname=listado.rml "openreports\listado.rml"
  File /oname=logo.jpg "openreports\logo.jpg"
  File /oname=pedidocliente.rml "openreports\pedidocliente.rml"
  File /oname=plantilla.rml "openreports\plantilla.rml"
  File /oname=plantilla_empresa.rml "openreports\plantilla_empresa.rml"
  File /oname=presupuesto.rml "openreports\presupuesto.rml"
  File /oname=informecliente.rml "openreports\informecliente.rml"



  SetOutPath "$INSTDIR\program"
  ;ADD YOUR OWN FILES HERE...
  !insertmacro InstallFilesFromBuild bulmafact.exe "\bulmafact\src\bulmafact.exe"
  !insertmacro InstallFilesFromBuild libbulmafact.dll "\bulmafact\src\libbulmafact.dll"
  !insertmacro InstallFilesFromBuild libbulmalib.dll "\bulmalib\src\libbulmalib.dll"


  !include "NSIS_includes_comunes\qt_fijo\include_qt_fijo_install.nsh"
  !include "NSIS_includes_comunes\postgresql_fijo\include_postgresql_fijo_install.nsh"





  !insertmacro InstallFilesFromBuild splashbulmafact.jpg "\..\bulmages\installbulmages\images\splashbulmafact.jpg"
  !insertmacro InstallFilesFromBuild bulmafact-background.png "\..\bulmages\installbulmages\images\bulmafact-background.png"

  ; Create file bulmafact.bat
  !include "NSIS_includes\bulmafact.bat.tpt"
  File /oname=bulmafact.vbs "bulmafact.vbs"


  !include "NSIS_includes\include_bulmafact_plugins_install.nsh"


  SetOutPath "$INSTDIR\program\pluginsbl"

  !insertmacro InstallFilesFromBuild libpluginbl_subform2ods.dll "\bulmalib\plugins\pluginbl_subform2ods\libpluginbl_subform2ods.dll"
  !insertmacro InstallFilesFromBuild libpluginbl_clipboard.dll "\bulmalib\plugins\pluginbl_clipboard\libpluginbl_clipboard.dll"


  !include "NSIS_includes_comunes\python\include_python_install.nsh"
  !include "NSIS_includes_comunes\postgresql\include_postgresql_install.nsh"
  !include "NSIS_includes_comunes\evince\include_evince_install.nsh"



  ;Store installation folder
  WriteRegStr HKCU "Software\Bulmages" "" $INSTDIR

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\desinstalar_fact.exe"

  ;Shortcuts
  CreateDirectory "$SMPROGRAMS\Bulmages\BulmaFact"
  SetOutPath "$INSTDIR\program"
  CreateShortCut "$SMPROGRAMS\Bulmages\BulmaFact\BulmaFact.lnk" "$INSTDIR\program\bulmafact.vbs"
  SetOutPath "$INSTDIR"
  CreateShortCut "$SMPROGRAMS\Bulmages\Desinstalar_Fact.lnk" "$INSTDIR\desinstalar_fact.exe"

  ;Initialize PostgreSQL
  DetailPrint 'Iniciando base de datos...'
  nsExec::Exec '"$INSTDIR\pgsql\bin\initdb.exe" -D "$INSTDIR\pgsql\data"'

  DetailPrint 'Activando base de datos...'
  nsExec::Exec '"$INSTDIR\pgsql\bin\pg_ctl.exe" -D "$INSTDIR\pgsql\data" -w -l "$INSTDIR\pgsql\data\registro" start'

  ;Create user and demo database
  DetailPrint 'Creando usuario postgres...'
  nsExec::Exec '"$INSTDIR\pgsql\bin\createuser.exe" -s postgres'
  DetailPrint 'Creando base de datos demo1...'
  nsExec::Exec '"$INSTDIR\pgsql\bin\createdb.exe" demo_fact'

  ;Load data to demo database
  DetailPrint 'Cargando la base de datos con datos...'
  nsExec::Exec '"$INSTDIR\pgsql\bin\psql.exe" -f "$INSTDIR\db\demo_fact.sql" -d demo_fact'

  DetailPrint 'Desactivando base de datos...'
  nsExec::Exec '"$INSTDIR\pgsql\bin\pg_ctl.exe" -D "$INSTDIR\pgsql\data" -w -l "$INSTDIR\pgsql\data\registro" stop'


SectionEnd


