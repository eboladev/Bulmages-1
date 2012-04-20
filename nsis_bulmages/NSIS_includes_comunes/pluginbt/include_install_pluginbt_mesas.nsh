!insertmacro InstallFilesFromBuild libpluginbt_mesas.dll "\bulmatpv\plugins\pluginbt_mesas\libpluginbt_mesas.dll"

; Generamos todos los confs 
SetOutPath "$INSTDIR\clipart"
!insertmacro InstallFilesFromSrc mesa2_1.svg "\bulmatpv\plugins\pluginbt_mesas\clipart\mesa2_1.svg"
!insertmacro InstallFilesFromSrc mesa4_1.svg "\bulmatpv\plugins\pluginbt_mesas\clipart\mesa4_1.svg"
!insertmacro InstallFilesFromSrc mesa4_2.svg "\bulmatpv\plugins\pluginbt_mesas\clipart\mesa4_2.svg"
!insertmacro InstallFilesFromSrc mesa6_1.svg "\bulmatpv\plugins\pluginbt_mesas\clipart\mesa6_1.svg"
!insertmacro InstallFilesFromSrc mesa6_2.svg "\bulmatpv\plugins\pluginbt_mesas\clipart\mesa6_2.svg"
!insertmacro InstallFilesFromSrc mesa8_1.svg "\bulmatpv\plugins\pluginbt_mesas\clipart\mesa8_1.svg"
!insertmacro InstallFilesFromSrc mesar4_1.svg "\bulmatpv\plugins\pluginbt_mesas\clipart\mesar4_1.svg"
!insertmacro InstallFilesFromSrc mesar8_1.svg "\bulmatpv\plugins\pluginbt_mesas\clipart\mesar8_1.svg"


SetOutPath "$INSTDIR\icons"
!insertmacro InstallFilesFromSrc table.svg "\bulmatpv\plugins\pluginbt_mesas\icons\table.svg"

  
SetOutPath "$INSTDIR\program"