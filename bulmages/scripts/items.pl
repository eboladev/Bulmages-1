#!/usr/bin/perl -w
use XML::EasyOBJ;
my $i=0;

if ($#ARGV<0) {
   die "ús: items.pl nomCombo fitx1.txt ... fitxN.txt\nGenera un xslt que canvia els items del combo amb nom el primer argument per les línies dels fitxers de text passats.\n"
}
$nomCombo = shift @ARGV;
#$doc =  new XML::EasyOBJ(-type => 'new', -param => 'widget');
$doc =  new XML::EasyOBJ(-type => 'new', -param => 'xsl:stylesheet');
$doc->setAttr('version','1.0','xmlns:xsl','http://www.w3.org/1999/XSL/Transform');
# el getElement crea l'ement si no existeix
$t=$doc->getElement('xsl:template',0);
$t->setAttr('match','node()|@*');
$t->getElement('xsl:copy')->getElement('xsl:apply-templates')->setAttr('select','node()|@*');


$t=$doc->getElement('xsl:template',1);
$t->setAttr('match','widget[@name=\''.$nomCombo.'\']');
$c=$t->getElement('xsl:copy');
$f=$c->getElement('xsl:for-each');
$f->setAttr('select','*[name()!=\'item\']|@*');
$f->getElement('xsl:copy')->getElement('xsl:apply-templates')->setAttr('select','node()|@*');

while(<>) {
  chomp;
  $c->item($i)->property()->setAttr('name','text');
  $c->item($i)->property()->string->setString($_);
  $i++;
}

print $doc->getDomObj->toString;

