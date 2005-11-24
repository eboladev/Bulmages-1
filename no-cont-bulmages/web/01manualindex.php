<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"><html><head><title>Manual de BulmaGes</title>

<meta content="text/html; charset=iso-8859-1" http-equiv="Content-Type">
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
</style></head>

<body>
<div align="center">
  <p><span class="titular">MANUAL DEL PROGRAMA BULMAG&#201;S </span><br>
    <br>
    Juliol-Agost 2003 (C) Cristina Marco Hern&#225;ndez<br>
	Traducció: Josep Antoni Ruiz
    <br>
  </p>
  </div>
<div class="titular">INTRODUCCI&#211;</div>

  <p>En els mesos de Juny i Juliol de 2001 va sorgir en la llista de distribuci&#243;
  de Bulma (<a href="http://bulmalug.net/">http://bulmalug.net</a>), la idea de
  realitzar programes de gesti&#243; per a empreses desenvolupats en programari
  lliure. L'objectiu era clar, fer possible l'&#250;s de Linux en el
  dia a dia d'una empresa. Per a aix&#242; cal que les empreses contin amb
  programari de qualitat que els permeti abordar totes les tasques que ja tenen informatitzades.</p>
<p>Amb el temps, va ser escollit com primer candidat a ser implementat un programa
  dedicat a la comptabilitat pels seg&#252;ents motius:</p>
      <p>
</p><li> &#201;s un programa a&#239;llat que influeix a un &#250;nic departament; no &#233;s
  vital per al funcionament de l'empresa i no &#233;s necessari migrar tot el
  paquet inform&#224;tic de l'empresa per a utilitzar BulmaG&#233;s: n'hi ha prou amb
  un ordinador</li>
<p></p>
      <p>
</p><li> Els requisits per a un programa comptable estan b&#233; definits i s&#243;n 
  pr&#224;cticament els mateixos per a qualsevol empresa, amb el que el programa 
  tindr&#224;, probablement, majors possibilitats de ser utilitzat que qualsevol 
  altre candidat dedicat a altre sector empresarial.</li>
<p></p>
      <p>
</p><li> No existia cap programa que es dediqu&#233;s a la comptabilitat. 
  Actualment, encara existeixen molt poques alternatives reals. I la major part dels 
  projectes dedicats a aquest tema han quedat oblidats.</li>
<p></p>
      
<p>Despr&#233;s de dos anys d'aparent immobilitat del projecte, per fi, en novembre
  de 2002 es va comen&#231;ar la codificaci&#243; del programa.</p>
    <br>
    <div class="titular">INSTAL&#183;LACI&#211; DE BULMAGES</div>

    <br>

    <br><div style="font-weight: bold;" class="subtitular1">A.- Instal&#183;laci&#243; R&#224;pida 
      </div><br>
      <br>
<li>El primer pas per a instal&#183;lar el programa &#233;s descarregar-lo. Pot obtenir un
  arxiu <strong>tgz</strong> amb tots els arxius necessaris per al programa
  en <a href="http://bulmages.bulma.net/">http://bulmages.bulma.net</a></li>
<br>
      <p>
</p><li>A continuaci&#243; descomprimeixi l'arxiu <strong>tgz</strong> en un directori</li>
<p></p>
      <p>
</p><li>Executi com a <strong>root</strong> l'script d'instal&#183;laci&#243; <strong>install-bulmages</strong></li>
<p></p>
      
<p>
</p><li>Comprovi que el directori<strong> /usr/local/bin</strong> es troba en 
  el path.</li>
<p></p>
<p> 
</p><li>Per a arrencar el programa n'hi ha prou amb escriure <strong>bulmages</strong>.</li>
<p></p>
      
<p class="subtitular2">Requisits del programa:</p>
<p>Per al correcte funcionament del programa es requereix:</p>
      <li>postgresql</li>
      <li>kde3.x </li>
      <li>libpq</li>

<p></p><div class="destacado1">Nota:</div>
<br>
&#160;&#160;&#160; Revisi l'apartat de <em>Configuraci&#243; del Motor de la Base
de Dades PostgreSQL</em> d'aquesta mateixa secci&#243; per a comprovar que
t&#233; la base de dades correctament instal&#183;lada.<br>
<br><p></p>
<div class="subtitular1">B.- Instal&#183;laci&#243; Manual</div>
<p>En aquest punt es detallaran tots els requisits necessaris per a posar
  en funcionament el programa, sense haver de fer &#250;s de l'script d'instal&#183;laci&#243;.
  D'aquesta forma ser&#224; m&#233;s f&#224;cil comprendre els components
  del programa.</p>
<p><br>
  Aquesta instal&#183;laci&#243; &#233;s una mica m&#233;s laboriosa per&#242; permet tenir major 
  control sobre els components de l'aplicaci&#243;. No cal executar 
  l'script d'instal&#183;laci&#243; installbulmages perqu&#232; funcioni, per&#242; si
  cal tenir acc&#233;s de root en la m&#224;quina en la qual es vagi a instal&#183;lar
  el programa.<br>
</p>
      
<h4>
  <div class="subtitular2">&#160; B1.- Instal&#183;laci&#243; dels serveis. </div>
</h4>
      Partint que el <strong>servidor de postgresql</strong> ja est&#224; 
instal&#183;lat i correctament configurat, per a instal&#183;lar les bases de dades
requerides per a qu&#232; funcioni Bulmag&#233;s haur&#224; d'accedir al directori
<strong>dbmodels</strong> i buscar en ell tres arxius de bases de dades 
(<strong>pgdump</strong>). Si el seu usuari no est&#224; donat d'alta en postgres 
pot fer-ho mitjan&#231;ant els seg&#252;ents ordres (suposant que l'usuari <em>tborras</em>
no t&#233; permisos per a accedir a bases de dades de postgres):<br>

     <br> <p class="codigo">
      /root/:# su postgres<br style="font-style: italic;">
      /home/postgres:$ createuser tborras</p>
      <br>
	  La primera ordre permet que el sistema entre en el compte de superusuari de
postgres, d'aquesta forma es poden executar ordres en la base de dades.<br>
La segona ordre crea l'usuari <em>tborras</em>, perqu&#232; aquest pugui
accedir a la base de dades.<br>
      <br>
Despr&#233;s d'aquestes dues ordres el sistema formular&#224; algunes preguntes sobre si
vol que l'usuari pugui crear noves bases de dades (Resposta: SI).<br>
      <br>
Fet aix&#242; ja es poden crear les bases de dades del programa mitjan&#231;ant les ordres:<br>
      <br>
      <p class="codigo">/home/tborras:$ createdb bulmages<br>
     /home/tborras:$ createdb metabd<br>
	 /home/tborras:$ createdb bgplangcont</p>
      <br>
Si tot ha funcionat b&#233;, el sistema mostrar&#224; el missatge <strong>CREATE
DATABASE</strong> indicant que la base de dades ha estat creada.<br>
      <br>
Despr&#233;s d'aix&#242; es poden bolcar les dades que se subministren amb la instal&#183;laci&#243;
del programa<br>
      <br>
      <p class="codigo">/home/tborras:$ psql bulmages
&lt; bulmages.pgdump<br style="font-style: italic;">
      /home/tborras:$ psql metabd
&lt; metabd.pgdump<br style="font-style: italic;">
      /home/tborras:$ psql
bgplangcont &lt; bgplangcont.pgdump</p>
      <br>
Aquestes ordres han de generar bastant sortida de dades en pantalla.<br>
Si tot ha anat correctament, i no ha aparegut cap missatge d'error,
en el sistema estaran creades les bases de dades que va a fer servir el programa.<br>
      <br>
Si s'ha produ&#239;t algun error, o vol tornar a instal&#183;lar, sempre es pot
esborrar la base de dades amb l'ordre:<br>
      <br>

<p class="codigo">/home/tborras:$ dropdb bulmages<br>
/home/tborras:$ dropdb metabd<br>
/home/tborras:$ dropdb bgplangcont</p>
<br style="font-style: italic;">
      <br>
Llegeixi el manual <em>de refer&#232;ncia de postgreSQL</em> (<a target="_blank" href="http://www.postgresql.org/">http://www.postgresql.org</a>)
per a m&#233;s informaci&#243; sobre l'administraci&#243; de bases de
dades.<br>
      <br>
      <div class="subtitular2">B2.- Instal&#183;laci&#243; del client de comptabilitat</div>
<p> Per a instal&#183;lar el client de comptabilitat l'&#250;nica cosa que cal fer
  &#233;s copiar una s&#232;rie d'arxius en determinades ubicacions.</p>
<p>1. Copiar <strong>bulmages.conf</strong> en el directori <strong>etc/</strong><br>
  Aquest &#233;s l'arxiu de configuraci&#243; del programa i ha de tenir permisos
  de lectura.<br>
  <br>
  2. Copiar<strong> bulmages</strong>, <strong>borraremp</strong>, <strong>cargaemp</strong>,
  <strong>guardaemp</strong> i <strong>nuevaemp</strong> en <strong>/usr/local/bin</strong><br>
  Aquests arxius s&#243;n els executables i ha de tenir permisos d'execuci&#243;.
  A m&#233;s, el directori<strong> /usr/local/bin</strong> ha d'estar
  <strong>en el PATH</strong> perqu&#232; pugui ser executat.<br>
  <br>
  3. Copiar la resta d'arxius en <strong>/usr/share/bulmages</strong><br>
  Aquests s&#243;n els arxius auxiliars del programa i han de tenir permisos de lectura.<br>
  <br>
  &#201;s possible que la part client i la part servidor estiguin allotjades en
  distintes m&#224;quines. Fins i tot pot tenir diversos clients atacant a una
  mateixa base de dades. Per a fer-ho existeixen dos par&#224;metres en l'arxiu&#160;<strong>
  /etc/bulmages.conf</strong> que indiquen a quin servidor de bases de dades
  ha de dirigir-se el programa. Aquests par&#224;metres s&#243;n <strong>CONF_SERVIDOR</strong>
  i <strong>CONF_PUERTO</strong>, on es pot indicar a quin servidor
  de bases de dades i port de connexi&#243; ha de fer el programa les peticions
  de base de dades.<br>
  <br>
</p>
<h4 class="subtitular1">C.- Configuraci&#243; del motor de bases de dades
PostgreSQL:</h4>

<p>La major complicaci&#243; que pot sorgir amb el programa &#233;s la correcta
  instal&#183;laci&#243; de les bases de dades.</p>
      <p class="subtitular2"><strong>C1.-Permisos dels usuaris:</strong></p>

<p>Postgres treballa amb diversos m&#232;todes d'autenticaci&#243;. Aquestes
  opcions es configuren en l'arxiu<strong> pg_hba.conf</strong>. Aquesta &#233;s
  la principal causa de problemes amb el programa. (En <em>Debian</em> aquest arxiu
  es troba en el directori<strong> /etc/postgresql/</strong>)</p>

<p>L'opci&#243; d'autenticaci&#243; <strong>trust</strong> permet
  que tots els usuaris tinguin acc&#233;s a la base de dades amb el que no haur&#224;
  problemes d'autenticaci&#243; i tots els usuaris podran utilitzar sense
  restriccions les bases de dades. Així que, si es disposa el tipus d'acc&#233;s trust
  i les bases de dades de l'aplicaci&#243; han estat creades, el programa funcionar&#224;
  sense problemes.</p>

<p>L'opci&#243; d'autenticaci&#243; <strong>ident sameuser</strong>
  nom&#233;s permet l'acc&#233;s a la base de dades a aquells usuaris que han
  estat creats pr&#232;viament. Aquesta opci&#243; &#233;s m&#233;s segura ja que nom&#233;s
  els usuaris que tinguin compte en l'ordinador tindran acc&#233;s a les bases
  de dades. Per a utilitzar <strong>Bulmag&#233;s</strong> amb aquest model d'autenticaci&#243;
  haur&#225; d'assegurar-se que els usuaris que van
  a usar el programa han estat donats d'alta en la base de dades. Per a aix&#242; pot
  fer servir l'ordre:</p>
      <p class="codigo">/home/postgres:$
create user xxxxxxxx</p>

<p>on<strong> xxxxxxxxx </strong>&#233;s <strong>el login de l'usuari</strong>
  al que se li atorgaran permisos d'acc&#233;s a la base de dades.</p>
      <p></p>





<hr>



    <br>
      <div class="titular">EXECUCI&#211; DEL PROGRAMA BULMAGES</div>

    <p>Una vegada que el programa ha estat correctament configurat, el seg&#252;ent pas
  &#233;s executar-ho. </p>
      <div style="text-align: center;"><img style="width: 449px; height: 357px;" alt="splash de la version 0.3.1" title="" src="imgmanual/splash.png"></div>

<p>El programa s'executa mitjan&#231;ant l'ordre <strong>bulmages</strong>.&#160;
  Una vegada executat apareixer&#224; una pantalla d'entrada on ha de seleccionar
  l'empresa sobre la qual vagi a treballar. El programa sol&#183;licitar&#224; un login
  i un password. Per defecte el programa t&#233; l'usuari <strong>bulmages</strong>
  predefinit i el seu password &#233;s <strong>bulmages</strong>.</p>






<hr>
<br>



  <div class="titular">PRIMERS PASSOS AMB EL PROGRAMA</div>
<br>
  <div class="subtitular1">INTRODUCCI&#211; AL PROGRAMA</div>
<p> <strong>BulmaG&#233;s</strong> &#233;s un projecte dedicat a resoldre la tasca
  de la comptabilitat d'empresa d'una forma senzilla i elegant i sempre sota
  el punt de vista del <strong>Programari Lliure</strong>. </p>
<p>El programa est&#224; dissenyat pensant en l'usuari final. Intenta
  que la majoria de les tasques de comptabilitat siguin completament accessibles i
  f&#224;cils de portar a terme mitjan&#231;ant un sistema molt intu&#239;tiu d'utilitzar
  el programa.</p>
<p><strong>BulmaG&#233;s</strong> ofereix un interf&#237;cie amigable i totalment configurable,
  amb el qual pret&#233;n facilitar la comprensi&#243; del programa, fer l'entorn
  de treball amigable i confondre el menys possible a l'usuari permetent que
  opcions susceptibles de no ser utilitzades puguin desapar&#232;ixer de la pantalla.<br>
  <br>
</p>
<div class="subtitular1">LA PANTALLA D'INICI: OBRIR EMPRESA</div>

    <br>
      <br>

            <br>
            
<div>La pantalla d'inici <strong></strong> permet seleccionar l'empresa <strong></strong> 
  amb la qual es desitgi treballar. Aquesta pantalla inclou el nom que se li hagi
  proporcionat a l'empresa, l'exercici al que es refereix i el nom de l'arxiu
  en el qual es guardaran les dades relatives a l'exercici en curs.</div>


          <br>
            <br>
Per a comen&#231;ar a utilitzar <strong>BulmaG&#233;s</strong> ser&#224; necessari
<strong>obrir</strong> una <strong>empresa</strong>. <strong>BulmaG&#233;s</strong>
inclou per defecte un pla comptable est&#224;ndard, una empresa de proves i
la configuraci&#243; d'un usuari "bulmages". Per tant, per a accedir per
primera vegada al programa ser&#224; necessari seleccionar l'empresa <strong>de proves
</strong> (<strong>bulmages</strong>) i l'usuari per defecte. <br>



<p>Els camps <strong>login</strong> i <strong>password</strong> romandran
  buits al menys que s'hagin especificat uns anteriorment. El programa
  t&#233; creat, per defecte, un usuari que pot accedir a totes les empreses.
  vost&#232; pot accedir al sistema sota aquest usuari. Per a aix&#242; pot usar els seg&#252;ents
  login i password:<br>
  </p>

<p><font color="#0000ff">LOGIN</font>: bulmages<br>
  <font color="#0000ff">PASSWORD</font>: bulmages<br>
</p>
<hr>
<p>&#160; </p>
En el cas que ja s'hagin creat diferents empreses, per a comen&#231;ar a operar
amb el programa, haur&#224; de seleccionar l'empresa <strong> </strong>amb
la que vulgui treballar i introduir despr&#233;s el <strong>login </strong> i el
<strong>password</strong> d'un usuari que tingui acc&#233;s a aquesta empresa. <br>
<br>
      <br><div class="titular"> PANTALLA PRINCIPAL</div>

    <br>
      <br>

<div>La pantalla principal de <strong>BulmaG&#233;s</strong> li ofereix les
  opcions generals del programa en dues modalitats: el <em>men&#250; </em>superior
  i la barra <em>d'acc&#233;s r&#224;pid</em> inferior. Dintre de la pantalla
  general s'obren les pantalles corresponents a les opcions espec&#237;fiques
  del programa (per exemple, la pantalla d'Apunts <strong>Comptables</strong>,
  la <strong>Gesti&#243; de Seients</strong>, etc.)</div>


    <br>
      <br>

<div>A l'iniciar una sessi&#243; de treball en una <strong>empresa</strong>
  seleccionada en <strong>BulmaG&#233;s</strong>, apareix la pantalla principal
  que obre per defecte les seg&#252;ents subpantallas:</div>


    <br>
      <br>
      <div>- INTRODUCCION D'APUNTS
COMPTABLES </div>


    <br>
      <br>
      <div>- LLIBRE DIARI D'APUNTS</div>


    <br>
      <br>

<div>- LLIBRE MAJOR o EXTRACTE DE COMPTES</div>


    <br>
      - BALAN&#199; DE COMPTES.<br>


    <br>
      - BALAN&#199; JER&#192;RQUIC.<br>


<hr>

  <br>
    <div class="titular">GESTI&#211; D'EMPRESES</div>


<br>
<div><strong>BulmaG&#233;s</strong> pot treballar amb tantes empreses com
  l'usuari desitgi, assignant aix&#237; a cada empresa l'exercici econ&#242;mic
  que correspongui. El men&#250; <strong>empresa</strong> consta de les seg&#252;ents
  opcions:</div>

  <br>
    <div style="vertical-align: top;" class="subtitular1">OBRIR EMPRESA</div>

  <br>
Mitjan&#231;ant aquesta opci&#243; es pot crear el seient d'obertura d'una empresa.
Per a crear el seient d'obertura d'una empresa cal que l'&#250;ltim
seient introdu&#239;t sigui el seient de tancament de l'empresa.<br>
 <br>
<div style="vertical-align: top;" class="subtitular1">TANCAR EMPRESA</div>


<p> Mitjan&#231;ant aquesta opci&#243; es crea el seient de tancament de l'empresa.<br>
</p>
<p>&#201;s possible generar tants seients de tancament i d'obertura com es desitgi durant
  l'exercici comptable.</p>
<div style="vertical-align: top;" class="subtitular1">REGULARITZAR EMPRESA</div>

  <br>
Regularitzar una empresa &#233;s el seient previ al seient de tancament. Amb aquest seient
es desplacen els resultats de l'exercici al compte de perdues i guanys.<br>

  <br>
    <div class="subtitular1">NOVA EMPRESA</div>

  <br>
<div>Permet afegir una nova empresa amb el seu corresponent exercici
  comptable. Per a crear una nova empresa &#233;s necessari accedir al men&#250;
  <strong>empresa</strong> i seleccionar l'opci&#243; de nova <strong>empresa</strong>.</div>


<div align="center"><br>
  <img style="width: 595px; height: 422px;" alt="nueva empresa" title="" src="imgmanual/nempresa.png">
</div>
<p> AL crear una nova empresa, el programa sol&#183;licita el nom de l'empresa,
  el nom de la base de dades que la va a contenir i l'exercici que es va
  a introduir.<br>
  En la mateixa finestra, es poden assignar els diferents usuaris que tenen permisos
  d'acc&#233;s a l'empresa i el tipus d'acc&#233;s (lectura o modificaci&#243;).<br>
  La creaci&#243; d'una empresa implica la creaci&#243; d'una base de
  dades nova. Per tant, l'usuari que executa el programa ha de tenir privilegis
  per a crear bases de dades en postgres.</p>

  <div class="subtitular1">ESBORRAR EMPRESA</div>

  <br>
Li permet esborrar una empresa existent de forma definitiva, incloent tots
les dades que s'hagin introdu&#239;t fins el moment.<br>
Per a esborrar una empresa primer &#233;s necessari seleccionar l'empresa que es desitgi
esborrar. Mitjan&#231;ant l'opci&#243; <strong>esborrar empresa </strong>del men&#250; l'empresa
oberta ser&#224; esborrada automàticament.<br>

  <br>
<div class="subtitular1">CANVIAR EMPRESA</div>

  <br>
<div>Mitjan&#231;ant aquesta opci&#243; pot canviar l'empresa activa a altra empresa
  diferent a la seleccionada en <strong>la pantalla d'inici</strong>: <strong>obrir
  empresa. </strong>Aix&#242; ser&#224; possible sempre que l'empresa que es vagi
  a seleccionar hagi estat creada anteriorment. Pot canviar d'empresa tantes
  vegades com desitgi sempre que no estigui a la meitat del proc&#233;s d'introducci&#243;
  de dades.<br>
        <br>
  La finestra de canvi d'empresa &#233;s la mateixa que la d'obertura d'empresa, &#233;s a dir
  , la finestra que apareix despr&#233;s d'executar el programa. Tamb&#233; es pot
  canviar l'usuari mitjan&#231;ant aquesta finestra.<br>
        </div>


<div align="center"><img style="width: 398px; height: 234px;" alt="cambiar empresa" title="" src="imgmanual/cambiaempresa.png"></div>

<br><br>

<div class="subtitular1"> PROPIETATS EMPRESA. <img height="25" width="24" src="imgbotones/boton_propempresa.gif"></div>

<p>Permet consultar i modificar els par&#224;metres especificats per a l'empresa <strong>
  activa</strong>. </p>

<div align="center"><img style="width: 591px; height: 410px;" alt="propiedades empresa" title="" src="imgmanual/propempresa.png"> </div>

<p> Mitjan&#231;ant aquesta pantalla pot canviar paràmetres de configuraci&#243; de l'empresa
  Aquests paràmetres s&#243;n: el nombre de d&#237;gits que per
  defecte tenen els comptes, el compte de regularitzaci&#243; de l'empresa
  i els permisos que tenen els usuaris sobre l'empresa. </p>
    <div class="subtitular1">GUARDAR EMPRESA</div>

  <br>
Permet fer una c&#242;pia de seguretat de l'empresa activa. La totalitat de les
dades d'aquesta empresa s&#243;n guardats en un arxiu en la ubicaci&#243; que s'indiqui
<br>


  <br>
    <div class="subtitular1">CARREGAR EMPRESA</div>


<br>
 Opci&#243; per a restaurar una
        c&#242;pia de seguretat realitzada amb anterioritat de l'empresa seleccionada
        o per a afegir les dades d'altra empresa.



	<hr>

<br>
    <br>

    <big><big><big><div style="color: rgb(0, 0, 153); font-weight: bold;"><div class="titular"><img height="24" width="24" src="imgbotones/boton_usuarios.gif">GESTIÓ
      D'USUARIS</div></div></big></big></big>

    <br>
Mitjan&#231;ant el bot&#243; <img height="24" width="24" src="imgbotones/boton_usuarios.gif">
del <strong>toolbar</strong> o l'opci&#243; del men&#250; Empresa pot
gestionar els usuaris: Crear, esborrar i modificar els usuaris que tenen acc&#233;s
al programa.<br>
<br>
      <div style="text-align: center;"><img style="width: 608px; height: 373px;" alt="gestion de usuarios" title="" src="imgmanual/usuarios.png"><br>
      </div>


    <br>
En la part de l'esquerra de la finestra pot seleccionar l'usuari <strong></strong>
que vulgui editar i modificar-lo amb els camps situats a la dreta. <br>
Si tenint un usuari seleccionat prem sobre el bot&#243; <img height="24" width="24" src="imgbotones/boton_borrar.gif">,
podra <strong>eliminar</strong> l'usuari seleccionat.<br>
Si prem sobre el bot&#243; <img height="24" width="24" src="imgbotones/boton_nuevo.gif">
apareixer&#224; un<strong> usuari nou</strong> en la part esquerra que despr&#233;s
podr&#224; modificar amb les dades que precisi.<br>



<br>
<hr>



 <br>
    <br><div class="titular"><img height="24" width="24" src="imgbotones/boton_cuenta.gif">
    PLA COMPTABLE</div>
  <br><br>
  <center>
  <img align="middle" style="width: 597px; height: 454px;" alt="plan de cuentas" src="imgmanual/plancuentas.png"><br>
  </center>
   <br>
  <br>

<p>Mitjan&#231;ant aquesta pantalla es pot <strong>veure</strong>, <strong>modificar</strong>
  i <strong>esborrar</strong> totes els comptes i subcomptes del pla comptable de l'empresa
  </p>


<p>El pla de comptes &#233;s accessible mitjan&#231;ant el bot&#243; <img height="24" width="24" src="imgbotones/boton_cuenta.gif">
  i mitjan&#231;ant l'opci&#243;<strong> Pla Comptable</strong> del men&#250; <strong>Compte</strong>.</p>


<p>El pla de comptes mostra el pla comptable amb el qual treballa l'empresa activa.
  <strong>Bulmag&#233;s</strong> se subministra amb <strong>el Nou Pla General
  Comptable B&#224;sic</strong> per defecte, per&#242; ofereix la possibilitat de modificar-lo
  i personalitzar-lo. D'aquesta forma, cada empresa pot operar amb un pla comptable
  personalitzat.<br>
  <br>
  La pantalla presenta dues pestanyes:
</p><ul>

  <li>En la primera (<strong>Pla Comptable</strong>) es pot veure un arbre
    de comptes de l'empresa.</li>
  <br>


  <li> En la segona (<strong>Subcomptes</strong>) apareix un llistat de les subcomptes
    de l'empresa.</li>
		  </ul>



<p>Per a crear un <strong>nou compte</strong> s'utilitza el bot&#243; <img height="24" width="24" src="imgbotones/boton_nuevo.gif">
  AL seleccionar un compte, prement aquest bot&#243;, el compte que creeu es
  col&#183;locar&#224; automàticament com "compte fill" del compte
  seleccionat.</p>


<p>Pot<strong>editar</strong> qualsevol compte fent doble clic en el compte
  desitjat. Tamb&#233; pot editar-la seleccionant-la i prement sobre
  el bot&#243; <img height="24" width="24" src="imgbotones/boton_editar.gif">.</p>
<p>Pot <strong>esborrar</strong> qualsevol compte (que no tingui "comptes fills",
  ni apunts comptables apuntats en ell) seleccionant-lo i prement sobre el
  bot&#243; <img height="24" width="24" src="imgbotones/boton_borrar.gif"></p>


    <div class="subtitular1">EDICI&#211; DE COMPTES. <img height="24" width="24" src="imgbotones/boton_editar.gif"></div>

    <br><br>
	<center><img style="width: 600px; height: 381px;" alt="edicion de cuentas" title="" src="imgmanual/edicuenta.png"></center><br>


    <br>
AL crear un compte nou, o editar un compte existent, apareix la finestra del
dibuix superior. En ella es mostren totes les dades del compte.<br>
La pestanya <strong>Altres Dades</strong> cont&#233; informaci&#243; addicional
del compte, com comptes bancaris, noms, CIF, i-mails i dades de contacte
que es puguin necessitar per a la gesti&#243; d'aquest compte.<br>


<br>
<hr>

    <br>
      <br>

<div class="titular"><img height="24" width="24" src="imgbotones/boton_intapunts.gif">
  <img height="24" width="24" src="imgbotones/boton_asientos.gif">SEIENTS / INTRODUCCI&#211;
  D'APUNTS</div>

	<br><br>
      <center><img style="width: 853px; height: 338px;" alt="introduccion de apuntes." title="" src="imgmanual/intapunts.png"></center>


<p><br>
  Aquesta pantalla permet la introducci&#243; de <strong>seients</strong> i
  <strong>apunts comptables</strong>. <br>
  <br>
  La <strong>gesti&#243; de seients</strong> &#233;s una de les parts principals
  del programa ja que les dades que s'introdueixin seran les que proporcionin
  despr&#233;s la visi&#243; exacta de la comptabilitat. &#201;s a partir d'aquestes
  dades dels quals <strong>BulmaG&#233;s </strong> obt&#233; la informaci&#243;
  necess&#224;ria per a elaborar els diferents informes o llistats. Per exemple, el llistat
  del <strong>diari</strong> o el llibre <strong>de major</strong>..
  <br>
  <br>
  La pantalla d'introducci&#243; <strong>de seients</strong> s'obre per
  defecte a l'inicialitzar el programa situant la pantalla per defecte en l'&#250;ltim
  <strong>número de seient</strong> introdu&#239;t. D'aquesta forma es facilita
  la inserci&#243; de seients seguint un ordre correlatiu.<br>
  En el cas que no hagi cap seient introdu&#239;t pr&#232;viament (per
  exemple en empreses reci&#233;n creades) <strong>BulmaG&#233;s</strong>
  obrir&#224; la <strong>Gesti&#243; de Seients</strong> en el número
  1 (u). <br>
  Per a modificar un seient existent caldr&#224; comen&#231;ar per obrir el seient
  mitjan&#231;ant el bot&#243; obrir seient <img height="24" width="24" src="imgbotones/boton_abreasiento.gif">.</p>
<p> Una vegada en la pantalla, s'ha de pr&#233;mer el bot&#243; de <strong>seg&#252;ent
  </strong>(dibuixat com una fletxa). AL pr&#233;mer aquesta opci&#243;, <strong>BulmaG&#233;s</strong>
  detectar&#224; que es desitja crear un seient nou i preguntar&#224;: "No
  existeix un seient amb el número que ha proporcionat, &#191;desitja crear
  un?". Aix&#242; significa que el programa crear&#224; un nou seient amb un
  número correlatiu a l'anterior.</p>


<p>Una vegada obert un nou seient, el fons de la pantalla canvia a color blanc
  i el programa permet la introducci&#243; de dades en els diferents camps
  disponibles. </p>


   <p>La introducci&#243; de seients pot realitzar-se mitjan&#231;ant la introducci&#243;
  lliure de les dades directament en la pantalla principal. Per a agilitar el proc&#233;s,
  el programa permet canviar d'una opci&#243; a una altra amb el teclat i sense
  la necessitat de rec&#243;rrer al ratol&#237;.</p>



<p>Bulmages li ofereix tamb&#233; la possibilitat d'introduir els seients
  en dues modalitats diferents:</p>


<p>Els <strong>Seients Simples</strong> consisteixen a inserir les dades en
  el programa apunt per apunt.</p>


<p>Els <strong>Seients Intel&#183;ligents</strong> o <strong>Predefinits</strong>
  s&#243;n aquells que, havent establert amb anterioritat les seves propietats, faciliten
  la introducci&#243; de seients repetits o usuals en la comptabilitat, com
  per exemple pagaments mensuals a prove&#239;dors, gestions usuals amb el banc, etc.
  Vost&#232; pot crear tants <strong>seients intel&#183;ligents </strong>com desitgi establint
  els valors que consideri oportuns per a cadascun d'ells.</p>


<p>Hi ha diversos par&#224;metres d'aquesta finestra que s&#243;n cofigurables a trav&#233;s
  de l'arxiu de configuraci&#243; del programa (veure secci&#243; bulmages.conf).
  Podr&#224; canviar la tipografia i els colors de la font. Tamb&#233;
  podr&#224; elegir si mostrar o no mostrar determinats camps.</p>


<div class="subtitular1">NAVEGACI&#211; A TRAV&#201;S DEL SISTEMA DE SEIENTS.
  <img height="24" width="113" src="imgbotones/boton_navegacion.gif"></div>


<p>Pot navegar a trav&#233;s dels seients introdu&#239;ts mitjan&#231;ant els <strong>botons
  de fletxes</strong> en les toolbars (barres d'eines). D'aquesta forma pot
  accedir al bot&#243; seg&#252;ent i anterior, al primer i a l'ultim seient.
  Segons el seu ordre de creaci&#243;.</p>

    <div class="subtitular1">FILTRAT DE SEIENTS. <img height="24" width="24" src="imgbotones/boton_filtrar.gif"></div>


<p>Pot interessar-li treballar amb una s&#232;rie de <strong>seients que compleixin unes
  caracter&#237;stiques determinades</strong>. Pot ser que el seu treball sigui m&#233;s
  senzill si mitjan&#231;ant la navegaci&#243; nom&#233;s es presenten aquells
  seients que compleixin unes caracter&#237;stiques espec&#237;fiques. Per a aix&#242;,
  pot establir un filtre que faci que nom&#233;s es presentin els seients
  que compleixin determinades caracter&#237;stiques amb el bot&#243; <img height="24" width="24" src="imgbotones/boton_filtrar.gif">
  situat en les toolbars.</p>
        <p>Immediatament apareixer&#224; la finestra seg&#252;ent que li permet establir
        els criteris del filtre.</p>

<p>Si t&#233; un filtre activat i desitja llevar-ho, en aquesta finestra t&#233; un bot&#243;
  de <strong>reset</strong> que desactiva tots els paràmetres del filtre i li
  mostrar&#224; tots els seients existents.</p>
		 <br>
		 <div style="text-align: center;"><img style="width: 608px; height: 203px;" alt="filtrado de asientos." title="" src="imgmanual/filtroasiento.png"></div>

   <br>  <br> <div class="subtitular1">SEIENTS SIMPLES</div>

    <br>
<div>Per a introduir seients simples tan sols és necessari <strong>obrir
  un seient</strong> o <strong>crear un nou </strong>tal com s'ha explicat
  anteriorment.<br>
  Una vegada que el programa li deixa modificar / introduir dades, la pantalla que
  mostra el seient presenta tots els apunts que componen el mateix en forma
  de files d'una taula. Cada fila es correspon amb un apunt.<br>
          Cada apunt es compon dels seg&#252;ents camps:<br>
          <br>
          <div style="font-weight: bold; color: rgb(51, 51, 255);">DATA:</div>
          La data de l'apunt indica la data<strong> que es realitza la transacci&#243;
  econ&#242;mica</strong>. La data pot introduir-se de distintes formes:
  <p>- Escrivint la data en qualsevol dels seg&#252;ents formats: DDMMAA,
    DDMM, DDMMAAAA, DD/MM/AA, DD/MM/AAAA. AL pr&#233;mer enter (despr&#233;s d'haver introdu&#239;t
    la data), es canviar&#224; el format d'aquesta al format est&#224;ndard
    DD/MM/AAAA.</p>

  <p> -Una altra forma d'introduir la data &#233;s prement la <strong>tecla +</strong>
    a l'editar aquest camp. Autom&#224;ticament apareixer&#224; una finestra
    amb un <strong>calendari</strong> en el qual podr&#224; seleccionar la data
    desitjada. Aquesta mateixa opci&#243; apareix al pr&#233;mer amb el bot&#243; dret
    sobre la data i seleccionant del men&#250; contextual l'opci&#243;
    "<strong>Seleccionar Valor</strong>"</p>
          <div style="text-align: center;"><img style="width: 198px; height: 173px;" alt="calendario" title="" src="imgmanual/calendario.png"></div>

  <p>Editant el camp de data, si prem sobre la <strong>tecla * </strong>autom&#224;ticament
    col&#183;locar&#224; en el camp que est&#224; editant el mateix valor que el
    camp de data anterior.</p>
          <div style="color: rgb(51, 51, 255); font-weight: bold;">SUBCOMPTE:</div><div style="font-weight: bold;"> </div>
  La introducci&#243; de comptes pot fer-se de diverses formes:
  <br><br>
  <li> Escrivint el número de compte mitjan&#231;ant el <strong>teclat</strong>.
    Tamb&#233; pot introduir el compte de forma <strong>abreujada</strong>,
    el compte 1000004 pot introduir-se de la forma "1.4," i el compte
    1000000 pot introduir-se de la forma "1."&#160; El sistema&#160; estendr&#224;
    el <strong></strong> . de forma que el número de compte tingui el nombre
    de d&#237;gits que s'usen per defecte en la comptabilitat de l'empresa.<br>

  </li><li>Una altra forma d'introduir comptes &#233;s prement la<strong> tecla + </strong>o
    l'opci&#243; <strong>elegir valor</strong> en el men&#250; contextual.
    Directament apareixer&#224; la finestra del pla de comptes, amb l'excepci&#243;
    que al fer doble clic sobre un compte aquest apareixer&#224; en
     la casella que est&#224; editant.<br>

  </li><li> Editant el camp de <strong>subcompte</strong>, si prem sobre la <strong>tecla
    *</strong> autom&#224;ticament es traslladar&#224; al camp que est&#224;
    editant el mateix valor que el camp de subcompte anterior. (Noti que les
    tecles seleccionades serveixen per a fer la introducci&#243; sense deixar
    d'utilitzar el teclat num&#232;ric).<br>

  </li><li> Si ho desitja, pot accedir de forma <strong>r&#224;pida</strong> a veure
    l'extracte d'una subcompte que estigui visualitzant en la finestra d'apunts
    Per a aix&#242; el seient ha d'estar tancat. Prement amb <strong>el bot&#243;
    dret</strong> sobre el compte del qual vulgui veure el seu extracte pot seleccionar
    l'opci&#243; <strong>veure extracte</strong>. A m&#233;s, el programa
    li permet escollir entre veure un <strong>extracte total</strong>, un <strong>extracte
    del mes</strong> que est&#224; introdu&#239;t l'apunt o un <strong>extracte
    del dia</strong> d'introducci&#243; de l'apunt.<br>
    <br>
    <br>
    <div style="color: rgb(51, 51, 255); font-weight: bold;">CONTRAPARTIDA:</div>
    <div style="font-weight: bold;"> </div>
    (Pot ser que en la seva configuraci&#243; del programa no aparegui el camp de
    contrapartida, veure secci&#243; de bulmages.conf). <br><br>
    La introducci&#243; de contrapartida &#233;s similar en funcions a la introducci&#243;
    de subcompte. <br>
    <br>
    &#160;&#160;&#160; La contrapartida &#233;s <strong>semi-autom&#224;tica</strong>,
    el programa sempre elegeix de forma intel&#183;ligent una contrapartida per a tot
    apunt introdu&#239;t, pot veure aquesta contrapartida a trav&#233;s d'un extracte
    o un diari. Pot donar-se el cas que la contrapartida elegida no sigui
    l'apropiada, llavors pot introduir la contrapartida correcta en el seient
    mitjan&#231;ant aquest camp. El programa sempre donar&#224; prefer&#232;ncia a la contrapartida
    que vost&#232; ha introdu&#239;t.<br>
    <br>
    <div style="color: rgb(51, 51, 255);">
      <div style="font-weight: bold;">DEU / HAVER:</div>
    </div>
    Pot introduir directament un valor en el camp de deu o d'haver, <strong>per&#242;
    nom&#233;s en un d'ells</strong>, l'altre ha de  valer obligat&#242;riament
    zero.<br>
</li><li> Si prem la <strong>tecla +</strong> en un d'aquests dos camps el valor
    de desquadri del seient ser&#224; assignat a aqueix camp.<br>
    
  </li><li> Si prem la <strong>tecla *</strong> el camp presentar&#224; el mateix 
    valor que el camp anteriorment introdu&#239;t.<br>
    <br style="color: rgb(51, 51, 255); font-weight: bold;">
    <div style="color: rgb(51, 51, 255); font-weight: bold;">CONCEPTE:</div>
    El concepte &#233;s <strong>una descripci&#243; de l'apunt</strong>, no t&#233;
    m&#233;s que introduir un text explicatiu del mateix. (De moment en aqueix
    text no estan permeses les tecles * i +).<br>

  </li><li> Si prem la <strong>tecla *</strong> a l'editar aquest camp, el camp prendr&#224;
    el mateix valor que el seu predecessor.<br>
    <br>
    <div style="color: rgb(51, 51, 255); font-weight: bold;">IVA:</div>
    La introducci&#243; d'IVA ha de realitzar-se mitjan&#231;ant el bot&#243; <strong>IVA</strong>
    i, normalment, es fa de forma autom&#224;tica al tancar el seient. Aquest
    camp apareix aqui per a indicar que aquest apunt duu associada una entrada
    en el registre d'IVA. El valor que apareix indica el <strong>percentatge</strong>
    d'IVA corresponent. En Espanya els valors est&#225;ndars de iva
    s&#243;n 0, 4, 7 i 16 %<br>
    <br>
    <div style="color: rgb(51, 51, 255); font-weight: bold;">CENTRE DE COST:</div>
    Encara per documentar<br>
    <br>
    <div style="color: rgb(51, 51, 255); font-weight: bold;">CANAL:</div>
    Encara per documentar<br>

    </li></div>

    <br>
Els apunts en el seient poden ser alterats en el seu ordre, mitjan&#231;ant la combinaci&#243;
de tecles<strong> CTRL + DALT i CTRL + BAIX </strong>Pot pujar o baixar un
apunt determinat. (Tamb&#233; disposa d'aquesta opci&#243; en el men&#250;
contextual que apareix al pr&#233;mer amb<strong> el bot&#243; dret</strong> sobre l'apunt
.<br>
 <br>
Pot voler <strong>esborrar</strong> un apunt determinat. Per a aix&#242; premi amb
<strong> el bot&#243; dret del ratol&#237; </strong>sobre l'i faci clic
sobre l'opci&#243; d'esborrar apunt.<br>
        <br>
En determinats casos li interessar&#224; <strong>duplicar</strong> un apunt,
per a aix&#242; tamb&#233; disposa d'una opci&#243; en el men&#250; contextual.<br>

<div>&#201;s recomanable emplenar totes les dades perqu&#232;, posteriorment, pugui
  comprendre millor els llistats i informes que pot proporcionar-li el programa.</div>


<br>
<div>Si despr&#233;s de tancar el seient desitja <strong>modificar-lo</strong>,
  pot fer-ho tornant a obrir-lo amb el bot&#243; d'Obrir <strong>cadenat.</strong></div>


<br>
<div>En el cas que el seient <strong></strong> quedi desquadrat, el programa
  li avisar&#224; quan intenti tancar el seient amb un missatge que li permet
  tornar a l'edici&#243; del seient o b&#233; <strong>guardar-ho obert </strong>per a
  la seva posterior modificaci&#243; i tancament. Els seients oberts no intervenen
  en els llistats comptables. Pel que &#233;s recomanable tenir b&#233; controlats
  els seients que ha deixat oberts. Una comptabilitat no pot ser considerada
  "correcta" si mant&#233; seients oberts. <br>
        </div>


<strong>BulmaG&#233;s</strong><strong></strong> li permet afegir a
cada seient tants apunts com consideri necessari sempre que el resultat
final quedi quadrat.<br>
<div>Pot ser que desitgi <strong>canviar la data d'un seient </strong>ja introdu&#239;t,
  o <strong>escriure algun comentari</strong> sobre aqueix seient, fins i tot<strong>
  canviar el número de seient</strong>, per a aix&#242; disposa de la finestra
  d'edici&#243; de seients prement sobre el bot&#243; <img height="24" width="24" src="imgbotones/boton_editar.gif">,
  on apareixer&#224; la seg&#252;ent finestra que li permet canviar totes les
  dades esmentades.<br>
</div>
        <div style="text-align: center;"><img style="width: 509px; height: 252px;" alt="detalle del asiento" title="" src="imgmanual/detallasiento.png"><br>
        </div>

    <br>
Bulmag&#233;s disposa d'un<strong> llistat r&#224;pid de seients</strong>.
Amb aqueix llistat pot veure tots els seients introdu&#239;ts en el sistema de forma
r&#224;pida, si estan oberts o tancats, i fins i tot el saldo deutor i creditor
dels mateixos. Aquest llistat li pot ser &#250;til a l'hora de verificar el
sistema comptable. Pot accedir a dita llistada mitjan&#231;ant el bot&#243; <img height="24" width="24" src="imgbotones/boton_asientos.gif"><br>
        <br> <div style="text-align: center;"><img style="width: 521px; height: 382px;" alt="listado de asientos" title="" src="imgmanual/listasientos.png"><br>
        </div>

    <br>
      <div style="vertical-align: top;">
<div class="subtitular1">SEIENTS INTEL&#183;LIGENTS. <img height="24" width="24" src="imgbotones/boton_ainteligente.gif"></div>
<br>

    <br>

  <div style="vertical-align: top;" class="subtitular2">INTRODUCCI&#211; DE
    SEIENTS MITJAN&#199;ANT SEIENTS INTEL&#183;LIGENTS</div>

    <br>
  Pot introduir un seient complet prement sobre el bot&#243; <img height="24" width="24" src="imgbotones/boton_ainteligente.gif">
  El seient ha d'estar obert perqu&#232; es realitzi la introducci&#243; d'un
  seient intel&#183;ligent. Autom&#224;ticament apareixerà la seg&#252;ent pantalla:<br>
   <br> <div style="text-align: center;"><img style="width: 608px; height: 279px;" alt="asiento inteligente" title="" src="imgmanual/ainteligente.png"><br>
          <br>
          <div style="text-align: left;">
      <p>En la part superior, pot seleccionar la <strong>plantilla</strong>
        a utilitzar per a la introducci&#243; de l'apunt. En la part inferior
        es demanaran les <strong>dades</strong> necess&#224;ries per a la <strong>confecció</strong>
        del seient. </p>
      <p> Una vegada emplenats les dades, pot pressionar el<strong> bot&#243;
        acceptar</strong> i automàticament es crearan els apunts que formen
        el seient en <strong>el seient obert</strong>.<br>
      Pot introduir tants seients intel&#183;ligents com desitgi en un mateix seient.<br>
    </p>
  </div>
        </div>

    <div class="subtitular1">CREACIÓ DE SEIENTS INTEL&#183;LIGENTS. <img height="24" width="24" src="imgbotones/boton_ainteligente.gif"></div>


  <p> Pot crear tants seients intel&#183;ligents com precisi per a la seva comptabilitat.
    Per a aix&#242; disposa de l'opci&#243; <img height="24" width="24" src="imgbotones/boton_ainteligente.gif">
    en les toolbars i en el men&#250; de l'aplicaci&#243;.</p>
  Automàticament apareixer&#224; la seg&#252;ent pantalla:<br>
   <br> <div style="text-align: center;"><img style="width: 942px; height: 332px;" alt="creacion de asientos inteligentes." title="" src="imgmanual/ainteligentecreacion.png"><br>
        </div>


  <p> La creaci&#243; de nous seients intel&#183;ligents funciona com la introducci&#243;
    d'un seient, amb l'excepci&#243; que en els camps en els quals calgui
    pugues introduir <strong>variables</strong> o <strong>operacions amb variables</strong>.</p>


  <p>Per a fer que un camp determinat del seient sigui variable n'hi ha prou amb introduir
    el nom de variable <strong>entre signes de dolar</strong> ($). Aix&#242; far&#224;
    que, una vegada que estigui treballant amb el seient intel&#183;ligent, per a introduir
    aquest valor abans sigui preguntat a l'usuari en la plantilla. Tamb&#233;
    es pot operar variables introduint una <strong>f&#243;rmula</strong>
    que usi la variable i en la creaci&#243; del seient s'introduir&#224;
    autom&#224;ticament el resultat de l'operaci&#243;.</p>



  <p>Existeixen <strong>variables predefinides</strong> pel programa, que, encara
    sent variables, no provoquen que el sistema pregunti per elles ja que s&#243;n
    calculades de forma autom&#224;tica.</p>
  <strong>Per exemple:</strong><br>
  <dir> $fechaactual$ -- ser&#224; substitu&#239;da per la data del dia actual<br>
  $fechaasiento$ -- la data del seient.</dir>

  <p>Pot, al mateix temps que crea tot el seient, crear entrades en el registre
    d'IVA de forma autom&#224;tica. Per a aix&#242; disposa de la pestanya
    addicional <strong>Registre IVA</strong>. Noti que si deixa un seient intel&#183;ligent
    sense entrades de registre d'IVA al tancar el seient creat se li preguntar&#224;
    per elles.</p>


<hr>

<br>
  <br>
      <div><div class="titular"><img height="24" width="24" src="imgbotones/boton_mayor.gif">
          LLIBRE MAJOR</div></div>



  <br>

  <div>Mostra el Llibre <strong>Major</strong> i permet la possibilitat de <strong>llistar
    els apunts</strong> dels comptes i subcomptes existents en la base de
    dades del programa.<br>
      <br>
      <div style="text-align: center;"><font face="Verdana, Arial, Helvetica, sans-serif" size="1"><img style="width: 920px; height: 484px;" alt="extracto de cuentas" title="" src="imgmanual/extracto.png"></font><br>
      </div>
      </div>


    <br>
  <div></div>
En l'extracte hi ha quatre pa&#341;ametros b&#224;sics que el delimiten:<br>
      <br>
      <span style="color: rgb(51, 51, 255);">Compte Inicial</span>: El compte
inicial a partir del qual es vol comen&#231;ar l'extracte<br>
  <span style="color: rgb(51, 51, 255);">Compte Final:</span> El compte que suposar&#224;
  el final del llistat de l'extracte.<br>
      <span style="color: rgb(51, 51, 255);">Data Inicial:</span>
Data inicial dels apunts a mostrar<br>
  <span style="color: rgb(51, 51, 255);">Data Final:</span> Data final a partir de
  la qual no es mostraran m&#233;s apunts.<br>
      <br>


    <br>
  Una vegada introdu&#239;ts les quatre dades b&#224;sics, l'extracte mostrar&#224;
  els apunts referents a cadascuna dels comptes referenciats entre les dates
  introdu&#239;des. Pot navegar entre els comptes mitjan&#231;ant els bot&#243;nes de
  navegaci&#243; de la toolbar, accedint al compte seg&#252;ent, el compte
  anterior, la primera i l'&#250;ltim compte. <br>
      <br>
  Si vol actualitzar la informaci&#243; disposa del bot&#243; actualitzar
  <strong>#bot&#243; actualitzar#</strong><br>
      <br>
  Per a cada compte l'extracte es mostra primer una <strong>informaci&#243;
  preliminar</strong> com &#233;s el codi del compte, el seu nom, i el deu,
  haver i saldo que t&#233; aquest compte abans de la data inicial.<br>
      <br>
  Despr&#233;s, en forma de taula, es presenten <strong>informaci&#243; detallada</strong>
  amb els diferents apunts introdu&#239;ts en la comptabilitat durant el per&#237;ode
  seleccionat i ordenats per data d'apunt.<br>
      <br>
AL final apareixen el deu, haver i saldo del compte a data final.<br>
      <br>
  Els camps que apareixen s&#243;n data, número de seient, descripci&#243;,
  contrapartida, deu, haver, saldo, centre de cost i canal. En la seva versi&#243;
  del programa pot ser que no apareguin tots els camps ja que els camps a visualitzar
  s&#243;n configurables mitjan&#231;ant l'arxiu bulmages.conf (veure secci&#243; bulmages.conf).<br>


    <br>

  <div class="subtitular1">IMPRESSI&#211;&#160; D'EXTRACTES. <img height="24" width="24" src="imgbotones/boton_imprimir.gif"></div>


  <p> Pot ser que vost&#232; vulgui una c&#242;pia impresa del seu extracte. En aquest cas disposa
    del bot&#243; <img height="24" width="24" src="imgbotones/boton_imprimir.gif">
    que far&#224; que aparegui el seg&#252;ent quadre de di&#224;leg solicitant-li les
    opcions d'impressi&#243;.</p>

      <div style="text-align: center;"><img style="width: 608px; height: 246px;" alt="imprimir extracto" title="" src="imgmanual/imprimirextracto.png"></div>
	  <br>

  <p>Els camps apareixeran plens amb les dades que t&#233; posats
    en pantalla, amb el que nom&#233;s haur&#224; d'elegir el format i pr&#233;mer
    el bot&#243; imprimir.</p>
  <p><strong>Bulmag&#233;s</strong> obrir&#224; un editor de textos o un navegador
    amb l'extracte visualitzat.</p>

    <br>

  <div style="vertical-align: top;" class="subtitular1">PUNTEIG I CASSACI&#211;</div>

    <br>
  Sobre l'extracte, pot interessar-li anar <strong>puntejant els apunts</strong>
  per a, per exemple, comprovar amb un extracte bancari la correcci&#243; dels
  mateixos.<br>
  Per a realitzar aquesta operaci&#243; disposa en l'extracte de <strong>caselles
  de verificaci&#243;</strong> sobre les quals pot pr&#233;mer per a posar o llevar
  el punteig. El punteig es guarda autom&#224;ticament en la base de dades, amb
  el que si canvia de finestra no perdr&#224; el punteig.<br>
      <br>
  <strong>BulmaG&#233;s</strong> tamb&#233; li ofereix la funci&#243; 
  de guardar el punteig en un moment determinat per a poder recuperar-ho m&#233;s endavant
  si cal. En la finestra de l'extracte disposa del bot&#243;
  <strong>#bot&#243; guardar punteig# </strong>que li preguntar&#224; un nom d'arxiu
  i la localitzaci&#243; del mateix per a guardar el punteig.<br>
      <br>
  M&#233;s endavant, si vol recuperar el punteig guardat amb anterioritat,
  disposa del bot&#243; <strong>#bot&#243; recuperar punteig# </strong>que li sol&#183;licitar&#224;
  a partir de quin arxiu recuperar el punteig.<br>
      <br>
  Pot tenir diversos punteigs emmagatzemats en el sistema i recuperar-los quantes
  vegades desitgi.<br>
      <br>
      <div style="font-weight: bold;">NOTA:</div> Si despr&#233;s de guardar el
punteig ha realitzat alguna modificaci&#243; d'apunts, o
inserci&#243; de nous seients comptables pot ser que el resultat de
recuperar el punteig no sigui correcte per als apunts modificats o per als
nous apunts inserits.<br>
      <br>
  En <strong>BulmaG&#233;s</strong> existeix tamb&#233; l'opci&#243; de
  <strong>puntejar directament els apunts que tenen un parell id&#232;ntic</strong>.
  Aix&#242; vol dir que&#160; existeix un apunt amb un import en el deu i altre
  apunt en el mateix compte amb un import id&#232;ntic en l'haver. Aquest cas
  sol oc&#243;rrer en aquells tipus d'apunts que solen correspondre amb una mateixa
  transacci&#243; econ&#242;mica que ja ha estat acabada i que ha deixat
  el saldo del compte igual que estava. Per a aix&#242; t&#233; el bot&#243; de <strong>cassaci&#243;</strong>
  #bot&#243; de cassaci&#243;#. AL pr&#233;mer aquest bot&#243; directament es puntejaran
  tots els parells d'apunts amb imports rec&#237;procs en l'extracte del
  compte que estigui visualitzant.<br>


    <br>
<div class="subtitular1">FILTRAT DE L'EXTRACTE. <img height="24" width="24" src="imgbotones/boton_filtrar.gif"></div>
<br>
  Pot oc&#243;rrer que li sigui m&#233;s c&#242;mode treballar si el sistema omet
  determinats apunts sobre els quals vost&#232; no est&#224; interessat. Per a aix&#242;
  t&#233; el bot&#243; de <strong>filtrat</strong> que li permet seleccionar les caracter&#237;stiques
  que han de complir aquells apunts que desitja veure.<br>
      <br>
      <div style="text-align: center;"><img style="width: 584px; height: 310px;" alt="filtrado en el extracto." title="" src="imgmanual/filtroextracto.png"><br>
      </div>




	<hr>

	<br>
    <br>
      <br>
      <div class="titular"><img height="23" width="25" src="imgbotones/boton_diario.gif">&#160;LLIBRE
        DIARI</div>

<br><br>


<div align="center"><img style="width: 993px; height: 427px;" alt="libro diario" title="" src="imgmanual/diario.png"></div>

 <br>
  <br>
  El diari <strong></strong> t&#233; dos paràmetres b&#224;sics que el defineixen:<br>
  <br>
  <span style="color: rgb(51, 51, 255);">Data Inicial:</span> Data inicial dels
  apunts a mostrar<br>
  <span style="color: rgb(51, 51, 255);">Data Final:</span> Data final a partir de
  la qual no es mostraran m&#233;s apunts.<br>
  <br>
  El llibre diari mostra tots els apunts introdu&#239;ts per <strong>ordre cronol&#242;gic</strong>.<br>
  <br>
  Amb el bot&#243; dret del raton podra veure un men&#250; contextual que
  permet anar al seient corresponent, mostrar el llibre balan&#231;, o un extracte
  de comptes de l'apunt seleccionat.<br>
  <br>
</div>
<div class="subtitular1">FILTRAT DEL DIARI. <img height="8" width="24" src="imgmanual/boton_filtrar.gif"></div>

    Pot ser que desitgi veure en el diari nom&#233;s els apunts que compleixin determinades
caracter&#237;stiques. Per a aix&#242; t&#233; l'opci&#243; de filtrat amb el bot&#243;
<img height="24" width="24" src="imgbotones/boton_filtrar.gif"><br>
<br>
      <div style="text-align: center;"><img style="width: 584px; height: 310px;" alt="filtro diario" title="" src="imgmanual/FILTROdiario.png"><br>
      </div>


    <br>

<div class="subtitular1">IMPRESSI&#211; DEL DIARI. <img height="24" width="24" src="imgbotones/boton_imprimir.gif"></div>


<p>Quan es trobi treballant amb el diari, si pressiona el bot&#243; <img height="24" width="24" src="imgbotones/boton_imprimir.gif">
  apareixer&#224; la seg&#252;ent finestra:</p>
<br>
      <center><img style="width: 608px; height: 233px;" alt="imprimir diario" title="" src="imgmanual/printdiario.png"></center>
<p>Els camps apareixeran plens amb les dades que t&#233; posats en pantalla,
  amb el que nom&#233;s haur&#224; d'elegir el format i pr&#233;mer el bot&#243;
  imprimir.</p>
<p align="left"><strong>Bulmag&#233;s</strong> obrir&#224; un editor de textos 
  o un navegador amb l'extracte visualitzat.</p>

    <br>



<hr>

<br>
    <br>

    <div class="titular" style="vertical-align: top;"><img height="24" width="24" src="imgbotones/boton_balance.gif">
      <img height="24" width="24" src="imgbotones/boton_balancetree.gif">BALAN&#199;
      DE COMPTES</div>


<p>El <strong>balan&#231; de comptes</strong> &#233;s un llistat molt utilitzat en comptabilitat
  (<strong>Balan&#231; de Sumes i Saldos</strong>). <strong>BulmaG&#233;s </strong>ofereix
  aqueix llistat en dues versions diferents.<br>
</p>
<div class="subtitular1"> BALAN&#199; DE COMPTES NORMAL. <img height="24" width="24" src="imgbotones/boton_balance.gif"></div>


<p>El balan&#231; de comptes normal mostra els <strong>comptes d'un determinat
  nivell i els seus saldos</strong> entre un per&#237;ode de temps. Pot elegir el nivell
  de detall que desitja per als seus comptes i un rang de comptes a mostrar.</p>
<p>Pot succeir que comptes de nivells superiors tinguin associats apunts comptables
  (encara que no &#233;s una bona pr&#224;ctica comptable). En aquest cas el balan&#231; tamb&#233;
  mostrar&#224; aquests comptes ja que, en cas contrari, no s'oferiria
  una imatge fidel de l'empresa.</p>
<p align="center"><img style="width: 908px; height: 366px;" alt="balance de cuentas" title="" src="imgmanual/balance.png"></p>
<br>
<div class="subtitular1">BALAN&#199; JER&#192;RQUIC DE COMPTES. <img height="24" width="24" src="imgbotones/boton_balancetree.gif"></div>
<p>El <strong>balan&#231; jer&#224;rquic</strong> de comptes ofereix la mateixa informaci&#243;
  que el balan&#231; normal des d'una perspectiva diferent. Mitjan&#231;ant el balan&#231; jer&#224;rquic
  podr&#224; veure l'arbre de comptes i els corresponents saldos que
  pugen a trav&#233;s de l'arbre. D'aquesta forma pot veure amb claredat quins
  comptes componen cadascuna de les parts dels comptes de nivell superior.</p>
<p>Pot elegir a quin nivell vol expandir l'arbre de forma que 
  sempre pot controlar la quantitat d'informaci&#243; mostrada.</p>
<p align="center"><img style="width: 908px; height: 366px;" alt="balance jerarquico" title="" src="imgmanual/balancejerarquico.png"><br>
</p>
<div style="vertical-align: top;">
  <div class="subtitular1">IMPRESSI&#211; DEL BALAN&#199;. <img height="24" width="24" src="imgbotones/boton_imprimir.gif"></div>
  <p>Pot obtenir una c&#242;pia impresa del balan&#231; (en qualsevol de les seves dues modalitats) 
    pressionant el bot&#243; <img height="24" width="24" src="imgbotones/boton_imprimir.gif"> 
    Immediantament apareixer&#224; la finestra d'impressi&#243; de balan&#231;.</p>

  <p>En aquesta finestra pot elegir les opcions del llistat, tamb&#233;
    pot elegir el format desitjat per al mateix. Quan premi el bot&#243; 
    imprimir s'obrir&#224; el programa visor del llistat i l'hi presentar&#224; 
    aix&#237;: </p>
  <div align="center"><br>
    <img style="width: 608px; height: 364px;" alt="impresion de balance" title="" src="imgmanual/balanceprint.png"><br>
  </div>
  <hr>
<br>
<br>
<div>
    <div class="titular">ESTAD&#205;STIQUES </div>
</div>
<br>
<div class="subtitular1">SEGUIMENT DE COMPTES</div> <br>
  Mitjan&#231;ant l'opci&#243; <strong>estadisticas </strong>/<strong> seguiment
  de comptes</strong> del men&#250; de l'aplicaci&#243; pot accedir a una 
  pantalla que li permet seguir de forma gr&#224;fica la progressi&#243; 
  dels comptes de la seva empresa. Mitjan&#231;ant aquesta pantalla pot seguir el saldo, 
  els moviments i les principals contrapartides que intervenen amb el compte 
  introdu&#239;t. Pot arribar a comparar fins tres comptes diferents.<br> 
<br>
<center><img style="width: 945px; height: 450px;" alt="Estadisticas" title="" src="imgmanual/estadisticas1.png"></center><br>
<br>
<center><img style="width: 945px; height: 450px;" alt="estadisticas" title="" src="imgmanual/estadisticas2.png"></center><br>
  <br>
<div><div class="subtitular1">BALAN&#199; GR&#192;FIC</div></div>
<br>
  Mitjan&#231;ant aquesta opci&#243; pot obtenir<strong> informaci&#243; gr&#224;fica
  sobre el balan&#231; de comptes</strong>. Gr&#224;cies al diagrama circular del balan&#231; 
  pot veure quins comptes tenen major rellev&#224;ncia en la seva comptabilitat. El que 
  mostra el balan&#231; gr&#224;fic &#233;s el saldo dels comptes de<strong> 
  nivell 2</strong>.<br>
<br>
<center><img title="" style="width: 768px; height: 438px;" alt="balance gr&#225;fico" src="imgmanual/bgrafico.png"></center>
<br>
<hr>
<br>
  <div class="titular">ALTRES OPCIONS</div>
  <div>
    <p>Permet posicionar les finestres obertes en el programa en diferents posicions
      predefinides.</p>
    </div>
  <div> 
    <div class="head1">NEW WINDOW</div>
    <a name="3271"></a></div>
  <br>
  <div> 
    <div class="head1"> CASCADE</div>
    <a name="3272"></a></div>
  <div>Mostra totes les finestres existents en aquest moment, una despr&#233;s d'una altra, en 
    l'ordre que han estat obertes.<br>
  </div>
  <div> 
    <div class="head1">TITLE</div>
    <a name="3273"></a></div>
  <br>
  <div>Mostra totes les finestres obertes ocupant el m&#224;xim espai
    del total de la pantalla.</div>
  <br>
  <div> 
    <div class="head1"> ALTRES OPCIONS</div>
    <a name="3274"></a></div>
  <br>
  <div>Permet alternar entre les finestres obertes en aquest moment sense necessitat 
    de mostrar-les totes o de minimitzar-les o maximitzar-les.<br>
  </div>
  <div> 
    <div class="subtitular1"> EL MEN&#218; VEURE</div>
    <a name="328"></a></div>
  <br>
  <div> 
    <div class="head1"> TOOLBAR</div>
    <a name="3281"></a></div>
  <div>Mostra o oculta la barra d'eines del programa segons estigui 
    o no seleccionada aquesta opci&#243;.</div>
  <br>
  <div> 
    <div class="head1">STATUSBAR</div>
    <a name="3282"></a></div>
  <div>Mostra o oculta la barra d'estatus del programa segons estigui 
    o no seleccionada aquesta opci&#243;.</div>
  <br>
</div>
<div class="subtitular1">EL MEN&#218; AJUDA</div>
<div style="vertical-align: top;"> <br>
  <br>
  <div class="subtitular1"> SOBRE </div>
  <br>
  <div>Mostra la informaci&#243; del programa: la versi&#243;, els autors 
    i col&#183;laboradors i les empreses o entitats que proporcionen o han proporcionat 
    suport al programa aix&#237; com algunes adreces d'inter&#232;s.<br>
    Per a m&#233;s informaci&#243; sobre el programa consulti la web oficial:<br>
    <a href="http://bulmages.bulma.net/">http://bulmages.bulma.net</a><br>
    O escrigui a l'e-mail d'algun dels autors.</div>
  <br>
  <div class="subtitular1">QU&#200; &#201;S ??</div>
</div>
<br>
<div>Li permet consultar la descripci&#243; de les icones del programa.</div>
<br>
<div> 
  <div class="subtitular1"> EL MEN&#218; SORTIR</div>
</div>
<br>
<div> 
  <div class="subtitular1"> EXIT</div>
</div>
<br>
<div>Tanca les finestres actives en el programa i li permet acabar l'execuci&#243; 
  de BulmaG&#233;s.</div>
<hr>
<br>
<div style="vertical-align: top;" class="titular">L'ARXIU DE CONFIGURACIÓ
  BULMAGES.CONF</div>
<p>A trav&#233;s d'aquest arxiu, pot especificar determinats par&#224;metres
  que afecten al comportament del programa.<br>
  L'arxiu es compon de l&#237;nies amb dues paraules. La primera paraula
  indica la car&#225;ter&#237;stica a la qual es desitja donar valor i la segona
  paraula indica el valor que pren aquesta caracter&#237;stica.<br>
  <br>
  A continuaci&#243; es detallen totes les variables que es poden configurar,
  com afecten al programa i els possibles valors que poden tenir.<br>
  <br>
  # Aquests valors indiquen el fons i la font de la finestra d'apunts.<br>
</p><div style="color: rgb(51, 51, 255);"><br>
  CONF_BG_APUNTES:</div>
 Aquesta opci&#243; indica el color de fons de la finestra d'introducci&#243;
d'apunts quan el seient està tancat. Com valor agafa un color en format
RGB (#RRGGBB)<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_FG_APUNTES:
  <div style="color: rgb(0, 0, 0);">Aquesta opci&#243; indica el color de text de la
    finestra d'introducci&#243; d'apunts quan el seient està tancat.
    Com valor agafa un color en format RGB.<br>
    <br>
  </div>
  CONF_BG_APUNTESA:</div>
 Aquesta opci&#243; indica el color de fons de la finestra d'introducci&#243;
d'apunts quan el seient est&#224; obert. Com valor agafa un color en
format RGB.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_FG_APUNTESA:</div>
 Aquesta opci&#243; indica el color de text de la finestra d'introducci&#243;
d'apunts quan el seient est&#224; obert. Com valor agafa un color en
format RGB.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_FONTSIZE_APUNTES:</div>
 Aquesta opci&#243; indica la grand&#224;ria de la font de la pantalla d'apunts.
Com valor agafa un número enter positiu indicant la grand&#224;ria de la font.
<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_FONTFAMILY_APUNTES:</div>
 Aquesta opci&#243; indica la tipografia de la font de la pantalla d'apunts
Com valor agafa un nom de font p.ej: courier, airal, estafis new roman,
helvetica, verdana.<br>
<br style="color: rgb(51, 51, 255);">
<div style="color: rgb(51, 51, 255);">CONF_CONTRAPARTIDA_APUNTES:</div>
&#160;  Aquesta opci&#243; indica si en la pantalla d'introducci&#243; d'apunts
ha d'apar&#232;ixer la columna de contrapartida. Si desitja que aparegui la contrapartida
en la pantalla d'introducci&#243; d'apunts posi el valor&#160; YES en aquesta
opci&#243;, en cas contrari escrigui el valor NO.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_CANAL_APUNTES:</div>
&#160;&#160;  Aquesta opci&#243; indica si en la pantalla d'introducci&#243;
d'apunts ha d'apar&#232;ixer la columna del canal. Els valors que pot agafar s&#243;n
YES i NO.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_CCOSTE_APUNTES:</div>
&#160;&#160;  Aquesta opci&#243; indica si en la pantalla d'introducci&#243;
d'apunts ha d'apar&#232;ixer la columna de centre de cost. Els valors que pot
agafar s&#243;n YES i NO.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_CONTRAPARTIDA_EXTRACTO:</div>
 Aquesta opci&#243; indica si en la finestra d'extracte ha d'apar&#232;ixer el camp
de contrapartida, aquesta opci&#243; pot conetener valors YES i NO.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_CANAL_EXTRACTO:</div>
&#160;  Aquesta opci&#243; indica si en la finestra d'extracte ha d'apar&#232;ixer
el camp de canal. Aquesta opci&#243; admet els valors YES i NO<br>
<br style="color: rgb(51, 51, 255);">
<div style="color: rgb(51, 51, 255);">CONF_CCOSTE_EXTRACTO:</div>
&#160;&#160;  Aquesta opci&#243; indica si en la finestra d'extracte ha d'apar&#232;ixer
el camp de centre de cost. Aquesta opci&#243; pot agafar valors YES i NO.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_CONTRAPARTIDA_DIAROI:</div>
&#160;  Aquesta opci&#243; li permet mostrar la columna de contrapartida en
el llibre diari del programa. Aquesta opci&#243; pot agafar valors YES o NO.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_CANAL_DIARIO:</div>
&#160;&#160;  Aquesta opci&#243; li permet mostrar la columna de canal en
el llibre diari del programa. Aquesta opci&#243; pot contenir valors YES o NO.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_CCOSTE_DIARIO:</div>
<p>&#160;&#160;  Aquesta opci&#243; li permet mostrar la columna de centre de
  cost en el llibre diari del programa. Aquesta opci&#243; pot agafar valors
  YES o NO.<br>
  <br>
  <br>
  # Aquests valors indiquen el fons de la finestra del diari</p>
<p><br>
  <font color="#0000ff">CONF_BG_DIARIO</font>&#160;&#160; #66CCCC<br>
  <font color="#0000ff">CONF_FONTSIZE_DIARIO</font>&#160; 12.<br>
  <font color="#0000ff">CONF_FONTFAMILY_DIAROI</font>&#160; Arial<br>
  <font color="#0000ff">CONF_FG_DIARIO1</font>&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;
  #CC0000.<br>
  <font color="#0000ff">CONF_FG_DIARIO2</font>&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;
  #000000.<br>
  <br>
  # Par&#224;metres de configuraci&#243; de la finestra d'Extracte</p>
<p><br>
  <font color="#0000ff">CONF_BG_EXTRACTO</font> #0099CC.<br>
  <br>
  # Par&#224;metres de configuraci&#243; de les finestres de Balan&#231;</p>
<p><br>
  <font color="#0000ff">CONF_BG_BALAN&#199;</font>&#160; #FFCC99.<br>
  <font color="#0000ff">CONF_BG_BALANCETREE</font> #CCCC66.<br>
  <br>
  #Par&#224;metres de configuraci&#243; del splash i del fons del area de treball</p>
<p><br>
  <font color="#0000ff">CONF_BACKGROUND:</font> /usr/share/bulmages/backgrounds/universo.jpg</p>
<p></p>
<p><br>
  <font color="#0000ff">CONF_SPLASH:</font>&#160; Aquesta opci&#243; indica la
  ruta de l'arxiu d'imatge del splash. P. ej: /usr/share/bulmages/nousplash1.jpg<br>
  <br>
  # Par&#224;metres de configuraci&#243; de la base de dades.<br>
</p><div style="color: rgb(51, 51, 255);">CONF_METABASE:</div>
&#160;  Aquesta opci&#243; li permet&#160; especificar quin ser&#224;
la base de dades metabd del sistema. Per defecte el nom de base de dades meta
&#233;s metabd, baix determinades circumst&#224;ncies a vost&#232; pot interessar-li que la base
de dades metabd tingui un nom diferent. Escrigui aqui el nom d'aquesta base
de dades.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_SERVIDOR:</div>
&#160;&#160;  Mitjan&#231;ant aquesta opci&#243; pot indicar quin ser&#224;
el servidor de bases de dades amb el qual el programa intentar&#224; establir
comunicaci&#243;. Per defecte aquesta opci&#243; va en localhost. Per&#242;, si
li interessa que les bases de dades estiguin en un ordinador diferent, pot
especificar la IP de dita ordinador mediate aquesta opci&#243;.<br>
<br>
<div style="color: rgb(51, 51, 255);">CONF_PUERTO:</div>
<p>&#160; Mitjan&#231;ant aquesta opci&#243; pot indicar en quin port de
  l'ordinador es troba allotjat el servidor de base de dades postgresql, per
  defecte sol ser el 5432.<br>
  <br>
  # Par&#224;metres de configuraci&#243; de programes</p>
<p><br>
  <font color="#0000ff">CONF_EDITOR:</font>&#160; Aquesta opci&#243; indica quin
  &#233;s l'editor de textos que es va a utilitzar per a mostrar els llistats. p ej:
  kedit<br>
</p>
<p><font color="#0000ff">CONF_NAVEGADOR:</font>&#160; Aquesta opci&#243; indica
  quin &#233;s el navegador html que es va a utilitzar per a mostrar els llistats
  i p&#224;gines auxiliars p ej:mozilla.<br>
</p>
</body></html>
