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
<H1>ASIENTOS INTELIGENTES PRESENTAMOS LA PLANTILLA</H1>

<?php echo $idainteligente?>
<?php recogevarsasiento($idainteligente) ?>
<?php recogevalores() ?>
<?php presentavars() ?>
<?php crear($idainteligente, $fechaasiento) ?>

<!-- END Cuerpo de la página -->
</TD></TR></table>
<?php include("inc/pie.inc") ?>
</BODY>
</HTML>
