<?php include ("inc/funciones.inc") ?>
<?php include ("inc/funcainteligentes.inc") ?>

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
<H1>ASIENTOS INTELIGENTES MOSTRAMOS PLANTILLA</H1>

<?php echo $idainteligente?>
<FORM name="form1" METHOD="post" action="ainteligentes2.php">
<input type="hidden" name="idainteligente" value="<?php echo $idainteligente ?>">
Fecha del asiento:<input type="text" name="fechaasiento"><BR>
<?php recogevarsasiento($idainteligente) ?>
<?php presentavars() ?>
<input type="submit" value="Aceptar">
</FORM>
<!-- END Cuerpo de la página -->
</TD></TR></table>
<?php include("inc/pie.inc") ?>
</BODY>
</HTML>
