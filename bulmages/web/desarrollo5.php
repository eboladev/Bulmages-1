<html>
<head>
<title>Untitled Document</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="estilos.css" type="text/css">
</head>

<body bgcolor="#FFFFFF" text="#000000" leftmargin="0" topmargin="0"><CENTER>
<?php
	include ("cabecera.inc");
?>
	<table border="0" cellspacing="0" cellpadding="0" height="88%" bordercolor="#333333" width="750">
  <tr>
      <td width="164" bgcolor="#FFFFFF" valign="top" bordercolor="#003366" height="544"> 
<?php
	include("menu.inc");
?>   
<!-- -------------------------------- -->
<!-- -------------------------------- -->
      
      </td>
      <td width="652" valign="top" align="left" bgcolor="#FFFFFF" height="544"> 
         <table width="100%" border="0" cellspacing="0" cellpadding="0" bgcolor="#000033">
		
            <tr> 
               <td bgcolor="#FFFFFF" valign="top" align="center" height="445">
                     <font face="Verdana, Arial, Helvetica, sans-serif"><font color="#0000FF" size="+2">Desarrollo</font></font>
                  <table width="96%" border="0" cellspacing="0" cellpadding="0">
                     <tr>
                        <td>
                                    
                              <p align="justify"><font face="Verdana, Arial, Helvetica, sans-serif"><b><font size="-3">Tabla apunte</font></b></font></p>
                           </td>
                     </tr>
                  </table>
                     <p>
                     En cada cuenta se van a&ntilde;adiendo apuntes, que son variaciones sobre el estado &quot;econ&oacute;mico&quot; de la cuenta. </p>
                     <table width="100%" border="1" cellspacing="0" cellpadding="0">
                        <tr>
                           <td width="33%">idcodigoborrador</td>
                           <td width="21%">numerico (FK)</td>
                           <td width="46%">Un c&oacute;digo secundario para mnemotecnia</td>
                        </tr>
                        <tr>
                           <td width="33%">idapunte</td>
                           <td width="21%">numerico (K)</td>
                           <td width="46%">Apunte correspondiente con el borrador</td>
                        </tr>
                        <tr>
                           <td width="33%">idasiento</td>
                           <td width="21%">numerico (FK)</td>
                           <td width="46%">Asiento al que pertenecer&aacute; el apunte</td>
                        </tr>
                        <tr>
                           <td width="33%">
                              iddiario
                              </td>
                           <td width="21%">numerico(FK)</td>
                           <td width="46%">Diario al que pertenece el apunte</td>
                        </tr>
                        <tr>
                           <td width="33%">fecha</td>
                           <td width="21%">fecha</td>
                           <td width="46%">Fecha en la que se realiza el apunte</td>
                        </tr>
                        <tr>
                           <td width="33%">conceptocontable</td>
                           <td width="21%">varchar [50]</td>
                           <td width="46%">Concepto </td>
                        </tr>
                        <tr>
                           <td width="33%">idcuenta</td>
                           <td width="21%">numerico (FK)</td>
                           <td width="46%">Cuenta en la que se hace el cargo o abono</td>
                        </tr>
                        <tr>
                           <td width="33%">descripcion</td>
                           <td width="21%">varchar [100]</td>
                           <td width="46%">Descripcion del apunte</td>
                        </tr>
                        <tr>
                           <td width="33%">debe</td>
                           <td width="21%">double (currency)</td>
                           <td width="46%">importe si es un cargo (o al rev&eacute;s)</td>
                        </tr>
                        <tr>
                           <td width="33%">haber</td>
                           <td width="21%">double (currency)</td>
                           <td width="46%">importe si es un abono (o al rev&eacute;s)</td>
                        </tr>
                        <tr>
                           <td width="33%">contrapartida</td>
                           <td width="21%">numerico (FK)</td>
                           <td width="46%">apunte que hace la operaci&oacute;n contraria</td>
                        </tr>
                        <tr>
                           <td width="33%">comentario</td>
                           <td width="21%">varchar [100]</td>
                           <td width="46%"> comentarios al borrador</td>
                        </tr>
                        <tr>
                           <td width="33%">canal</td>
                           <td width="21%">numerico (FK)</td>
                           <td width="46%">una nueva clasificacion de apuntes (para contabilidades dobles, triples, etc)</td>
                        </tr>
                        <tr>
                           <td width="33%">marcaconciliacion</td>
                           <td width="21%">char [12]</td>
                           <td width="46%">&nbsp;</td>
                        </tr>
                     </table>
                     <p>&nbsp;</p>
                     <p><a href="desarrollo4.php">&lt;&lt;Anterior</a>----<a href="desarrollo6.php">Siguiente &gt;&gt; </a>
						
                  </p>
                  </td>
               <td bgcolor="#FFFFFF" valign="middle" align="center" height="445"><img src="img/shim.gif" width="1" height="1"></td>
            </tr>
         </table>
    </td>
  </tr>
</table>
</CENTER>
</body>
</html>
