<?php
include "lib/easymenu.inc";

$search="<form action=\"$PHP_SELF\" method=\"GET\">\nBuscar:&nbsp;<INPUT style=\"height:1.2em; float:right; top:0px\" type=\"text\" name=\"search\">\n</form>\n";

$menu = new easymenu($search);

$menu->background				= '#81afc8';
$menu->foreground				= '#ffffff';
$menu->popup_title_bg			= '#81afc8';
$menu->popup_title_fg			= '#004488';
$menu->unselected_item_bg		= '#91bfd8';
$menu->unselected_item_fg		= '#006699';
$menu->selected_item_bg			= '#006699';
$menu->selected_item_fg			= '#91bfd8';
$menu->title_item_bg			= '#91bfd8';
$menu->title_item_fg			= '#004488';

$menu->option('Proyectos', 'Bulmagés', $PHP_SELF);
$menu->option('Proyectos', 'ERM', $PHP_SELF);

$menu->option('Recursos', 'Documentación', $PHP_SELF);
$menu->option('Recursos', 'Herramientas', $PHP_SELF);

$menu->option('Descargas', 'Bulmagés', $PHP_SELF);
$menu->option('Descargas', 'BulmaConta', $PHP_SELF);
$menu->option('Descargas', 'BulmaFact', $PHP_SELF);
$menu->option('Descargas', 'BulmaTPV', $PHP_SELF);
$menu->option('Descargas', 'ERM2DB', $PHP_SELF);

$menu->option('Usuarios', 'Login', $PHP_SELF);
$menu->option('Usuarios', 'Darte de alta', $PHP_SELF);

$menu->option('Pruebas CSS', 'Normal', "$PHP_SELF?css=");
$menu->option('Pruebas CSS', 'Menu groc', "$PHP_SELF?css=iglues-yellow.css");

echo $menu->html();
?>