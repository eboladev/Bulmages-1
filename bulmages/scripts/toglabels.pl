#!/usr/bin/perl -w


use XML::EasyOBJ;

my $glabels = new XML::EasyOBJ(-type => 'new' , -param=> 'Glabels-templates');
my $i = 0;
while(<>) {

   if (/^\s*(\S*)(.*)\(([0-9., ]+)\)\s*$/) {
      my $nom = $1;
      my $desc = $2;
      my $dades = $3;
      my %mides = ();

      @mides{qw/amplada alcada num_linies num_columnes ample_paper alt_paper marge_superior marge_esquerra marge_dret marge_inferior/} = split(/,/,$dades);
      my $t = $glabels->Template($i++);
      $t->setAttr('name',$nom);
      $t->setAttr('_description',$desc);
      $t->setAttr('size','Other');
      $t->setAttr('height',$mides{'alt_paper'}.'mm');
      $t->setAttr('width',$mides{'ample_paper'}.'mm');
      my $r = $t->getElement('Label-rectangle');
      $r->setAttr('id',0);  #legal ?
      $r->setAttr('width',$mides{'amplada'}.'mm');
      $r->setAttr('height',$mides{'alcada'}.'mm');
      my $l = $r->getElement('Layout');
      $l->setAttr('ny',$mides{'num_linies'});
      $l->setAttr('nx',$mides{'num_columnes'});
      $l->setAttr('y0',$mides{'marge_superior'}.'mm');
      $l->setAttr('x0',$mides{'marge_esquerra'}.'mm');
      if ($mides{'num_columnes'}>1) {
         $l->setAttr('dx',(($mides{'ample_paper'}-$mides{'marge_esquerra'}-$mides{'marge_dret'}-$mides{'amplada'})/($mides{'num_columnes'}-1)).'mm');
      }
      if ($mides{'num_linies'}>1) {
         $l->setAttr('dy',(($mides{'alt_paper'}-$mides{'marge_superior'}-$mides{'marge_inferior'}-$mides{'alcada'})/($mides{'num_linies'}-1)).'mm');
      }
   }
}

my $dom = $glabels->getDomObj;
print (($dom->getOwnerDocument || $dom)->toString);
