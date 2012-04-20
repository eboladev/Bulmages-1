
;Instala las librerias de vc++.
SetOutPath "$INSTDIR\Python26"
File /oname=Microsoft.VC90.CRT.manifest "NSIS_includes_comunes\vc++\Microsoft.VC90.CRT.manifest"
File /oname=msvcm90.dll "NSIS_includes_comunes\vc++\msvcm90.dll"
File /oname=msvcp90.dll "NSIS_includes_comunes\vc++\msvcp90.dll"
File /oname=msvcr90.dll "NSIS_includes_comunes\vc++\msvcr90.dll"

SetOutPath "$INSTDIR\Python26\Lib\site-packages\PIL"


;Copia librerias de vc++.
CopyFiles $INSTDIR\Python26\Microsoft.VC90.CRT.manifest $INSTDIR\Python26\Lib\site-packages\PIL\Microsoft.VC90.CRT.manifest
CopyFiles $INSTDIR\Python26\msvcm90.dll $INSTDIR\Python26\Lib\site-packages\PIL\msvcm90.dll
CopyFiles $INSTDIR\Python26\msvcp90.dll $INSTDIR\Python26\Lib\site-packages\PIL\msvcp90.dll
CopyFiles $INSTDIR\Python26\msvcr90.dll $INSTDIR\Python26\Lib\site-packages\PIL\msvcr90.dll


