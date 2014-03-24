#!/usr/bin/perl -w
use IPC::Open2;
use encoding  'ISO-8859-1';

sub sep {
   my $a = $_[0];
  if ( ($a % 8) == 7)  {
     $s = "\n";
  } else {
     $s = "";
  }
  if ( ($a % 32) == 31)  {
     $s .= "//".($a+1)."\n";
  }
  return $s;
}

foreach $a (0..255) {
  if (($a == 0x0a) or ($a == ord("'")) or  ($a == ord("\\"))) { 
    push @l , sprintf ("'\\x%x'%s",$a,sep($a));
  } else {
  if (($a & 127) < 32) { 
      push @l , sprintf ("' '%s",sep($a));
    } else {
      push @l , sprintf("'%s'%s" ,chr($a),sep($a))
    }
  }
}
#La Norma 19 �s fatxa per� jo no vull fer m�s maleses de les necess�ries
foreach $a ('�','�') {

  $l[ord($a)]=sprintf ("'\\x%x'%s",165,sep(ord($a)));
}
#ASCII DEL
$l[127]="'?'".sep(127);

$pid =open2(\*ICONV_OUT,\*ICONV,  'iconv','--from-code=l1','--to-code=ascii//TRANSLIT') ;
print "//Fitxer generat automàticament. Jo de tu no el modificaria.\n// Pots modificar $0 i regenerar-lo.\n";
print  "char taula[] = {\n";
print ICONV join ',' , @l ;
close ICONV;
while(<ICONV_OUT>) {
  s/<<|>>/"/g;
  s/'[^\\,][^']+'/'?'/g;
  $_ = uc($_); 
  s/\\X/\\x/g;
  print;
}
print  "};\n";
waitpid $pid,0;
close ICONV_OUT
