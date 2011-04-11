<?php $selempresa="0"; ?>
<?php include ("inc/funciones.inc") ?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<HTML>
  <head>
    <title>selectempresa.php</title>
  </head>
<BODY>
<table><TR><TD>
<?php include("inc/menu.inc") ?>
</TD><TD>
<!-- BEGIN Cuerpo de la página -->
  <body>
  
  <FORM NAME="form" METHOD="post" ACTION="selectempresa.php">
  <SELECT NAME="idempresa">
 <?php
 $query = "SELECT * FROM empresa";
 
 $result = pg_exec($metabd, $query);
 $filas = pg_NumRows($result);
 while ($filas--) {
?>
	<OPTION VALUE="<?php echo pg_result($result,$filas,"idempresa")?>"><?php echo pg_result($result,$filas,"nombre")?></OPTION> 
<?php
 }// end while
?>
</SELECT>
<INPUT TYPE="submit" VALUE="Cambiar" NAME="Cambiar">
</FORM>
<!-- END Cuerpo de la página -->
</TD></TR></table>
<?php include("inc/pie.inc") ?>
</BODY>
</HTML>
