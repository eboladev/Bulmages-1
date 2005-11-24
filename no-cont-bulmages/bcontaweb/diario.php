<?php include ("inc/funciones.inc") ?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<HTML>
  <head>
    <title>selectempresa.php</title>
  </head>
<BODY>
<table border="1"><TR><TD valign="top">
toma MENU
<?php include("inc/menu.inc")?>
</TD><TD>
<!-- BEGIN Cuerpo de la página -->
<H1> DIARIO</H1>
<FORM name="diario" ACTION="diario.php" METHOD="POST">
Fecha Inicial <INPUT TYPE="text" NAME="finicial"><BR>
Fecha Final <INPUT TYPE="text" NAME="ffinal"><BR>
<INPUT TYPE="submit" VALUE="Aceptar">
</FORM>
<TABLE BORDER="1">
<?php
	$SQLQuery = "SELECT * FROM cuenta, apunte WHERE cuenta.idcuenta = apunte.idcuenta ORDER BY fecha";
	$result = pg_exec($empresabd, $SQLQuery);
	 $filas = pg_NumRows($result);
 	while ($filas--) {
		echo "<TR>";
		echo "<TD>".pg_result($result,$filas,"fecha")."</TD>";
		echo "<TD>".pg_result($result,$filas,"codigo")."</TD>";
		echo "<TD>".pg_result($result,$filas,"descripcion")."</TD>";
		echo "<TD>".pg_result($result,$filas,"conceptocontable")."</TD>";
		echo "<TD>".pg_result($result,$filas,"debe")."</TD>";
		echo "<TD>".pg_result($result,$filas,"haber")."</TD>";
		echo "</TR>";
	}// end while
?>

</TABLE>

<!-- END Cuerpo de la página -->
</TD></TR></table>
<?php include("inc/pie.inc") ?>
</BODY>
</HTML>
