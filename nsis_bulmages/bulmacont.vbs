Set WshShell = CreateObject("WScript.Shell")

If Wscript.Arguments.Count = 0 Then
	WshShell.Run "bulmacont.bat", 0, true
Else
	WshShell.Run "bulmacont.bat "&Wscript.Arguments(0), 0, true
End	If

Set WshShell = Nothing