#!/usr/bin/perl -w

use LWP::RobotUA;
use HTML::TokeParser::Simple;
my $ua = LWP::RobotUA->new("RecuperaMidesEtiquetes/0.1 ", 'xdrudis@tinet.cat');
$ua->delay(1);  # be very nice -- max one hit every ten minutes!

# enllaços a referències de fulls d'etiquetes a 
# http://www.apli.es/cat/catalog_refs.asp?Fam=88
# Famílies d'articles APLI interessants (2009.07.26) valors del paràmere Fam
# 88 89 92 93 104 105 117 118 120 128 
# 
# noms dels camps del full de mides. 
# marge_superior,pas_horitzontal,amplada,marge_esquerra,pas_vertical,num_columnes,alcada,etiq_fulla,num_linies
# referència mirada a mà
# 01209 38,21,2
# altres referències d'exemple
# 25 rectes 
# 01209 01212 01213 01214 01215 10825 10826 10827 10919 
# 25 romes
# 01232 01233 01992 01993 01997 01998 01999
# 
sub unaref {
 my ($REF) = @_;
my $res = $ua->get("http://www.apli.es/cat/catalog_cotes.asp?Ref=$REF\&Cm=0");

# Check the outcome of the response
if ($res->is_success) {
     my %mides=();
     my $p = HTML::TokeParser::Simple->new( string => $res->content );
     while ( my $token = $p->get_tag('input' ) ) {

      my $name = $token->get_attr('name');
      my $value = $token->get_attr('value');
      $value =~ s/,/./;
 
      $mides{$name}=$value;

     }

#     $mides{'ample_paper'} = $mides{'marge_esquerra'} + $mides{'amplada'}* $mides{'num_columnes'} + $mides{'pas_horitzontal'} * ($mides{'num_columnes'} - 1)   
     $mides{'ample_paper'} = 210; 
     $mides{'alt_paper'} = 297 ;
     $mides{'marge_dret'} = $mides{'ample_paper'} - ($mides{'marge_esquerra'} + $mides{'num_columnes'}*$mides{'amplada'} + ($mides{'num_columnes'}-1)*$mides{'pas_horitzontal'});
     $mides{'marge_inferior'} = $mides{'alt_paper'} - ($mides{'marge_superior'} + $mides{'num_linies'}*$mides{'alcada'} + ($mides{'num_linies'}-1)*$mides{'pas_vertical'});
   
     
#     print join(',',keys(%mides)) ;
#     print "\n";
# marge_superior,pas_horitzontal,amplada,marge_esquerra,pas_vertical,num_columnes,alcada,etiq_fulla,num_linies
     print "APLI$REF (" , join (',',@mides{qw/amplada alcada num_linies num_columnes ample_paper alt_paper marge_superior marge_esquerra marge_dret marge_inferior/}) , ")\n";


 }
else {
   print STDERR "referència $REF falla : " , $res->status_line, "\n";
}

}

sub unafam {
 my ($FAM) = @_;
my $res = $ua->get("http://www.apli.es/cat/catalog_refs.asp?Fam=$FAM");

# Check the outcome of the response
if ($res->is_success) {
     my %refs=();
     my $p = HTML::TokeParser::Simple->new( string => $res->content );
     while ( my $token = $p->get_tag('a' ) ) {
  
       my $value = $token->get_attr('href');
       if ($value =~ /obrir_popup\('(\d+)'\)/) {
	  $refs{$1}=1;
       }

     }

     for $r (keys(%refs)) {
         unaref($r);
     }      

 }
else {
   print STDERR "familia $FAM falla : " , $res->status_line, "\n";
}

}



################# MAIN

     for $p (@ARGV) {
     #  unaref($p);
	 unafam($p);
     }
