<?php include ("inc/funciones.inc") ?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<HTML>
  <head>
    <title>selectempresa.php</title>
  </head>
<BODY bgcolor="<?php echo $confpr->config("CONF_BG_APUNTES") ?>">
<table><TR><TD valign="top">
<?php include("inc/menu.inc")?>
</TD><TD>
Importacion y exportacion<BR>
    Bulmages &lt;=&gt; Galopin
<HR>
    Nombre Sincronización <input type="text" name="nomsinc" size="60"><BR><BR>
    <TABLE bgcolor="CCCCCC" align="center" border="1" width="98%">
         <FORM method="POST" action="post">
      <TR>
        <TD valign="top">
          <CENTER>BulmaGés</CENTER><BR>
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
       </TD>
	<td valign="top"><CENTER>Galopín</CENTER><BR>
	Base de datos:<input type="text" name="nomdb" size="15"><BR>
	Login usuario:<input type="text" name="loginus" size="15"><BR>
	Password usuario:<input type="text" name="passus" size="15"><BR>
	Servidor:<input type="text" name="servidor" size="15"><BR>
	Puerto:<input type="text" name="puerto" size="15"><BR>
	</td>
     </TR>
     
     
     <tr><TD valign="top">
           <CENTER>Clientes &nbsp;&nbsp;&nbsp;y Proveedores</CENTER><BR>
	   Cuentas de Clientes<input type="text" name="cclientes" size="12"> <IMG src="img/find.png" width="22" height="22" border="0"><BR>
	   Cuentas de Proveedores<input type="text" name="cproveedores" size="12"> <IMG src="img/find.png" width="22" height="22" border="0"><BR>
           <input type="checkbox">Cuentas de cliente incrementales<BR>
           <input type="checkbox">Actualizar BulmaGés con nuevos clientes<BR>
	   <input type="checkbox">Basarse en el NIF como campo de identificacion <BR>
	   <input type="checkbox">Basarse en el nombre como campo de identificacion <BR>
	   <input type="checkbox">Confirmar cada inserción 
          <br>
        </TD><td valign="top">
          Clientes y Proveedores [<A HREF="sincclientes.php">sincronizar</A>]<BR>
	  <input type="checkbox">Actualizar Galopin con nuevos clientes<BR>
	  <input type="checkbox">Confirmar cada inserción <BR>
        </td></tr>
	
	
	
	
	<tr><TD valign="top">
	Asientos
	Asiento inteligente a aplicar <SELECT ></SELECT><br>
	<input type="checkbox">Confirmar cada cambio <BR>
	<input type="checkbox">Comprobar borrado de facturas <BR>
	<input type="checkbox">Comprobar modificacion de facturas<BR>
	</TD>
	<td valign="top">
	Facturas Emitidas/Soportadas
	</td></tr>
	<tr><TD valign="top">
	Asientos
	Asiento inteligente a aplicar <SELECT ></SELECT><br>
	<input type="checkbox">Confirmar cada inserción <BR>
	<input type="checkbox">Comprobar borrado de cobros/pagos <BR>
	<input type="checkbox">Comprobar modificacion de Cobros/Pagos<BR>	
	</TD>
	<td valign="top">
	Cobros/Pagos
	</td></tr>
	
	  </FORM>
    </TABLE><BR>
    <input type="button" value="Guardar">
    <input type="button" value="Ejecutar">
    <input type="button" value="Probar">
<!-- END Cuerpo de la página -->
</TD></TR></table>
<?php include("inc/pie.inc") ?>
</BODY>
</HTML>
