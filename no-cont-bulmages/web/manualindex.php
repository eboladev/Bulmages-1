<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>Manual de BulmaGes</title>
<meta http-equiv="Content-Type"
 content="text/html; charset=iso-8859-1">
<style type="text/css">
<!--
a {
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size: 12px;
	color: #663300;
	text-decoration: none;
}

a:hover {
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size: 12px;
	color: #999999;
	text-decoration: none;
}
.head1 {
	font-family: Geneva, Arial, Helvetica, san-serif;
	font-size: 16px;
	font-weight: bold;
	text-transform: capitalize;
	color: #000066;
	filter: Xray;
}

-->
  </style>
<style type="text/css">
<!--
.titular {
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size: 20px;
	font-style: normal;
	font-weight: bold;
	font-variant: small-caps;
	text-transform: uppercase;
	color: #0000FF;
	text-decoration: none;
}
-->
</style>
<style type="text/css">
<!--
.subtitular1 {
	font-family: Arial, Helvetica, sans-serif;
	font-size: 16px;
	font-style: normal;
	line-height: normal;
	font-weight: bold;
	font-variant: normal;
	text-transform: uppercase;
	color: #FF0000;
	text-decoration: none;
	margin-left: 50px;
	text-align: left;
}
.subtitular2 {
	font-family: Arial, Helvetica, sans-serif;
	font-size: 16px;
	font-style: normal;
	font-weight: bold;
	font-variant: small-caps;
	text-transform: capitalize;
	color: #CC3399;
	text-decoration: none;
	margin-left: 100px;
}
body {
	font-family: Arial, Helvetica, sans-serif;
	font-size: x-small;
	font-style: normal;
	color: #663300;
	text-decoration: none;
	text-align: justify;
}
.codigo {
	font-family: "Courier New", Courier, mono;
	font-size: 12px;
	font-weight: bold;
	font-variant: normal;
	text-transform: lowercase;
	color: #006600;
	text-decoration: none;
	background-color: #CCCCCC;
	margin: 3px;
	padding: 3px;
	height: auto;
	width: auto;
	border: 1px dotted #0000FF;
}
-->
</style>
<style type="text/css">
<!--
.destacado1 {
	font-family: Arial, Helvetica, sans-serif;
	font-size: medium;
	font-weight: bold;
	color: #000000;
	text-decoration: none;
}
-->
</style>
</head>
<body>
<div align="center"><span class="titular">MANUAL DEL PROGRAMA BULMAG&Eacute;S 
  </span><br> <br>
  Julio-Agosto 2003 (C) Cristina Marco Hern&aacute;ndez <br>
  <br>
