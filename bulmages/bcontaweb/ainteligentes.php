<?php include ("inc/funciones.inc") ?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<HTML>
  <head>
    <title>selectempresa.php</title>
  </head>
<BODY>
<table><TR><TD valign="top">
<?php include("inc/menu.inc")?>
</TD><TD>
<!-- BEGIN Cuerpo de la página -->
<H1>ASIENTOS INTELIGENTES</H1>
<FORM NAME="form1" METHOD="POST" ACTION="ainteligentes1.php">
<SELECT NAME="idainteligente">
<?php
	$SQLQuery = "SELECT * FROM  ainteligente";
	$result = pg_exec($empresabd,$SQLQuery);
	$filas = pg_NumRows($result);
	while ($filas--) {
		echo "<OPTION VALUE=\"".pg_result($result,$filas,"idainteligente")."\">".pg_result($result,$filas,"descripcion")."</OPTION>";	
	}// end while
?>
</SELECT>
<INPUT TYPE="SUBMIT" VALUE="Aceptar">
</FORM>

<!-- END Cuerpo de la página -->
</TD></TR></table>
<?php include("inc/pie.inc") ?>
</BODY>
</HTML>
