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
                                    
                              <p align="justify"><font face="Verdana, Arial, Helvetica, sans-serif"><b><font size="-3">Tabla acumulado</font></b></font></p>
                           </td>
                     </tr>
                  </table>
                     <p>
                     Sirve para mantener los saldos de las cuentas. Es una tabla redundante que se introduce para acelerar determinados procesos, con el riesgo de inconsistencia de datos.</p>
                     <table width="100%" border="1" cellspacing="0" cellpadding="0">
                        <tr>
                           <td width="33%">idacumulado</td>
                           <td width="21%">autonumerico (K)</td>
                           <td width="46%">El identificador de la tabla.</td>
                        </tr>
                        <tr>
                           <td width="33%">idcuenta</td>
                           <td width="21%">numerico (FK)</td>
                           <td width="46%">La cuenta a la que pertenece.</td>
                        </tr>
                        <tr>
                           <td width="33%">debe</td>
                           <td width="21%">numerico</td>
                           <td width="46%">El acumulado en el debe</td>
                        </tr>
                        <tr>
                           <td width="33%">haber</td>
                           <td width="21%">numerico</td>
                           <td width="46%">El acumulado en el haber</td>
                        </tr>
                        <tr>
                           <td width="33%">&nbsp;</td>
                           <td width="21%">&nbsp;</td>
                           <td width="46%">&nbsp;</td>
                        </tr>
                     </table>
                     <p>&nbsp;</p>
                     <p><a href="desarrollo2.php">&lt;&lt;Anterior</a>----<a href="desarrollo4.php">Siguiente &gt;&gt; </a>                 						
						
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