</div>
<div class="titular">INTRODUCCIÓN</div>
  
  <p>En los meses de Junio y Julio de 2001 surgi&oacute; en la lista de distribuci&oacute;n 
  de Bulma (<a href="http://bulmalug.net">http://bulmalug.net</a>), la idea de 
  realizar programas de gesti&oacute;n para empresas desarrollados en software 
  libre. El objetivo era claro, hacer posible el uso de Linux en el d&iacute;a 
  a d&iacute;a de una empresa. Para ello es preciso que las empresas cuenten con 
  software de calidad que les permita abordar todas las tareas que ya tienen informatizadas.</p>
<p>Con el tiempo, fue escogido como primer candidato a ser implementado un programa 
  dedicado a la contabilidad por los siguientes motivos:</p>
      <p> 
<LI> Es un programa aislado que influye a un &uacute;nico departamento; no es 
  vital para el funcionamiento de la empresa y no es necesario migrar todo el 
  paquete inform&aacute;tico de la empresa para utilizar BulmaG&eacute;s: basta 
  con un ordenador</LI>
</p>
      <p>
<LI> Los requisitos para un programa contable est&aacute;n bien definidos y son 
  pr&aacute;cticamente los mismos para cualquier empresa, con lo que el programa 
  tendr&aacute;, probablemente, mayores posibilidades de ser utilizado que cualquier 
  otro candidato dedicado a otro sector empresarial.</LI>
</p>
      <p>
<LI> No exist&iacute;a ning&uacute;n programa que se dedicase a la contabilidad. 
  Actualmente, a&uacute;n existen muy pocas alternativas reales. Y la mayor parte de 
  los proyectos dedicados a este tema han quedado olvidados.</LI>
</p>
      
<p>Tras dos a&ntilde;os de aparente inmovilidad del proyecto, por fin, en Noviemgre 
  de 2002 se comenz&oacute; la codificaci&oacute;n del programa.</p>
    <br>
    <div class="titular">INSTALACI&Oacute;N DE BULMAGES</div>
    
    <br>
      
    <br><div class="subtitular1" style="font-weight: bold;">A.- Instalaci&oacute;n R&aacute;pida 
      </div><br>
      <br>
<li>El primer paso para instalar el programa es descargarlo. Puede obtener un 
  archivo <strong>tgz</strong> con todos los archivos necearios para el programa 
  en <a href="http://bulmages.bulma.net">http://bulmages.bulma.net</a></li>
<br>
      <p>
<li>A continuaci&oacute;n descomprima el archivo <strong>tgz</strong> en un directorio</font></li>
</p>
      <p>
<li>Ejecute como <strong>root</strong> el script de instalaci&oacute;n <strong>install-bulmages</strong></font></li>
</p>
      
<p>
<li>Compruebe que el directorio<strong> /usr/local/bin</strong> se encuentra en 
  el path.</li>
</p>
<p> 
<li>Para arrancar el programa basta con escribir <strong>bulmages</strong>.</font></li>
</p>
      
<p class="subtitular2">Requisitos del programa:</p>
<p>Para el correcto funcionamiento del programa se requiere:</font></p>
      <li>postgresql</li>
      <li>kde3.x </li>
      <li>libpq</li>
      
<p ><div class="destacado1">Nota:</div>
<br>
&nbsp;&nbsp;&nbsp; Revise el apartado de <em>Configuraci&oacute;n del Motor de 
Bases de Datos PostgreSQL</em> de esta misma secci&oacute;n para comprobar que 
tiene las bases de datos correctamente instaladas.<br>
<br></p>
<div class="subtitular1">B.- Instalaci&oacute;n Manual</div>
<p >En este punto se detallar&aacute;n todos los requisitos necesarios para poner 
  en funcionamiento el programa, sin tener que hacer uso del script de instalaci&oacute;n. 
  De esta forma ser&aacute; m&aacute;s f&aacute;cil comprender los componentes 
  del programa.</p>
<p ><br></div>
  Esta instalaci&oacute;n es algo m&aacute;s laboriosa pero permite tener mayor 
  control sobre los componentes de la aplicaci&oacute;n. No es preciso ejecutar 
  el script de instalaci&oacute;n installbulmages para que funcione, pero s&iacute; 
  es preciso tener acceso de root en la m&aacute;quina en la que se vaya a instalar 
  el programa.<br>
</p>
      
<h4>
  <div class="subtitular2">&nbsp; B1.- Instalaci&oacute;n de los servicios. </div>
</h4>
      Partiendo de que el <strong>servidor de postgresql</strong> ya est&aacute; 
instalado instalado y correctamente configurado, para instalar las bases de datos 
requeridas para que funcione Bulmag&eacute;s deber&aacute; acceder al directorio 
<strong>dbmodels</strong> y buscar en &eacute;l tres archivos de bases de datos 
(<strong>pgdump</strong>). Si su usuario no est&aacute; dado de alta en postgres 
puede hacerlo mediante los siguientes comandos (suponiendo que el usuario <em>tborras</em> 
no tiene permisos para acceder a bases de datos de postgres):<br>

     <br> <p class="codigo">
      /root/:# su postgres</div><br
 style="font-style: italic;">
      /home/postgres:$ createuser tborras</div></p>
      <br>
El primer comando permite que el sistema entre en la cuenta de superusuario de 
postgres, de esta forma se pueden ejecutar comandos en la base de datos.<br>
El segundo comando crea el usuario <em>tborras</em>, para que &eacute;ste pueda 
acceder a la base de datos.<br>
      <br>
Tras estos dos comandos el sistema formular&aacute; algunas preguntas sobre si 
quiere que el usuario pueda crear nuevas bases de datos (Respuesta: SI).<br>
      <br>
Hecho esto ya se pueden crear las bases de datos del programa mediante los comandos:<br>
      <br>
      <p class="codigo">/home/tborras:$ createdb bulmages<BR> 
     /home/tborras:$ createdb metabd<BR>
	 /home/tborras:$ createdb bgplangcont</p>
      <br>
Si todo ha funcionado bien, el sistema mostrar&aacute; el mensaje <strong>CREATE 
DATABASE</strong> indicando que la base de datos ha sido creada.<br>
      <br>
Tras esto se pueden volcar los datos que se suministran con la instalaci&oacute;n 
del programa<br>
      <br>
      <p class="codigo">/home/tborras:$ psql bulmages
&lt; bulmages.pgdump<br style="font-style: italic;">
      /home/tborras:$ psql metabd
&lt; metabd.pgdump<br style="font-style: italic;">
      /home/tborras:$ psql
bgplangcont &lt; bgplangcont.pgdump</p>
      <br>
Estos comandos deben generar bastante salida de datos en pantalla.<br>
Si todo ha ido correctamente, y no ha aparecido ning&uacute;n mensaje de error, 
en el sistema estar&aacute;n creadas las bases de datos que va a utlizar el programa.<br>
      <br>
Si se ha producido alg&uacute;n error, o quiere volver a instalar, siempre se 
puede borrar la base de datos con el comando:<br>
      <br>
      
<p class="codigo">/home/tborras:$ dropdb bulmages<br>
/home/tborras:$ dropdb metabd<br>
/home/tborras:$ dropdb bgplangcont</p>
<br
 style="font-style: italic;">
      <br>
Lea el <em>manual de referencia de postgreSQL</em> (<a href="http://www.postgresql.org" target="_blank">http://www.postgresql.org</a>) 
para m&aacute;s informaci&oacute;n sobre la administraci&oacute;n de bases de 
datos.<br>
      <br>
      <div class="subtitular2">B2.- Instalaci&oacute;n del cliente de contabilidad</div>
<p> Para instalar el cliente de contabilidad lo &uacute;nico que hay que hacer 
  es copiar una serie de archivos en determinadas ubicaciones.</p>
<p>1. Copiar <strong>bulmages.conf</strong> en el directorio <strong>etc/</strong><br>
  Este es el archivo de configuraci&oacute;n del programa y debe tener permisos 
  de lectura.<br>
  <br>
  2. Copiar<strong> bulmages</strong>, <strong>borraremp</strong>, <strong>cargaemp</strong>, 
  <strong>guardaemp</strong> y <strong>nuevaemp</strong> en <strong>/usr/local/bin</strong><br>
  Estos archivos son los ejecutables y deben tener permisos de ejecuci&oacute;n. 
  Adem&aacute;s, el directorio<strong> /usr/local/bin</strong> debe estar en el 
  <strong>PATH</strong> para que pueda ser ejecutado.<br>
  <br>
  3. Copiar el resto de archivos en <strong>/usr/share/bulmages</strong><br>
  Estos son los archivos auxiliares del programa y deben tener permisos de lectura.<br>
  <br>
  Es posible que la parte cliente y la parte servidor est&eacute;n alojadas en 
  distintas m&aacute;quinas. Incluso puede tener varios clientes atacando a una 
  misma base de datos. Para hacerlo existen dos par&aacute;metros en el archivo&nbsp;<strong> 
  /etc/bulmages.conf</strong> que indican a qu&eacute; servidor de bases de datos 
  debe dirigirse el programa. Estos par&aacute;metros son <strong>CONF_SERVIDOR</strong> 
  y <strong>CONF_PUERTO</strong>, donde se puede indicar a qu&eacute; servidor 
  de bases de datos y puerto de conexi&oacute;n debe hacer el programa las peticiones 
  de base de datos.<br>
  <br>
</p>
<h4 class="subtitular1">C.- Configuraci&oacute;n del motor de bases de datos
PostgreSQL:</h4>
      
<p >La mayor complicaci&oacute;n que puede surgir con el programa es la correcta 
  instalaci&oacute;n de las bases de datos.</p>
      <p  class="subtitular2"><strong>C1.-Permisos de los usuarios:</strong></p>
      
<p >Postgres trabaja con varios m&eacute;todos de autentificaci&oacute;n. Estas 
  opci&oacute;nes se configuran en el archivo<strong> pg_hba.conf</strong>. Esta es la 
  principal causa de problemas con el programa. (En <em>Debian</em> este archivo 
  se encuentra en el directorio<strong> /etc/postgresql/</strong>)</p>
      
<p >La opci&oacute;n de autentificaci&oacute;n <strong>trust</strong> permite 
  que todos los usuarios tengan acceso a la base de datos con lo que no habr&aacute; 
  problemas de autentificaci&oacute;n y todos los usuarios podran utilizar sin 
  restricciones las bases de datos. Asi que, si se dispone el tipo de acceso trust 
  y las bases de datos de la aplicaci&oacute;n han sido creadas, el programa funcionar&aacute; 
  sin problemas.</p>
      
<p >La opci&oacute;n de autentificaci&oacute;n <strong>ident sameuser</strong> 
  s&oacute;lo permite el acceso a la base de datos a aquellos usuarios que han 
  sido creados previamente. Esta opci&oacute;n es m&aacute;s segura ya que s&oacute;lo 
  los usuarios que tengan cuenta en el ordenador tendr&aacute;n acceso a las bases 
  de datos. Para utilizar <strong>Bulmag&eacute;s</strong> con este modelo de 
  autentificaci&oacute;n debr&aacute; asegurarse de que los usuarios que van a 
  usar el programa han sido dados de alta en la base de datos. Para ello puede 
  usar el comando:</p>
      <p  class="codigo"">/home/postgres:$
create user xxxxxxxx</p>
      
<p >donde<strong> xxxxxxxxx </strong>es e<strong>l login del usuario</strong> 
  al que se le otorgar&aacute;n permisos de acceso a la base de datos.</p>
      <p ></p>
      
    



<HR>


  
    <br>
      <div class="titular">EJECUCI&Oacute;N DEL PROGRAMA BULMAGES</div>
    
    <p>Una vez que el programa ha sido correctamente configurado, el siguiente paso 
  es ejecutarlo. </p>
      <div style="text-align: center;"><img src="imgmanual/splash.png" title="" alt="splash de la version 0.3.1" style="width: 449px; height: 357px;"></div>
      
<p>El prgrama se ejecuta mediante el comando <strong>bulmages</strong>.&nbsp; 
  Una vez ejecutado aparecer&aacute; una pantalla de entrada donde debe seleccionar 
  la empresa sobre la que vaya a trabajar. El programa solicitar&aacute; un login 
  y un password. Por defecto el programa tiene el usuario <strong>bulmages</strong> 
  predefinido y su password es <strong>bulmages</strong>.</p>
      
    
  



<HR>
<BR>

    

  <div class="titular">PRIMEROS PASOS CON EL PROGRAMA</div>
<br>
  <div class="subtitular1">INTRODUCCI&Oacute;N AL PROGRAMA</div> 
<p> <strong>BulmaG&eacute;s</strong> es un proyecto dedicado a resolver la tarea 
  de la contabilidad de empresa de una forma sencilla y elegante y siempre bajo 
  el punto de vista del <strong>Software Libre</strong>. </p>
<p>El programa est&aacute; dise&ntilde;ado pensando en el usuario final. Intenta 
  que la mayoria de las tareas de contabilidad sean completamente accesibles y 
  f&aacute;ciles de llevar a cabo mediante un sistema muy intuitivo de utilizar 
  el programa.</p>
<p><strong>BulmaG&eacute;s</strong> ofrece un interfaz amigable y totalmente configurable, 
  con el que pretende facilitar la comprensi&oacute;n del programa, hacer el entorno 
  de trabajo amigable y confundir lo menos posible al usuario permitiendo que 
  opci&oacute;nes susceptibles de no ser utilizadas puedan desaparecer de la pantalla.<br>
  <br>
</p>
<div class="subtitular1">LA PANTALLA DE INICIO: ABRIR EMPRESA</div>
    
    <br>
      <br>

            <br>
            
<div >La pantalla de <strong>inicio</strong> permite seleccionar la <strong>empresa</strong> 
  con la que se desee trabajar. Esta pantalla incluye el nombre que se le haya 
  proporcionado a la empresa, el ejercicio al que se refiere y el nombre del archivo 
  en el que se guardar&aacute;n los datos relativos al ejercicio en curso.</div>
            
          
          <br>
            <br>
Para comenzar a utilizar <strong>BulmaG&eacute;s</strong> ser&aacute; necesario 
<strong>abrir</strong> una <strong>empresa</strong>. <strong>BulmaG&eacute;s</strong> 
incluye por defecto un plan contable est&aacute;ndar, una empresa de pruebas y 
la configuraci&oacute;n de un usuario "bulmages". Por lo tanto, para acceder por 
primera vez al programa ser&aacute; necesario seleccionar la <strong>empresa de 
pruebas</strong> (<strong>bulmages</strong>) y el usuario por defecto. <br>
            

  
<p>Los campos <strong>login</strong> y <strong>password</strong> permanecer&aacute;n 
  vac&iacute;os a menos que se hayan especificado unos anteriormente. El programa 
  tiene creado, por defecto, un ususario que puede acceder a todas las empresas. 
  usted puede acceder al sistema bajo ese usuario. Para ello puede usar los siguientes 
  login y password:<br>
  </p>
  
<p><font color="#0000FF">LOGIN</font>: bulmages<br>
  <font color="#0000FF">PASSWORD</font>: bulmages<br>
</p>
<hr>
<p>&nbsp; </p>
En el caso de que ya se hayan creado diferentes empresas, para comenzar a operar 
con el programa, deber&aacute; seleccionar la <strong>empresa </strong>con la 
que quiera trabajar e introducir despu&eacute;s el <strong>login </strong> y el 
<strong>password</strong> de un usario que tenga acceso a dicha empresa. <br>
<br>
      <br><div class="titular"> PANTALLA PRINCIPAL</div>
    
    <br>
      <br>
      
<div >La pantalla principal de <strong>BulmaG&eacute;s</strong> le ofrece las 
  opci&oacute;nes generales del programa en dos modalidades: el <em>men&uacute; </em>superior 
  y la <em>barra de acceso r&aacute;pido</em> inferior. Dentro de la pantalla 
  general se abren las pantallas correspondientes a las opci&oacute;nes espec&iacute;ficas 
  del programa (por ejemplo, la pantalla de <strong>Apuntes Contables</strong>, 
  la <strong>Gesti&oacute;n de Asientos</strong>, etc.)</font></div>
      
    
    <br>
      <br>
      
<div >Al iniciar una sesi&oacute;n de trabajo en una <strong>empresa</strong> 
  seleccionada en <strong>BulmaG&eacute;s</strong>, aparece la pantalla principal 
  que abre por defecto las siguientes subpantallas:</font></div>
      
    
    <br>
      <br>
      <div >- INTRODUCCION DE APUNTES
CONTABLES </font></div>
      
    
    <br>
      <br>
      <div >- LIBRO DIARIO DE APUNTES</font></div>
      
    
    <br>
      <br>
      
<div >- LIBRO MAYOR o EXTRACTO DE CUENTAS</font></div>
      
    
    <br>
      - BALANCE DE CUENTAS.<br>
      
    
    <br>
      - BALANCE JER&Aacute;RQUICO.<br>
      

<HR>

  <br> 
    <div class="titular">GESTI&Oacute;N DE EMPRESAS</div>
  
  
<br> 
<div ><strong>BulmaG&eacute;s</strong> puede trabajar con tantas empresas como 
  el usuario desee, asignando as&iacute; a cada empresa el ejercicio econ&oacute;mico 
  que corresponda. El men&uacute; <strong>empresa</strong> consta de las siguientes 
  opci&oacute;nes:</font></div>
  
  <br> 
    <div class="subtitular1" style="vertical-align: top;">ABRIR EMPRESA</div>
  
  <br>
Mediante esta opci&oacute;n se puede crear el asiento de apertura de una empresa. 
Para crear el asiento de apertura de una empresa es preciso que el &uacute;ltimo 
asiento introducido sea el asiento de cierre de la empresa.<br>
 <br>
<div class="subtitular1" style="vertical-align: top;">CERRAR EMPRESA</div>
  
  
<p> Mediante esta opci&oacute;n se crea el asiento de cierre de la empresa.<br>
</p>
<p>Es posible generar tantos asientos de cierre y de apertura como se desee durante 
  el ejercicio contable.</p>
<div class="subtitular1" style="vertical-align: top;">REGULARIZAR EMPRESA</div>
  
  <br>
Regularizar una empresa es el asiento previo al asiento de cierre. Con este asiento 
se desplazan los resultados del ejercicio a la cuenta de perdidas y ganancias.<br> 
  
  <br> 
    <div class="subtitular1">NUEVA EMPRESA</div>
  
  <br>
<div >Permite a&ntilde;adir una nueva empresa con su correspondiente ejercicio 
  contable.</font> Para crear una nueva empresa es necesario acceder al men&uacute; 
  <strong>empresa</strong> y seleccionar la opci&oacute;n de nueva <strong>empresa</strong>.</div>
  
  
<div align="center"><br>
  <img src="imgmanual/nempresa.png" title="" alt="nueva empresa" style="width: 595px; height: 422px;"> 
</div>
<p> Al crear una nueva empresa, el programa solicita el nombre de la empresa, 
  el nombre de la base de datos que la va a contener y el ejercicio que se va 
  a introducir.<br>
  En la misma ventana, se pueden asignar los diferentes usuarios que tienen permisos 
  de acceso a la empresa y el tipo de acceso (lectura o modificaci&oacute;n).<br>
  La creaci&oacute;n de una empresa implica la creaci&oacute;n de una base de 
  datos nueva. Por lo tanto, el usuario que ejecuta el programa debe tener privilegios 
  para crear bases de datos en postgres.</p> 
  
  <div class="subtitular1">BORRAR EMPRESA</div>
  
  <br>
Le permite borrar una empresa existente de forma definitiva, incluyendo todos 
los datos que se hayan introducido hasta el momento.<br>
Para borrar una empresa primero es necesario seleccionar la empresa que se desee 
borrar. Mediante la opci&oacute;n <strong>borrar empresa </strong>del men&uacute; la 
empresa abierta ser&aacute; borrada autom&aacute;ticamente.<br>
  
  <br>
<div class="subtitular1">CAMBIAR EMPRESA</div>
  
  <br>
<div >Mediante esta opci&oacute;n puede cambiar la empresa activa a otra empresa 
  diferente a la seleccionada en la <strong>pantalla de inicio</strong>: <strong>abrir 
  empresa. </strong>Esto ser&aacute; posible siempre que la empresa que se vaya 
  a seleccionar haya sido creada anteriormente. Puede cambiar de empresa tantas 
  veces como desee siempre y cuando no est&eacute; en mitad del proceso de introducci&oacute;n 
  de datos.<br>
        <br>
  La ventana de cambio de empresa es la misma que la de apertura de empresa, es 
  decir, la ventana que aparece tras ejecutar el programa. Tambi&eacute;n se puede 
  cambiar el usuario mediante esta ventana.<br>
        </font></div>
  
  
<div align="center"><img src="imgmanual/cambiaempresa.png" title="" alt="cambiar empresa" style="width: 398px; height: 234px;"></div>

<br><br>

<div class="subtitular1"> PROPIEDADES EMPRESA. <img src="imgbotones/boton_propempresa.gif" width="24" height="25"></div>
    
<p>Permite consultar y modificar los par&aacute;metros especificados para la <strong>empresa 
  activa</strong>. </p>
  
<div align="center"><img src="imgmanual/propempresa.png" title="" alt="propiedades empresa" style="width: 591px; height: 410px;"> </div>

<p> Mediante esta pantalla puede cambiar parametros de configuraci&oacute;n de 
  la empresa. Estos parametros son: el n&uacute;mero de d&iacute;gitos que por 
  defecto tienen las cuentas, la cuenta de regularizaci&oacute;n de la empresa 
  y los permisos que tienen los usuarios sobre la empresa. </p> 
    <div class="subtitular1">GUARDAR EMPRESA</div>
  
  <br>
Permite hacer una copia de seguridad de la empresa activa. La totalidad de los 
datos de esa empresa son guardados en un archivo en la ubicaci&oacute;n que se 
indique.<br>
        </font></div>
  
  <br> 
    <div class="subtitular1">CARGAR EMPRESA</div>
  
  
<br>
 Opci&oacute;n para restaurar una 
        copia de seguridad realizada con anterioridad de la empresa seleccionada 
        o para a&ntilde;adir los datos de otra empresa.</font></div>
  
	
	
	<HR>
	
<br>
    <br>
      
    <big><big><big><div
 style="color: rgb(0, 0, 153); font-weight: bold;"><div class="titular"><img src="imgbotones/boton_usuarios.gif" width="24" height="24">GESTION 
      DE USUARIOS</div></div></big></big></big>
    
    <br>
Mediante el bot&oacute;n <img src="imgbotones/boton_usuarios.gif" width="24" height="24"> 
del <strong>toolbar</strong> o la opci&oacute;n del men&uacute; Empresa puede 
gestionar los usuarios: Crear, borrar y modificar los usuarios que tienen acceso 
al programa.<br>
<br>
      <div style="text-align: center;"><img
 src="imgmanual/usuarios.png" title=""
 alt="gestion de usuarios" style="width: 608px; height: 373px;"><br>
      </div>
      
    
    <br>
En la parte de la izquierda de la ventana puede seleccinar el <strong>usuario</strong> 
que quiera editar y modificarlo con los campos situados a la derecha. <br>
Si teniendo un usuario seleccionado pulsa sobre el bot&oacute;n <img src="imgbotones/boton_borrar.gif" width="24" height="24">, 
podra <strong>eliminar</strong> el usuario seleccionado.<br>
Si pulsa sobre el bot&oacute;n <img src="imgbotones/boton_nuevo.gif" width="24" height="24"> 
aparecer&aacute; un<strong> usuario nuevo</strong> en la parte izquierda que luego 
podr&aacute; modificar con los datos que precise.<br>
      
    

<br>
<HR>

  
    
 <br>
    <br><div class="titular"><img src="imgbotones/boton_cuenta.gif" width="24" height="24"> 
    PLAN CONTABLE</div>
  <br><BR>
  <center>
  <img src="imgmanual/plancuentas.png" alt="plan de cuentas" style="width: 597px; height: 454px;" align="center"><br>
  </center>
   <br>
  <br>

<p>Mediante esta pantalla se puede <strong>ver</strong>, <strong>modificar</strong> 
  y <strong>borrar</strong> todas las cuentas y subucentas del plan contable de 
  la empresa.</p>

          
<p>El plan de cuentas es accesible mediante el bot&oacute;n <img src="imgbotones/boton_cuenta.gif" width="24" height="24"> 
  y mediante la opci&oacute;n<strong> Plan Contable</strong> del men&uacute; <strong>Cuenta</strong>.</p>

          
<p>El plan de cuentas muestra el plan contable con el que trabja la empresa activa. 
  <strong>Bulmag&eacute;s</strong> se suministra con el <strong>Nuevo Plan General 
  Contable B&aacute;sico</strong> por defecto, pero ofrece la posibilidad de modificarlo 
  y personalizarlo. De esta forma, cada empresa puede operar con un plan contable 
  personalizado.<br>
  <br>
  La pantalla presenta dos pesta&ntilde;as:
<UL>
          
  <li>En la primera (<strong>Plan Contable</strong>) se puede ver un &aacute;rbol 
    de cuentas de la empresa.</li>
  <br>
		  
          
  <li> En la segunda (<strong>Subcuentas</strong>) aparece un listado de las subcuentas 
    de la empresa.</li>
		  </UL>
		  
          
          
<p>Para crear una <strong>nueva cuenta</strong> se utiliza el bot&oacute;n <img src="imgbotones/boton_nuevo.gif" width="24" height="24"> 
  Al seleccionar una cuenta, pulsando este bot&oacute;n, la cuenta que cree se 
  colocar&aacute; utom&aacute;ticamente como &quot;cuenta hija&quot; de la cuenta 
  seleccionada.</p>
          
          
<p>Puede<strong>editar</strong> cualquier cuenta haciendo doble click en la cuenta 
  deseada. Tambi&eacute;n puede editarla seleccion&aacute;ndola y pulsando sobre 
  el bot&oacute;n <img src="imgbotones/boton_editar.gif" width="24" height="24">.</p>
<p>Puede <strong>borrar</strong> cualquer cuenta (que no tenga &quot;cuentas hijas&quot;, 
  ni apuntes contables apuntados en ella) seleccionandola y pulsando sobre el 
  bot&oacute;n <img src="imgbotones/boton_borrar.gif" width="24" height="24"></p>
      
    
    <div class="subtitular1">EDICI&Oacute;N DE CUENTAS. <img src="imgbotones/boton_editar.gif" width="24" height="24"></div>
    
    <br><br>
	<center><img src="imgmanual/edicuenta.png" title="" alt="edicion de cuentas" style="width: 600px; height: 381px;"></center><br>
      
    
    <br>
Al crear una cuenta nueva, o editar una cuenta existente, aparece la ventana del 
dibujo superior. En ella se muestran todos los datos de la cuenta.<br>
La pesta&ntilde;a <strong>Otros Datos</strong> contiene informaci&oacute;n adicional 
de la cuenta, como cuentas bancarias, nombres, CIF, e-mails y datos de contacto 
que se puedan necesitar para la gesti&oacute;n de dicha cuenta.<br>
      
    
<br>
<hr>
  
    <br> 
      <br>
	  
<div class="titular"><img src="imgbotones/boton_intapunts.gif" width="24" height="24"> 
  <img src="imgbotones/boton_asientos.gif" width="24" height="24">ASIENTOS / INTRODUCCI&Oacute;N 
  DE APUNTES</div>
    
	<BR><br>
      <center><img src="imgmanual/intapunts.png" title="" alt="introduccion de apuntes." style="width: 853px; height: 338px;"></center>
	  
	  
<p><br>
  Esta pantalla permite la introducci&oacute;n de <strong>asientos</strong> y 
  <strong>apuntes contables</strong>.</font></div> <br>
  <br>
  La <strong>gesti&oacute;n de asientos</strong> es una de las partes principales 
  del programa ya que los datos que se introduzcan ser&aacute;n los que proporcionen 
  despu&eacute;s la visi&oacute;n exacta de la contabilidad. Es a partir de estos 
  datos de los que <strong>BulmaG&eacute;s </strong> obtiene la informaci&oacute;n 
  necesaria para elaborar los diferentes informes o listados. Por ejemplo, el 
  listado del <strong>diario</strong> o el <strong>libro de mayor</strong>..</font></div> 
  <br>
  <br>
  La pantalla de <strong>introducci&oacute;n de asientos</strong> se abre por 
  defecto al inicializar el programa situando la pantalla por defecto en el &uacute;ltimo 
  <strong>n&uacute;mero de asiento</strong> introducido. De esta forma se facilita 
  la inserci&oacute;n de asientos siguiendo un orden correlativo.<br>
  En el caso de que no haya ning&uacute;n asiento introducido previamente (por 
  ejemplo en empresas reci&eacute;n creadas) <strong>BulmaG&eacute;s</strong> 
  abrir&aacute; la <strong>Gesti&oacute;n de Asientos</strong> en el n&uacute;mero 
  1 (uno).</font></div> <br>
  Para modificar un asiento existente habr&aacute; que comenzar por abrir el asiento 
  mediante el bot&oacute;n abrir asiento <img src="imgbotones/boton_abreasiento.gif" width="24" height="24">.</p>
<p> Una vez en la pantalla, se debe pulsar el bot&oacute;n de <strong>siguiente 
  </strong>(dibujado como una flecha). Al pulsar esta opci&oacute;n, <strong>BulmaG&eacute;s</strong> 
  detectar&aacute; que se desea crear un asiento nuevo y preguntar&aacute;: "No 
  existe un asiento con el n&uacute;mero que ha proporcionado, &iquest;desea crear 
  uno?". Esto significa que el programa crear&aacute; un nuevo asiento con un 
  n&uacute;mero correlativo al anterior.</p>
  
  
<p>Una vez abierto un nuevo asiento, el fondo de la pantalla cambia a color blanco 
  y el programa permite la introducci&oacute;n de datos en los diferentes campos 
  disponibles. </p>
  
 
   <p>La introducci&oacute;n de asientos puede realizarse mediante la introducci&oacute;n 
  libre de los datos directamente en la pantalla principal. Para agilizar el proceso, 
  el programa permite cambiar de una opci&oacute;n a otra con el teclado y sin 
  la necesidad de recurrir al rat&oacute;n.</p>
  
  
  
<p>Bulamges le ofrece tambi&eacute;n la posibilidad de introducir los asientos 
  en dos modalidades diferentes:</p>
  
  
<p>Los <strong>Asientos Simples</strong> consisten en insertar los datos en el 
  programa apunte por apunte.</p>
  
  
<p>Los <strong>Asientos Inteligentes</strong> o <strong>Predefinidos</strong> 
  son aquellos que, habiendo establecido con anterioridad sus propiedades, facilitan 
  la introducci&oacute;n de asientos repetidos o usuales en la contabilidad, como 
  por ejemplo pagos mensuales a proveedores, gestiones usuales con el banco, etc. 
  Usted puede crear tantos <strong>asientos inteligentes </strong>como desee estableciendo 
  los valores que considere oportunos para cada uno de ellos.</p>
  
  
<p>Hay diversos par&aacute;metros de esta ventana que son cofigurables a trav&eacute;s 
  del archivo de configuraci&oacute;n del programa (ver secci&oacute;n bulmages.conf). 
  Podr&aacute; cambiar la tipograf&iacute;a y los colores de la fuente. Tambi&eacute;n 
  podr&aacute; elegir si mostrar o no mostrar determinados campos.</p>

  
<div class="subtitular1">NAVEGACI&Oacute;N A TRAV&Eacute;S DEL SISTEMA DE ASIENTOS. 
  <img src="imgbotones/boton_navegacion.gif" width="113" height="24"></div>
     
      
<p>Puede navegar a trav&eacute;s de los asientos introducidos mediante los <strong>bot&oacute;nes 
  de flecha</strong> en las toolbars (barras de herramientas). De esta forma puede 
  acceder al bot&oacute;n siguiente y anterior, al primer y al ultimo asiento. 
  Segun su orden de creaci&oacute;n.</p> 
    
    <div class="subtitular1">FILTRADO DE ASIENTOS. <img src="imgbotones/boton_filtrar.gif" width="24" height="24"></div>
	  
	  
<p>Puede interesarle trabajar con una serie de <strong>asientos que cumplan unas 
  caracter&iacute;sticas determinadas</strong>. Puede que su trabajo sea m&aacute;s 
  sencillo si mediante la navegaci&oacute;n s&oacute;lo se presentan aquellos 
  asientos que cumplan unas caracter&iacute;sticas espec&iacute;ficas. Para ello, 
  puede establecer un filtro que haga que s&oacute;lo se presenten los asientos 
  que cumplan determinadas caracter&iacute;sticas con el bot&oacute;n <img src="imgbotones/boton_filtrar.gif" width="24" height="24"> 
  situado en las toolbars.</p>
        <p>Inmediatamente aparecer&aacute; la ventana siguiente que le permite establecer 
        los criterios del filtro.</p>
        
<p>Si tiene un filtro activado y desea quitarlo, en dicha ventana tiene un bot&oacute;n 
  de <strong>reset</strong> que desactiva todos los parametros del filtro y le 
  mostrar&aacute; todos los asientos existentes.</p>
		 <br> 
		 <div style="text-align: center;"><img src="imgmanual/filtroasiento.png" title="" alt="filtrado de asientos." style="width: 608px; height: 203px;"></div>
    
   <br>  <br> <div  class="subtitular1">ASIENTOS SIMPLES</div>
    
    <br>
<div >Para introducir asientos simples tan s&oacute;lo es necesario <strong>abrir 
  un asiento</strong> o <strong>crear uno nuevo </strong>tal y como se ha explicado 
  anteriormente.<br>
  Una vez que el programa le deja modificar / introducir datos, la pantalla que 
  muestra el asiento presenta todos los apuntes que componen el mismo en forma 
  de filas de una tabla. Cada fila se corresponde con un apunte.<br>
          Cada apunte se compone de los siguientes campos:<br>
          <br>
          <div style="font-weight: bold; color: rgb(51, 51, 255);">FECHA:</div> 
          La fecha del apunte indica la<strong> fecha en que se realiza la transacci&oacute;n 
  econ&oacute;mica</strong>. La fecha puede introducirse de distintas formas: 
  <p>- Escribiendo la fecha en cualquiera de los siguientes formatos: DDMMAA, 
    DDMM, DDMMAAAA, DD/MM/AA, DD/MM/AAAA. Al pulsar enter (tras haber introducido 
    la fecha), se cambiar&aacute; el formato de &eacute;sta al formato est&aacute;ndar 
    DD/MM/AAAA.</p>
          
  <p> - Otra forma de introducir la fecha es pulsando la <strong>tecla +</strong> 
    al editar dicho campo. Autom&aacute;ticamente aparecer&aacute; una ventana 
    con un <strong>calendario</strong> en el que podr&aacute; seleccionar la fecha 
    deseada. Esta misma opci&oacute;n aparece al pulsar con el bot&oacute;n derecho 
    sobre la fecha y seleccionando del men&uacute; contextual la opci&oacute;n 
    "<strong>Seleccionar Valor</strong>"</p>
          <div style="text-align: center;"><img
 src="imgmanual/calendario.png" title=""
 alt="calendario" style="width: 198px; height: 173px;"></div>
          
  <p>Editando el campo de fecha, si pulsa sobre la <strong>tecla * </strong>autom&aacute;ticamente 
    colocar&aacute; en el campo que est&aacute; editando el mismo valor que el 
    campo de fecha anterior.</p>
          <div style="color: rgb(51, 51, 255); font-weight: bold;">SUBCUENTA:</div><div
 style="font-weight: bold;"> </div>
  La introducci&oacute;n de cuentas puede hacerse de diversas formas:
  <br><br>
  <li> Escribiendo el n&uacute;mero de cuenta mediante el <strong>teclado</strong>. 
    Tambi&eacute;n puede introducir la cuenta de <strong>forma abreviada</strong>, 
    la cuenta 1000004 puede introducirse de la forma &quot;1.4,&quot; y la cuenta 
    1000000 puede introducirse de la forma "1."&nbsp; El sistema&nbsp; extender&aacute; 
    el <strong>.</strong> de forma que el numero de cuenta tenga el n&uacute;mero 
    de d&iacute;gitos que se usan por defecto en la contabilidad de la empresa.<br>
          
  <li> Otra forma de introducir cuentas es pulsando la<strong> tecla + </strong>o 
    la <strong>opci&oacute;n elegir valor</strong> en el men&uacute; contextual. 
    Directamente aparecer&aacute; la ventana del plan de cuentas, con la salvedad 
    que al hacer doble click sobre una cuenta &eacute;sta aparecer&aacute; en 
    el la casilla que est&aacute; editando.<br>
          
  <li> Editando el campo de <strong>subcuenta</strong>, si pulsa sobre la <strong>tecla 
    *</strong> autom&aacute;ticamente se trasladar&aacute; al campo que est&aacute; 
    editando el mismo valor que el campo de subcuenta anterior. (Note que las 
    teclas seleccionadas sirven para hacer la introducci&oacute;n sin dejar de 
    utilizar el teclado num&eacute;rico).<br>
          
  <li> Si lo desea, puede acceder de <strong>forma r&aacute;pida</strong> a ver 
    el extracto de una subcuenta que est&eacute; visualizando en la ventana de 
    apuntes. Para ello el asiento debe estar cerrado. Pulsando con el <strong>bot&oacute;n 
    derecho</strong> sobre la cuenta de la que quiera ver su extracto puede seleccionar 
    la opci&oacute;n <strong>ver extracto</strong>. Adem&aacute;s, el programa 
    le permite escoger entre ver un <strong>extracto total</strong>, un <strong>extracto 
    del mes</strong> en que est&aacute; introducido el apunte o un <strong>extracto 
    del d&iacute;a</strong> de introducci&oacute;n del apunte.<br>
    <br>
    <br>
    <div style="color: rgb(51, 51, 255); font-weight: bold;">CONTRAPARTIDA:</div>
    <div
 style="font-weight: bold;"> </div>
    (Puede que en su configuraci&oacute;n del programa no aparezca el campo de 
    contrapartida, ver secci&oacute;n de bulmages.conf). <br><br>
    La introducci&oacute;n de contrapartida es similar en funciones a la introducci&oacute;n 
    de subcuenta. <br>
    <br>
    &nbsp;&nbsp;&nbsp; La contrapartida es <strong>semi-autom&aacute;tica</strong>, 
    el programa siempre elige de forma inteligente una contrapartida para todo 
    apunte introducido, puede ver dicha contrapartida a trav&eacute;s de un extracto 
    o un diario. Puede darse el caso de que la contrapartida elegida no sea la 
    apropiada, entonces puede introducir la contrapartida correcta en el asiento 
    mediante este campo. El programa siempre dar&aacute; preferencia a la contrapartida 
    que usted ha introducido.<br>
    <br>
    <div style="color: rgb(51, 51, 255);"> 
      <div
 style="font-weight: bold;">DEBE / HABER:</div>
    </div>
    Puede introducir directamente un valor en el campo de debe o de haber, <strong>pero 
    s&oacute;lo en uno de ellos</strong>, el otro debe valer obligatoriamente 
    cero.<br>
<li> Si pulsa la <strong>tecla +</strong> en uno de estos dos campos el valor 
    de descuadre del asiento ser&aacute; asignado a dicho campo.<br>
    
  <li> Si pulsa la <strong>tecla *</strong> el campo presentar&aacute; el mismo 
    valor que el campo anteriormente introducido.<br>
    <br style="color: rgb(51, 51, 255); font-weight: bold;">
    <div style="color: rgb(51, 51, 255); font-weight: bold;">CONCEPTO:</div>
    El concepto es una <strong>descripci&oacute;n del apunte</strong>, no tiene 
    m&aacute;s que introducir un texto explicativo del mismo. (De momento en dicho 
    texto no estan permitidas las teclas * y +).<br>
    
  <li> Si pulsa la <strong>tecla *</strong> al editar dicho campo, el campo tomar&aacute; 
    el mismo valor que su predecesor.<br>
    <br>
    <div style="color: rgb(51, 51, 255); font-weight: bold;">IVA:</div>
    La introducci&oacute;n de IVA debe realizarse mediante el bot&oacute;n <strong>IVA</strong> 
    y, normalmente, se hace de forma autom&aacute;tica al cerrar el asiento. Este 
    campo aparece aqui para indicar que dicho apunte lleva asociada una entrada 
    en el registro de IVA. El valor que aparece indica el <strong>porcentaje</strong> 
    de IVA correspondiente. En espa&ntilde;a los valores est&aacute;ndars de iva 
    son 0, 4, 7 y 16 %<br>
    <br>
    <div style="color: rgb(51, 51, 255); font-weight: bold;">CENTRO DE COSTE:</div>
    A&uacute;n por documentar<br>
    <br>
    <div style="color: rgb(51, 51, 255); font-weight: bold;">CANAL:</div>
    A&uacute;n por documentar<br>
    </font>
    </div>
    
    <br>
Los apuntes en el asiento pueden ser alterados en su orden, mediante la combinaci&oacute;n 
de teclas<strong> CTRL + ARRIBA y CTRL + ABAJO </strong>Puede subir o bajar un 
apunte determinado. (Tambi&eacute;n dispone de esta opci&oacute;n en el men&uacute; 
contextual que aparece al pulsar con el<strong> bot&oacute;n derecho</strong> sobre el 
apunte).<br>
 <br>
Puede querer <strong>borrar</strong> un apunte determinado. Para ello pulse con 
el<strong> bot&oacute;n derecho del rat&oacute;n </strong>sobre el y haga click 
sobre la opci&oacute;n de borrar apunte.<br>
        <br>
En determinados casos le interesar&aacute; <strong>duplicar</strong> un apunte, 
para ello tambi&eacute;n dispone de una opci&oacute;n en el men&uacute; contextual.<br>    
    
<div >Es recomendable rellenar todos los datos para que, posteriormente, pueda 
  comprender mejor los listados e informes que puede proporcionarle el programa.</div>
    
    
<br> 
<div >Si despu&eacute;s de cerrar el asiento desea <strong>modificarlo</strong>, 
  puede hacerlo volviendo a abrirlo con el bot&oacute;n de <strong>Abrir candado.</strong></div>
    
    
<br> 
<div >En el caso de que el <strong>asiento</strong> quede descuadrado, el programa 
  le avisar&aacute; cuando intente cerrar el asiento con un mensaje que le permite 
  volver a la edici&oacute;n del asiento o bien <strong>guardarlo abierto </strong>para 
  su posterior modificaci&oacute;n y cierre. Los asientos abiertos no intervienen 
  en los listados contables. Por lo que es recomendable tener bien controlados 
  los asientos que ha dejado abiertos. Una contabilidad no puede ser considerada 
  &quot;correcta&quot; si mantiene asientos abiertos. <br>
        </div>
    
    
<strong>BulmaG&eacute;s</strong><strong></strong> le permite a&ntilde;adir a cada 
asiento tantos apuntes como considere necesario siempre y cuando el resultado 
final quede cuadrado.<br>
<div >Puede que desee <strong>cambiar la fecha de un asiento </strong>ya introducido, 
  o <strong>escribir algun comentario</strong> sobre dicho asiento, incluso<strong> 
  cambiar el n&uacute;mero de asiento</strong>, para ello dispone de la ventana 
  de edici&oacute;n de asientos pulsando sobre el bot&oacute;n <img src="imgbotones/boton_editar.gif" width="24" height="24">, 
  donde aparecer&aacute; la siguiente ventana que le permite cambiar todos los 
  datos mencionados.<br>
</div>
        <div style="text-align: center;"><img
 src="imgmanual/detallasiento.png" title=""
 alt="detalle del asiento" style="width: 509px; height: 252px;"><br>
        </div>
    
    <br>
Bulmag&eacute;s dispone de un<strong> listado r&aacute;pido de asientos</strong>. 
Con dicho listado puede ver todos los asientos introducidos en el sistema de forma 
r&aacute;pida, si estan abiertos o cerrados, e incluso el saldo deudor y acreedor 
de los mismos. Este listado le puede ser &uacute;til a la hora de verificar el 
sistema contable. Puede acceder a dicho listado mediante el bot&oacute;n <img src="imgbotones/boton_asientos.gif" width="24" height="24"><br> 
        <br> <div style="text-align: center;"><img
 src="imgmanual/listasientos.png" title=""
 alt="listado de asientos" style="width: 521px; height: 382px;"><br>
        </div>
    
    <br> 
      <div style="vertical-align: top;">
<div class="subtitular1">ASIENTOS INTELIGENTES. <img src="imgbotones/boton_ainteligente.gif" width="24" height="24"></div>
<br>
    
    <br> 
      
  <div class="subtitular2" style="vertical-align: top;">INTRODUCCI&Oacute;N DE 
    ASIENTOS MEDIANTE ASIENTOS INTELIGENTES</div>
    
    <br>
  Puede introducir un asiento completo pulsando sobre el bot&oacute;n <img src="imgbotones/boton_ainteligente.gif" width="24" height="24"> 
  El asiento debe estar abierto para que se realice la introducci&oacute;n de 
  un asiento inteligente. Autom&aacute;ticamente aparecera la siguiente pantalla:<br>
   <br> <div style="text-align: center;"><img
 src="imgmanual/ainteligente.png" title=""
 alt="asiento inteligente" style="width: 608px; height: 279px;"><br>
          <br>
          <div style="text-align: left;">
      <p>En la parte superior, puede seleccionar la <strong>plantilla</strong> 
        a utilizar para la introducci&oacute;n del apunte. En la parte inferior 
        se pediran los <strong>datos</strong> necesarios para la <strong>confeccion</strong> 
        del asiento. </p>
      <p> Una vez rellenados los datos, puede presionar el<strong> bot&oacute;n 
        aceptar</strong> y automaticamente se crear&aacute;n los apuntes que forman 
        el asiento en el <strong>asiento abierto</strong>.<br>
      Puede introducir tantos asientos inteligentes como desee en un mismo asiento.<br>
    </p>
  </div>
        </div>
    
    <div class="subtitular1">CREACION DE ASIENTOS INTELIGENTES. <img src="imgbotones/boton_ainteligente.gif" width="24" height="24"></div>
    
    
  <p> Puede crear tantos asientos inteligentes como precise para su contabilidad. 
    Para ello dispone de la opci&oacute;n <img src="imgbotones/boton_ainteligente.gif" width="24" height="24"> 
    en las toolbars y en el men&uacute; de la aplicaci&oacute;n.</p>
  Automaticamente aparecer&aacute; la siguiente pantalla:<br>
   <br> <div style="text-align: center;"><img
 src="imgmanual/ainteligentecreacion.png"
 title="" alt="creacion de asientos inteligentes."
 style="width: 942px; height: 332px;"><br>
        </div>
    
    
  <p> La creaci&oacute;n de nuevos asientos inteligentes funciona como la introducci&oacute;n 
    de un asiento, con la salvedad de que en los campos en los que sea preciso 
    puede introducir <strong>variables</strong> u <strong>operaciones con variables</strong>.</p>
		
        
  <p>Para hacer que un campo determinado del asiento sea variable basta con introducir 
    el nombre de variable <strong>entre signos de dolar</strong> ($). Esto har&aacute; 
    que, una vez que est&eacute; trabajando con el asiento inteligente, para introducir 
    dicho valor antes sea preguntado al usuario en la plantilla. Tambi&eacute;n 
    se puede operar variables introduciendo una <strong>f&oacute;rmula</strong> 
    que use la variable y en la creaci&oacute;n del asiento se introducir&aacute; 
    autom&aacute;ticamente el resultado de la operaci&oacute;n.</p>
		
		 
        
  <p>Existen <strong>variables predefinidas</strong> por el programa, que, a&uacute;n 
    siendo variables, no provocan que el sistema pregunte por ellas ya que son 
    calculadas de forma autom&aacute;tica.</p>
  <strong>Por ejemplo:</strong><br>
  <DIR> $fechaactual$ -- ser&aacute; sustituida por la fecha del dia actual<br>
  $fechaasiento$ -- la fecha del asiento.</DIR>
 
  <p>Puede, al mismo tiempo que crea todo el asiento, crear entradas en el registro 
    de IVA de forma autom&aacute;tica. Para ello dispone de la pesta&ntilde;a 
    adicional <strong>Registro IVA</strong>. Note que si deja un asiento inteligente 
    sin entradas de registro de IVA al cerrar el asiento creado se le preguntar&aacute; 
    por ellas.</p>
    

<hr>

<br>
  <br>
      <div ><div class="titular"><img src="imgbotones/boton_mayor.gif" width="24" height="24"> 
          LIBRO MAYOR</div></div>
      
    
    
  <br>
      
  <div >Muestra el <strong>Libro Mayor</strong> y permite la posibilidad de <strong>listar 
    los apuntes</strong> de las cuentas y subcuentas existentes en la base de 
    datos del programa.<br>
      <br>
      <div style="text-align: center;"><font size="1"
 face="Verdana, Arial, Helvetica, sans-serif"><img
 src="imgmanual/extracto.png" title=""
 alt="extracto de cuentas" style="width: 920px; height: 484px;"></font><br>
      </div>
      </div>
      
    
    <br>
  <div ></div>
En el extracto hay cuatro pa&#341;ametros b&aacute;sicos que lo delimitan:<br>
      <br>
      <span style="color: rgb(51, 51, 255);">Cuenta Inicial</span>: La
cuenta inicial a partir de la que se quiere empezar el extracto<br>
  <span style="color: rgb(51, 51, 255);">Cuenta Final:</span> La cuenta que supondr&aacute; 
  el final del listado del extracto.<br>
      <span style="color: rgb(51, 51, 255);">Fecha Inicial:</span>
Fecha inicial de los apuntes a mostrar<br>
  <span style="color: rgb(51, 51, 255);">Fecha Final:</span> Fecha final a partir 
  de la cual no se mostrar&aacute;n m&aacute;s apuntes.<br>
      <br>
      
    
    <br>
  Una vez introducidos los cuatro datos b&aacute;sicos, el extracto mostrar&aacute; 
  los apuntes referentes a cada una de las cuentas referenciadas entre las fechas 
  introducidas. Puede navegar entre las cuentas mediante los bot&oacute;nes de 
  navegaci&oacute;n de la toolbar, accediendo a la cuenta siguiente, la cuenta 
  anterior, la primera y la &uacute;ltima cuenta. <br>
      <br>
  Si quiere actualizar la informaci&oacute;n dispone del bot&oacute;n acutalizar 
  <strong>#bot&oacute;n actualizar#</strong><br>
      <br>
  Para cada cuenta el extracto se muestra primero una <strong>informaci&oacute;n 
  preliminar</strong> como es el c&oacute;digo de la cuenta, su nombre, y el debe, 
  haber y saldo que tiene dicha cuenta antes de la fecha inicial.<br>
      <br>
  Despu&eacute;s, en forma de tabla, se presentan <strong>informaci&oacute;n detallada</strong> 
  con los diferentes apuntes introducidos en la contabilidad durante el periodo 
  seleccionado y ordenados por fecha de apunte.<br>
      <br>
Al final aparecen el debe, haber y saldo de la cuenta a fecha final.<br>
      <br>
  Los campos que aparecen son fecha, n&uacute;mero de asiento, descripci&oacute;n, 
  contrapartida, debe, haber, saldo, centro de coste y canal. En su versi&oacute;n 
  del programa puede que no aparezcan todos los campos ya que los campos a visualizar 
  son configurables mediante el archivo bulmages.conf (ver secci&oacute;n bulmages.conf).<br>
      
    
    <br>
    
  <div class="subtitular1">IMPRESI&Oacute;N&nbsp; DE EXTRACTOS. <img src="imgbotones/boton_imprimir.gif" width="24" height="24"></div>
    
    
  <p> Puede que usted quiera una copia impresa de su extracto. En ese caso dispone 
    del bot&oacute;n <img src="imgbotones/boton_imprimir.gif" width="24" height="24"> 
    que har&aacute; que aparezca el siguiente cuadro de di&aacute;logo solicit&aacute;ndolelas 
    opci&oacute;nes de impresi&oacute;n.</p>

      <div style="text-align: center;"><img src="imgmanual/imprimirextracto.png" title="" alt="imprimir extracto" style="width: 608px; height: 246px;"></div>
	  <br>
      
  <p>Los campos aparecer&aacute;n rellenos con los datos que tiene puestos en 
    pantalla, con lo que s&oacute;lo tendr&aacute; que elegir el formato y pulsar 
    el bot&oacute;n imprimir.</p>
  <p><strong>Bulmag&eacute;s</strong> abrir&aacute; un editor de textos o un navegador 
    con el extracto visualizado.</p>
    
    <br>
      
  <div class="subtitular1" style="vertical-align: top;">PUNTEO Y CASACI&Oacute;N</div>
    
    <br>
  Sobre el extracto, puede interesarle ir <strong>punteando los apuntes</strong> 
  para, por ejemplo, comprobar con un extracto bancario la correcci&oacute;n de 
  los mismos.<br>
  Para realizar dicha operaci&oacute;n dispone en el extracto de <strong>casillas 
  de verificaci&oacute;n</strong> sobre las que puede pulsar para poner o quitar 
  el punteo. El punteo se guarda autom&aacute;ticamente en la base de datos, con 
  lo que si cambia de ventana no perder&aacute; el punteo.<br>
      <br>
  <strong>BulmaG&eacute;s</strong> tambi&eacute;n le ofrece la funci&oacute;n 
  de guardar el punteo en un momento determinado para poder recuperarlo m&aacute;s 
  adelante si es preciso. En la ventana del extracto dispone del bot&oacute;n 
  <strong>#bot&oacute;n guardar punteo# </strong>que le preguntar&aacute; un nombre de 
  archivo y la localizaci&oacute;n del mismo para guardar el punteo.<br>
      <br>
  M&aacute;s adelante, si quiere recuperar el punteo guardado con anterioridad, 
  dispone del bot&oacute;n <strong>#bot&oacute;n recuperar punteo# </strong>que le solicitar&aacute; 
  a partir de qu&eacute; archivo recuperar el punteo.<br>
      <br>
  Puede tener varios punteos almacenados en el sistema y recuperarlos cuantas 
  veces desee.<br>
      <br>
      <div style="font-weight: bold;">NOTA:</div> Si tras guardar el
punteo ha realizado alguna modificaci&oacute;n de apuntes, o
inserci&oacute;n de nuevos asientos contables puede que el resultado de
recuperar el punteo no sea correcto para los apuntes modificados o para
los nuevos apuntes insertados.<br>
      <br>
  En <strong>BulmaG&eacute;s</strong> existe tambi&eacute;n la opci&oacute;n de 
  <strong>puntear directamente los apuntes que tienen un par id&eacute;ntico</strong>. 
  Esto quiere decir que&nbsp; existe un apunte con un importe en el debe y otro 
  apunte en la misma cuenta con un importe id&eacute;ntico en el haber. Este caso 
  suele ocurrir en aquellos tipos de apuntes suelen corresponder con una misma 
  transacci&oacute;n econ&oacute;mica que ya ha sido terminada y que ha dejado 
  el saldo de la cuenta igual que estaba. Para ello tiene el bot&oacute;n de <strong>casaci&oacute;n</strong> 
  #bot&oacute;n de casaci&oacute;n#. Al pulsar dicho bot&oacute;n directamente se puntear&aacute;n 
  todos los pares de apuntes con importes rec&iacute;procos en el extracto de 
  la cuenta que est&eacute; visualizando.<br>
      
    
    <br>
<div class="subtitular1">FILTRADO DEL EXTRACTO. <img src="imgbotones/boton_filtrar.gif" width="24" height="24"></div>
<br>
  Puede ocurrir que le sea m&aacute;s c&oacute;modo trabajar si el sistema omite 
  determinados apuntes sobre los que usted no est&aacute; interesado. Para ello 
  tiene el bot&oacute;n de <strong>filtrado</strong> que le permite seleccionar las caracter&iacute;sticas 
  que deben cumplir aquellos apuntes que desea ver.<br>
      <br>
      <div style="text-align: center;"><img
 src="imgmanual/filtroextracto.png" title=""
 alt="filtrado en el extracto." style="width: 584px; height: 310px;"><br>
      </div>
      
    
	
	
	<hr>
	
	<br>
    <br>
      <br>
      <div class="titular"><img src="imgbotones/boton_diario.gif" width="25" height="23">&nbsp;LIBRO 
        DIARIO</div>
      
<br><br>
      
      
<div align="center"><img src="imgmanual/diario.png" title="" alt="libro diario" style="width: 993px; height: 427px;"></div>
 
 <br>
  <br>
  El <strong>diario</strong> tiene dos parametros b&aacute;sicos que lo definen:<br>
  <br>
  <span style="color: rgb(51, 51, 255);">Fecha Inicial:</span> Fecha inicial de 
  los apuntes a mostrar<br>
  <span style="color: rgb(51, 51, 255);">Fecha Final:</span> Fecha final a partir 
  de la cual no se mostrar&aacute;n m&aacute;s apuntes.<br>
  <br>
  El libro diario muestra todos los apuntes introducidos por <strong>orden cronol&oacute;gico</strong>.<br>
  <br>
  Con el bot&oacute;n derecho del raton podra ver un men&uacute; contextual que 
  permite ir al asiento correspondiente, mostrar el libro balance, o un extracto 
  de cuentas del apunte seleccionado.<br>
  <br>
</div>
<div class="subtitular1">FILTRADO DEL DIARIO. <img src="imgbotones/boton_filtrar.gif" width="24" height="8"></div>
    
    Puede que desee ver en el diario s&oacute;lo los apuntes que cumplan determinadas 
caracter&iacute;sticas. Para ello tiene la opci&oacute;n de filtrado con el bot&oacute;n 
<img src="imgbotones/boton_filtrar.gif" width="24" height="24"><br>
<br>
      <div style="text-align: center;"><img
 src="imgmanual/FILTROdiario.png" title=""
 alt="filtro diario" style="width: 584px; height: 310px;"><br>
      </div>
      
    
    <br>
      
<div class="subtitular1">IMPRESI&Oacute;N DEL DIARIO. <img src="imgbotones/boton_imprimir.gif" width="24" height="24"></div>
    
	
<p>Cuando se encuentre trabajando con el diario, si presiona el bot&oacute;n <img src="imgbotones/boton_imprimir.gif" width="24" height="24"> 
  aparecer&aacute; la siguiente ventana:</p>
<br>
      <center><img
 src="imgmanual/printdiario.png" title=""
 alt="imprimir diario" style="width: 608px; height: 233px;"></center>
<p>Los campos aparecer&aacute;n rellenos con los datos que tiene puestos en pantalla, 
  con lo que s&oacute;lo tendr&aacute; que elegir el formato y pulsar el bot&oacute;n 
  imprimir.</p>
<p><strong>Bulmag&eacute;s</strong> abrir&aacute; un editor de textos o un navegador 
  con el extracto visualizado.</p>
    
    <br>
      
    

<HR>

<br>
    <br>
      
    <div style="vertical-align: top;" class="titular"><img src="imgbotones/boton_balance.gif" width="24" height="24"> 
      <img src="imgbotones/boton_balancetree.gif" width="24" height="24">BALANCE 
      DE CUENTAS</div>
    
	
<p>El <strong>balance de cuentas</strong> es un listado muy utilizado en contabilidad 
  (<strong>Balance de Sumas y Saldos</strong>). <strong>BulmaG&eacute;s </strong>ofrece 
  dicho listado en dos versiones diferentes.<br>
</p>
<div class="subtitular1"> BALANCE DE CUENTAS NORMAL. <img src="imgbotones/boton_balance.gif" width="24" height="24"></div>
  
      
<p>El balance de cuentas normal muestra las <strong>cuentas de un determinado 
  nivel y sus saldos</strong> entre un periodo de tiempo. Puede elegir el nivel 
  de detalle que desea para sus cuentas y un rango de cuentas a mostrar.</p>
<p>Puede suceder que cuentas de niveles superiores tengan asociados apuntes contables 
  (a&uacute;nqe no es una buena pr&aacute;ctica contable). En este caso el balance tambi&eacute;n 
  mostrar&aacute; dichas cuentas ya que, en caso contrario, no se ofrecer&iacute;a 
  una imagen fiel de la empresa.</p>
<p align="center"><img src="imgmanual/balance.png" title="" alt="balance de cuentas" style="width: 908px; height: 366px;"></p>
<br>
<div class="subtitular1">BALANCE JER&Aacute;RQUICO DE CUENTAS. <img src="imgbotones/boton_balancetree.gif" width="24" height="24"></div>
<p>El <strong>balance jer&aacute;rquico</strong> de cuentas ofrece la misma informaci&oacute;n 
  que el balance normal desde una perspectiva diferente. Mediante el balance jer&aacute;rquico 
  podr&aacute; ver el &aacute;rbol de cuentas y los correspondientes saldos que 
  suben a trav&eacute;s del arbol. De esta forma puede ver con claridad qu&eacute; 
  cuentas componen cada una de las partes de las cuentas de nivel superior.</p>
<p>Puede elegir a qu&eacute; nivel quiere expandir el &aacute;rbol de forma que 
  siempre puede controlar la cantidad de informaci&oacute;n mostrada.</p>
<p align="center"><img
 src="imgmanual/balancejerarquico.png"
 title="" alt="balance jerarquico" style="width: 908px; height: 366px;"><br>
</p>
<div style="vertical-align: top;">
  <div class="subtitular1">IMPRESI&Oacute;N DEL BALANCE. <img src="imgbotones/boton_imprimir.gif" width="24" height="24"></div>
  <p>Puede obtener una copia impresa del balance (en cualquiera de sus dos modalidades) 
    presionando el bot&oacute;n <img src="imgbotones/boton_imprimir.gif" width="24" height="24"> 
    Inmediantamente aparecer&aacute; la ventana de impresi&oacute;n de balance.</p>

  <p>En esta ventana puede elegir las opci&oacute;nes del listado, tambi&eacute;n 
    puede elegir el formato deseado para el mismo. Cuando pulse el bot&oacute;n 
    imprimir se abrir&aacute; el programa visor del listado y se lo presentar&aacute; 
    as&iacute;: </p>
  <div align="center"><br>
    <img src="imgmanual/balanceprint.png" title="" alt="impresion de balance" style="width: 608px; height: 364px;"><br>
  </div>
  <HR>
<br>
<br>
<div >
    <div class="titular">ESTAD&Iacute;STICAS </div>
</div>
<br>
<div class="subtitular1">SEGUIMIENTO DE CUENTAS</div> <br>
  Mediante la opci&oacute;n <strong>estadisticas </strong>/<strong> seguimiento 
  de cuentas</strong> del men&uacute; de la aplicaci&oacute;n puede acceder a 
  una pantalla que le permite seguir de forma gr&aacute;fica la progresi&oacute;n 
  de las cuentas de su empresa. Mediante esta pantalla puede seguir el saldo, 
  los movimientos y las principales contrapartidas que intervienen con la cuenta 
  introducida. Puede llegar a comparar hasta tres cuentas diferentes.<br> 
<br>
<center><img src="imgmanual/estadisticas1.png" title="" alt="Estadisticas" style="width: 945px; height: 450px;"></center><br>
<br>
<center><img src="imgmanual/estadisticas2.png" title="" alt="estadisticas" style="width: 945px; height: 450px;"></center><br>
  <br>
<div ><div class="subtitular1">BALANCE GR&Aacute;FICO</div></div>
<br>
  Mediante esta opci&oacute;n puede obtener<strong> informaci&oacute;n gr&aacute;fica 
  sobre el balance de cuentas</strong>. Gracias al diagrama circular del balance 
  puede ver qu&eacute; cuentas tienen mayor relevancia en su contabilidad. Lo 
  que muestra el balance gr&aacute;fico es el saldo de las cuentas de<strong> 
  nivel 2</strong>.<br>
<br>
<center><img src="imgmanual/bgrafico.png" alt="balance gr&aacute;fico" style="width: 768px; height: 438px;" title=""></center>
<br>
<HR>
<br>
  <div class="titular">OTRAS opciOnES</div>  
  <div >
    <p>Permite posicionar las ventanas abiertas en el programa en diferentes posiciones 
      predefinidas.</p>
    </div>
  <div > 
    <div class="head1">NEW WINDOW</div>
    <a name="3271"></a></div>
  <br>
  <div > 
    <div class="head1"> CASCADE</div>
    <a name="3272"></a></div>
  <div >Muestra todas las ventanas existentes en ese momento, una tras otra, en 
    el orden en que han sido abiertas.<br>
  </div>
  <div > 
    <div class="head1">TITLE</div>
    <a name="3273"></a></div>
  <br>
  <div >Muestra todas las ventanas abiertas ocupando el m&aacute;ximo espacio 
    del total de la pantalla.</div>
  <br>
  <div > 
    <div class="head1"> OTRAS oPCIONES</div>
    <a name="3274"></a></div>
  <br>
  <div >Permite alternar entre las ventanas abiertas en ese momento sin necesidad 
    de mostrarlas todas o de minimizarlas o maximizarlas.<br>
  </div>
  <div > 
    <div class="subtitular1"> EL MEN&Uacute; VER</div>
    <a
 name="328"></a></div>
  <br>
  <div > 
    <div class="head1"> TOOLBAR</div>
    <a name="3281"></a></div>
  <div >Muestra u oculta la barra de herramientas del programa seg&uacute;n est&eacute; 
    o no seleccionada esta opci&oacute;n.</div>
  <br>
  <div > 
    <div class="head1">STATUSBAR</div>
    <a name="3282"></a></div>
  <div >Muestra u oculta la barra de estatus del programa seg&uacute;n est&eacute; 
    o no seleccionada esta opci&oacute;n.</div>
  <br>
</div>
<div class="subtitular1" >EL MEN&Uacute; AYUDA</div>
<div style="vertical-align: top;"> <br>
  <br>
  <div class="subtitular1"> ACERCA DE </div>
  <br>
  <div >Muestra la informaci&oacute;n del programa: la versi&oacute;n, los autores 
    y colaboradores y las empresas o entidades que proporcionan o han proporcionado 
    soporte al programa as&iacute; como algunas direcciones de inter&eacute;s.<br>
    Para m&aacute;s informaci&oacute;n sobre el programa consulte la web oficial:<br>
    <a href="http://bulmages.bulma.net">http://bulmages.bulma.net</a><br>
    O escriba al e-mail de alguno de los autores.</div>
  <br>
  <div class="subtitular1">QU&Eacute; ES ??</div>
</div>
<br>
<div >Le permite consultar la descripci&oacute;n de los iconos del programa.</div>
<br>
<div > 
  <div class="subtitular1"> EL MEN&Uacute; SALIR</div>
</div>
<br>
<div > 
  <div class="subtitular1"> EXIT</div>
</div>
<br>
<div >Cierra las ventanas activas en el programa y le permite terminar la ejecuci&oacute;n 
  de BulmaG&eacute;s.</div>
<HR>
<br>
<div class="titular" style="vertical-align: top;">EL ARCHIVO DE CONFIGURACION 
  BULMAGES.CONF</div>
<p>A trav&eacute;s de este archivo, puede especificar determinados par&aacute;metros 
  que afectan al comportamiento del programa.<br>
  El archivo se compone de l&iacute;neas con dos palabras. La primera palabra 
  indica la car&aacute;ter&iacute;stica a la que se desea dar valor y la segunda 
  palabra indica el valor que toma dicha caracter&iacute;stica.<br>
  <br>
  A continuaci&oacute;n se detallan todas las variables que se pueden configurar, 
  c&oacute;mo afectan al programa y los posibles valores que pueden tener.<br>
  <br>
  # Estos valores indican el fondo y la fuente de la ventana de apuntes.<br>
<div style="color: rgb(51, 51, 255);"><br>
  CONF_BG_APUNTES</div>
: Esta opci&oacute;n indica el color de fondo de la ventana de introducci&oacute;n 
de apuntes cuando el asiento esta cerrado. Como valor coge un color en formato 
RGB (#RRGGBB)<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_FG_APUNTES: 
  <div
 style="color: rgb(0, 0, 0);">Esta opci&oacute;n indica el color de texto de la 
    ventana de introducci&oacute;n de apuntes cuando el asiento esta cerrado. 
    Como valor coge un color en formato RGB.<br>
    <br>
  </div>
  CONF_BG_APUNTESA</div>
: Esta opci&oacute;n indica el color de fondo de la ventana de introducci&oacute;n 
de apuntes cuando el asiento est&aacute; abierto. Como valor coge un color en 
formato RGB.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_FG_APUNTESA</div>
: Esta opci&oacute;n indica el color de texto de la ventana de introducci&oacute;n 
de apuntes cuando el asiento est&aacute; abierto. Como valor coge un color en 
formato RGB.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_FONTSIZE_APUNTES</div>
: Esta opci&oacute;n indica el tama&ntilde;o de la fuente de la pantalla de apuntes. 
Como valor coge un numero entero positivo indicando el tama&ntilde;o de la fuente. 
<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_FONTFAMILY_APUNTES</div>
: Esta opci&oacute;n indica la tipograf&iacute;a de la fuente de la pantalla de 
apuntes. Como valor coge un nombre de fuente p.ej: courier, airal, times new roman, 
helvetica, verdana.<br>
<br style="color: rgb(51, 51, 255);">
<div style="color: rgb(51, 51, 255);">CONF_CONTRAPARTIDA_APUNTES</div>
&nbsp; : Esta opci&oacute;n indica si en la pantalla de introducci&oacute;n de 
apuntes debe aparecer la columna de contrapartida. Si desea que aparezca la contrapartida 
en la pantalla de introducci&oacute;n de aputnes ponga el valor&nbsp; YES en esta 
opci&oacute;n, en caso contrario escriba el valor NO.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_CANAL_APUNTES</div>
&nbsp;&nbsp; : Esta opci&oacute;n indica si en la pantalla de introducci&oacute;n 
de apuntes debe aparecer la columna del canal. Los valores que puede coger son 
YES y NO.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_CCOSTE_APUNTES</div>
&nbsp;&nbsp; : Esta opci&oacute;n indica si en la pantalla de introducci&oacute;n 
de apuntes debe aparecer la columna de centro de coste. Los valores que puede 
coger son YES y NO.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_CONTRAPARTIDA_EXTRACTO</div>
: Esta opci&oacute;n indica si en la ventana de extracto debe aparecer el campo 
de contrapartida, esta opci&oacute;n puede conetener valores YES y NO.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_CANAL_EXTRACTO</div>
&nbsp; : Esta opci&oacute;n indica si en la ventana de extracto debe aparecer 
el campo de canal. Esta opci&oacute;n admite los valores YES y NO<br>
<br style="color: rgb(51, 51, 255);">
<div style="color: rgb(51, 51, 255);">CONF_CCOSTE_EXTRACTO</div>
&nbsp;&nbsp; : Esta opci&oacute;n indica si en la ventana de extracto debe aparecer 
el campo de centro de coste. Esta opci&oacute;n puede coger valores YES y NO.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_CONTRAPARTIDA_DIARIO</div>
&nbsp; : Esta opci&oacute;n le permite mostrar la columna de contrapartida en 
el libro diario del programa. Esta opci&oacute;n puede coger valores YES o NO.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_CANAL_DIARIO</div>
&nbsp;&nbsp; : Esta opci&oacute;n le permite mostrar la columna de canal en el 
libro diario del programa. Esta opci&oacute;n puede contener valores YES o NO.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_CCOSTE_DIARIO</div>
<p>&nbsp;&nbsp; : Esta opci&oacute;n le permite mostrar la columna de centro de 
  coste en el libro diario del programa. Esta opci&oacute;n puede coger valores 
  YES o NO.<br>
  <br>
  <br>
  # Estos valores indican el fondo de la ventana del diario</p>
<p><br>
  <font color="#0000FF">CONF_BG_DIARIO</font>&nbsp;&nbsp; #66CCCC<br>
  <font color="#0000FF">CONF_FONTSIZE_DIARIO</font>&nbsp; 12<br>
  <font color="#0000FF">CONF_FONTFAMILY_DIARIO</font>&nbsp; Arial<br>
  <font color="#0000FF">CONF_FG_DIARIO1</font>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
  #CC0000<br>
  <font color="#0000FF">CONF_FG_DIARIO2</font>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
  #000000<br>
  <br>
  # Par&aacute;metros de configuraci&oacute;n de la ventana de Extracto</p>
<p><br>
  <font color="#0000FF">CONF_BG_EXTRACTO</font> #0099CC<br>
  <br>
  # Par&aacute;metros de configuraci&oacute;n de las ventanas de Balance</p>
<p><br>
  <font color="#0000FF">CONF_BG_BALANCE</font>&nbsp; #FFCC99<br>
  <font color="#0000FF">CONF_BG_BALANCETREE</font> #CCCC66<br>
  <br>
  #Par&aacute;metros de configuraci&oacute;n del splash y del fondo del area de trabajo</p>
<p><br>
  <font color="#0000FF">CONF_BACKGROUND</font> /usr/share/bulmages/backgrounds/universo.jpg</p> 
</p>
<p><br>
  <font color="#0000FF">CONF_SPLASH</font>&nbsp;: Esta opci&oacute;n indica la 
  ruta del archivo de imagen del splash. P. ej: /usr/share/bulmages/nousplash1.jpg<br>
  <br>
  # Par&aacute;metros de configuraci&oacute;n de la base de datos.<br>
<div style="color: rgb(51, 51, 255);">CONF_METABASE</div>
&nbsp; : Esta opci&oacute;n le permite&nbsp; especificar cu&aacute;l ser&aacute; 
la base de datos metabd del sistema. Por defecto el nombre de base de datos meta 
es metabd, bajo determinadas circunstancias a usted puede interesarle que la base 
de datos meta tenga un nombre diferente. Escriba aqui el nombre de dicha base 
de datos.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_SERVIDOR</div>
&nbsp;&nbsp; : Mediante esta opci&oacute;n puede indicar cu&aacute;l ser&aacute; 
el servidor de bases de datos con el que el programa intentar&aacute; establecer 
comunicaci&oacute;n. Por defecto esta opci&oacute;n va en localhost. Pero, si 
le interesa que las bases de datos est&eacute;n en un ordenador diferente, puede 
especificar la IP de dicho ordenador mediate esta opci&oacute;n.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_PUERTO</div>
<p>&nbsp; : Mediante esta opci&oacute;n puede indicar en qu&eacute; puerto del 
  ordenador se encuentra alojado el servidor de base de datos postgresql, por 
  defecto suele ser el 5432<br>
  <br>
  # Par&aacute;metros de configuraci&oacute;n de programas</p>
<p><br>
  <font color="#0000FF">CONF_EDITOR</font>&nbsp;: Esta opci&oacute;n indica cu&aacute;l 
  es el editor de textos que se va a utilizar para mostrar los listados. p ej: 
  kedit<br>
</p>
<p><font color="#0000FF">CONF_NAVEGADOR</font>&nbsp;: Esta opci&oacute;n indica 
  cu&aacute;l es el navegador html que se va a utilizar para mostrar los listados 
  y p&aacute;ginas auxiliares p ej:mozilla.<br>
</p>
</body>
</html>
