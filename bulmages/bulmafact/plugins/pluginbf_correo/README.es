#######
#Plugins
#######
#README.es
#20080501

Nombre: Enviar documentos por e-mail

Biblioteca: libpluginbf_correo.so

<<<<<<< HEAD
<<<<<<< HEAD
Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/mail_send.svg" WIDTH="100"/><BR>Envios por e-mail</H1>
<P>Permite enviar, mediante un botón nuevo en las fichas, los documentos impresos por e-mail directamente. Al pulsar sobre le botón de enviar por e-mail se abre el gestor de correo con un texto y el documento adjunto</P>
<P>El programa utiliza la directiva <B>CONF_EMAIL_CLIENT</B> de los archivos de configuración. Preste especial atenci�n al archivo //.bulmages/bulmafact.conf que es el lugar donde se guarda el parametro por defecto.</P>
<P>La primera vez que este plugin es iniciado, arrancará una ventana de diálogo, que nos permite seleccionar nuestro cliente de correo preferido, si el lugar donde este instalado se encuentra en el PATH de nuestro sistema operativo, tratará de localizar su ruta automáticamente, en caso contrario deberemos presionar el botón "(...)" y seleccionar el programa, una vez seleccionado, pulsar Aceptar.</P>
<P>Una vez instalado el plugin en los presupuestos, pedidos, albaranes, facturas y cobros a clientes aparecerá el botón <IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/mail_send.svg" WIDTH="32"/> que al ser presionado abrirá el cliente de correo configurado y con un email listo para ser enviado y con el documento correspondiente adjunto. La dirección de correo a la que se envia es la que tiene puesta el cliente.</P>
=======
Descripción: <H1>Envios por e-mail</H1>
=======
Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/mail_send.svg" WIDTH="100"/><BR>Envios por e-mail</H1>
>>>>>>> 44fc05b... Arreglo problemas con kmail en el plugin de correo y mejoro el texto del README
<P>Permite enviar, mediante un botón nuevo en las fichas, los documentos impresos por e-mail directamente. Al pulsar sobre le botón de enviar por e-mail se abre el gestor de correo con un texto y el documento adjunto</P>
<P>El programa utiliza la directiva <B>CONF_EMAIL_CLIENT</B> de los archivos de configuración</P>
<P>La primera vez que este plugin es iniciado, arrancará una ventana de diálogo, que nos permite seleccionar nuestro cliente de correo preferido, si el lugar donde este instalado se encuentra en el PATH de nuestro sistema operativo, tratará de localizar su ruta automáticamente, en caso contrario deberemos presionar el botón "(...)" y seleccionar el programa, una vez seleccionado, pulsar Aceptar.</P>
<<<<<<< HEAD
>>>>>>> 9129262... Nueva versión del PluginBf_Correo finalizada.
=======
<P>Una vez instalado el plugin en los presupuestos, pedidos, albaranes, facturas y cobros a clientes aparecerá el botón <IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/imagesrc/images/oxygen/actions/mail_send.svg" WIDTH="32"/> que al ser presionado abrirá el cliente de correo configurado y con un email listo para ser enviado y con el documento correspondiente adjunto. La dirección de correo a la que se envia es la que tiene puesta el cliente.</P>
>>>>>>> 44fc05b... Arreglo problemas con kmail en el plugin de correo y mejoro el texto del README
<B>Autor:</B>Daniel Ripoll Osma (info@danielripoll.es)<BR>
<HR>


ParmBD: 

ArchivoSQLpatch: 

ArchivoSQLdeins: 

Dependencias:

Incompatibles:

Categorias: Utilidades genericas;

Orden: 900

Comentarios: Este plugin actualmente es compatible con Thunderbird, Evolution, Kmail y Outlook. (Funciona tanto en Unix como Windows.)
