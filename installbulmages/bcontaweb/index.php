<HTML>
<BODY>
<table><TR><TD>
<?php include("inc/menu.inc") ?>
</TD><TD>
<!-- BEGIN Cuerpo de la página -->
<CENTER>
<H1>BContaWeb</H1>
Interface web para BulmaConta.<BR>
Esta Web permite interactuar con la contabilidad via WEB.<BR>
Ademas es un buen ejemplo de como se pueden hacer interficies con la contabilidad via web.<BR>
<BR><BR>
<FORM name="formulario" METHOD="post" ACTION="selectempresa.php">
	Login: <INPUT TYPE="text" name="login"><BR>
	Password: <INPUT TYPE="password" name="password"><br>
	<input type="submit" value="Aceptar"><BR>
</FORM>
</CENTER>
<!-- END Cuerpo de la página -->
</TD></TR></table>
<?php include("inc/pie.inc") ?>
</BODY>
</HTML>
