Set WshShell = CreateObject("WScript.Shell")
WshShell.Run """[[INSTDIR]]\pgsql\bin\pg_ctl.exe"" -D ""[[INSTDIR]]\pgsql\data"" -w -l ""[[INSTDIR]]\pgsql\data\registro"" start",0
WshShell.Run """[[INSTDIR]]\pgsql\bin\pgAdmin3.exe"""
WshShell.Run """[[INSTDIR]]\pgsql\bin\pg_ctl.exe"" -D ""[[INSTDIR]]\pgsql\data"" -w -l ""[[INSTDIR]]\pgsql\data\registro"" stop", 0
Set WshShell = Nothing
