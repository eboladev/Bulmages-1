#######
#Plugins
#######
#README.es
#20080501

Nombre: Imprimir tickets

Biblioteca: libpluginbf_ticket.so

Descripción: <H1><IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/icons/ticket.png" WIDTH="100"/><BR>Impresion de Tickets</H1>
<P>Imprime documentos a través en forma de tickets en impresoras ESC/POS.</P>
<P>Precisa de la impresora conectada al dispositivo serie o usb.</P>
<P>Este plugin pone un nuevo icono en las fichas de presupuestos, pedidos, albaranes y facturas <IMG SRC="@CMAKE_INSTALL_PREFIX@/share/bulmages/icons/ticket.png"/> la pulsación de este botón genera la impresion del documento a través de la impresora ticketera configurada en el programa.</P>
<P>La impresión está determinada por los archivos plantilla <B>presupuesto_ticket.txt, pedido_ticket.txt, albaran_ticket.txt y factura_ticket.txt</B> que son plantillas para el formato de salida ESC/POS</P>
<B>Author:</B>Tomeu Borrás (tborras@conetxia.com)<BR>
<HR>

ParmBD: 

ArchivoSQLpatch: 

ArchivoSQLdeins: 

Dependencias:

Incompatibles:

Reports: impers_factura.txt; impers_presupuesto.txt; impers_pedidocliente.txt; impers_albaran.txt

Categorias: Utilidades genericas;Experimentales;

Orden: 900

Comentarios: 