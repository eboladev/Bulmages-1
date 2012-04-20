

;--------------------------------
;Remove PostgreSQL data

Section /o "un.Database" UDB
  RMDir /r "$INSTDIR\pgsql\data"

  ; Borra la configuracion de programa del registro.
  ; Si se borra la base de datos. Entonces se hace la desinstalacion completa del registro de Windows.
  DeleteRegValue HKCU "Software\Bulmages" "BG_InstallDir"
  DeleteRegValue HKCU "Software\Bulmages" "BG_VersionDb"
  DeleteRegValue HKCU "Software\Bulmages" "BG_VersionProgram"

SectionEnd


;--------------------------------
;Uninstaller Section

Section "Uninstall" UUU

  SectionIn RO

  ;CONFIG FILES...
  Delete "$INSTDIR\etc\bulmages.conf"
  Delete "$INSTDIR\etc\bulmafact.conf"

  RMDir "$INSTDIR\etc"

  ;DB FILES...
  Delete "$INSTDIR\db\demo_fact.sql"
  Delete "$INSTDIR\db\fact.sql"

  RMDir "$INSTDIR\db"


  ;BGTRML2PDF FILES...
  Delete "$INSTDIR\bgtrml2pdf\bgtrml2pdf"
  Delete "$INSTDIR\bgtrml2pdf\bgcolor.py"
  Delete "$INSTDIR\bgtrml2pdf\bgutils.py"
  Delete "$INSTDIR\bgtrml2pdf\svgimage.py"
  Delete "$INSTDIR\bgtrml2pdf\flip.exe"

  RMDir /r "$INSTDIR\bgtrml2pdf"


  ; CSS (estilos)
  Delete "$INSTDIR\css\bulmafact_es_ES.css"

  RMDir /r "$INSTDIR\css"


  ;OPENREPORTS FILES...
  Delete "$INSTDIR\openreports\plantilla_empresa.rml"
  Delete "$INSTDIR\openreports\recibo.rml"
  Delete "$INSTDIR\openreports\factura.rml"
  Delete "$INSTDIR\openreports\albaran.rml"
  Delete "$INSTDIR\openreports\articulos.rml"
  Delete "$INSTDIR\openreports\estilos.rml"
  Delete "$INSTDIR\openreports\familias.rml"
  Delete "$INSTDIR\openreports\ficha.rml"
  Delete "$INSTDIR\openreports\listado.rml"
  Delete "$INSTDIR\openreports\logo.jpg"
  Delete "$INSTDIR\openreports\pedidocliente.rml"
  Delete "$INSTDIR\openreports\plantilla.rml"
  Delete "$INSTDIR\openreports\presupuesto.rml"
  Delete "$INSTDIR\openreports\informecliente.rml"
  Delete "$INSTDIR\openreports\pedidoproveedor.rml"


  RMDir "$INSTDIR\openreports"

  Delete "$INSTDIR\program\bulmafact.exe"
  Delete "$INSTDIR\program\libbulmafact.dll"
  Delete "$INSTDIR\program\libbulmalib.dll"
  Delete "$INSTDIR\program\mingwm10.dll"
  Delete "$INSTDIR\program\QtCore4.dll"
  Delete "$INSTDIR\program\QtNetwork4.dll"
  Delete "$INSTDIR\program\QtXml4.dll"
  Delete "$INSTDIR\program\QtGui4.dll"
  Delete "$INSTDIR\program\QtAssistantClient4.dll"
  Delete "$INSTDIR\program\QtWebKit4.dll"
  Delete "$INSTDIR\program\QtXmlPatterns4.dll"
  Delete "$INSTDIR\program\phonon4.dll"
  Delete "$INSTDIR\program\libgcc_s_dw2-1.dll"
  Delete "$INSTDIR\program\libpq.dll"
  Delete "$INSTDIR\program\ssleay32.dll"
  Delete "$INSTDIR\program\libeay32.dll"
  Delete "$INSTDIR\program\libintl-8.dll"
  Delete "$INSTDIR\program\krb5_32.dll"
  Delete "$INSTDIR\program\comerr32.dll"

  Delete "$INSTDIR\program\gssapi32.dll"
  Delete "$INSTDIR\program\iconv.dll"
  Delete "$INSTDIR\program\k5sprt32.dll"
  Delete "$INSTDIR\program\libxml2.dll"
  Delete "$INSTDIR\program\libxslt.dll"
  Delete "$INSTDIR\program\msvcr71.dll"
  Delete "$INSTDIR\program\zlib1.dll"
  Delete "$INSTDIR\program\libiconv-2.dll"

  Delete "$INSTDIR\program\splashbulmafact.png"
  Delete "$INSTDIR\program\bulmafact-background.png"

  Delete "$INSTDIR\program\bulmafact.bat"
  Delete "$INSTDIR\program\bulmafact_demo.bat"
  Delete "$INSTDIR\program\bulmafact.vbs"
  Delete "$INSTDIR\program\bulmafact_demo.vbs"



  Delete "$INSTDIR\program\libeay32.dll"
  Delete "$INSTDIR\program\Microsoft.VC80.CRT.manifest"
  Delete "$INSTDIR\program\msvcm80.dll"
  Delete "$INSTDIR\program\msvcp80.dll"
  Delete "$INSTDIR\program\msvcr80.dll"
  Delete "$INSTDIR\program\msvcr71.dll"
  Delete "$INSTDIR\program\ssleay32.dll"




  Delete "$INSTDIR\program\bulmages_install.ico"
  Delete "$INSTDIR\program\Bulmages_Backup_Tool.exe"
  Delete "$INSTDIR\program\Bulmages_Restore_Tool.exe"
  Delete "$INSTDIR\program\backup.ico"
  Delete "$INSTDIR\program\restore.ico"


  !include "NSIS_includes\include_bulmafact_plugins_remove.nsh"
  !include "NSIS_includes\include_bulmafact_es_ES_remove.nsh"


  ;Uninstall translation system
  !include "NSIS_includes_comunes\gettext_fijo\include_gettext_remove.nsh"

  !include "NSIS_includes_comunes\python\include_python_remove.nsh"
  !include "NSIS_includes_comunes\postgresql\include_postgresql_remove.nsh"
  !include "NSIS_includes_comunes\evince\include_evince_remove.nsh"


  Delete "$SMPROGRAMS\Bulmages\pgAdmin3.lnk"
  Delete "$SMPROGRAMS\Bulmages\Fact\BGW_Fact.lnk"
  Delete "$SMPROGRAMS\Bulmages\Fact\BGW_Fact (demo).lnk"
  Delete "$SMPROGRAMS\Bulmages\Fact\Desinstalar_Fact.lnk"
  Delete "$SMPROGRAMS\Bulmages\Tools\BGW_Backup.lnk"
  Delete "$SMPROGRAMS\Bulmages\Tools\BGW_Restore.lnk"


  Delete "$INSTDIR\desinstalar.exe"


  RMDir "$INSTDIR\program\pluginsbf"
  RMDir "$INSTDIR\program\pluginsbl"
  RMDir "$INSTDIR\program"

  RMDir "$SMPROGRAMS\Bulmages\Fact"
  RMDir "$SMPROGRAMS\Bulmages\Tools"
  
  
  ; Borrado en bestia Realmente la desinstalación es esto.
  RMDir /r "$SMPROGRAMS\Bulmages"
  RMDir /r "$INSTDIR"
  ; Borra el icono del escritorio
  delete "$DESKTOP\Facturacion.lnk"
  delete "$DESKTOP\Contabilidad.lnk"
  delete "$DESKTOP\TPV.lnk"

SectionEnd




;!insertmacro MUI_UNFUNCTION_DESCRIPTION_BEGIN
;  !insertmacro MUI_DESCRIPTION_TEXT ${UDB} "Borra toda la informacion de la base de datos."
;  !insertmacro MUI_DESCRIPTION_TEXT ${UUU} "Borra el programa y todos los componentes."
;!insertmacro MUI_UNFUNCTION_DESCRIPTION_END


Function un.onInit

;	SectionSetText  ${UDB} "Base de datos."
;	SectionSetText  ${UUU} "Programa y componentes."

FunctionEnd


