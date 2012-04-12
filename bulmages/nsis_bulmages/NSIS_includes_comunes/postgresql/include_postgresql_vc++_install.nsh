
  ;Instala las librerias de vc++.
  SetOutPath "$INSTDIR\pgsql\bin"
  File /oname=Microsoft.VC80.CRT.manifest "NSIS_includes_comunes\vc++\Microsoft.VC80.CRT.manifest"
  File /oname=msvcm80.dll "NSIS_includes_comunes\vc++\msvcm80.dll"
  File /oname=msvcp80.dll "NSIS_includes_comunes\vc++\msvcp80.dll"
  File /oname=msvcr80.dll "NSIS_includes_comunes\vc++\msvcr80.dll"
  File /oname=msvcr71.dll "NSIS_includes_comunes\vc++\msvcr71.dll"

  ;Copia librerias de vc++.
  SetOutPath "$INSTDIR\pgsql\lib"
  File /oname=Microsoft.VC80.CRT.manifest "NSIS_includes_comunes\vc++\Microsoft.VC80.CRT.manifest"
  File /oname=msvcm80.dll "NSIS_includes_comunes\vc++\msvcm80.dll"
  File /oname=msvcp80.dll "NSIS_includes_comunes\vc++\msvcp80.dll"
  File /oname=msvcr80.dll "NSIS_includes_comunes\vc++\msvcr80.dll"
  File /oname=msvcr71.dll "NSIS_includes_comunes\vc++\msvcr71.dll"


