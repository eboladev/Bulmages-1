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
 my ($REF, $refs, @path) = @_;
 print STDERR "baixo ref\n";
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
     if ($mides{'num_columnes'} && $mides{'num_linies'}) {
        $mides{'ample_paper'} = 210; 
        $mides{'alt_paper'} = 297 ;
        $mides{'marge_dret'} = $mides{'ample_paper'} - ($mides{'marge_esquerra'} + $mides{'num_columnes'}*$mides{'amplada'} + ($mides{'num_columnes'}-1)*$mides{'pas_horitzontal'});
        $mides{'marge_inferior'} = $mides{'alt_paper'} - ($mides{'marge_superior'} + $mides{'num_linies'}*$mides{'alcada'} + ($mides{'num_linies'}-1)*$mides{'pas_vertical'});
     } else {
        $mides{'ample_paper'} = $mides{'amplada'}; 
        $mides{'alt_paper'} = $mides{'alcada'} ;
        $mides{'marge_dret'} = 0;
        $mides{'marge_inferior'} = 0;
        $mides{'marge_esquerra'} = 0;
        $mides{'marge_superior'} = 0; 
        $mides{'num_columnes'} = $mides{'num_linies'} = 1;
     }
     my $out = STDOUT; 
     if ( (grep { $_ < 0 }  values(%mides))
          ||(grep { $_ < 1 } @mides{qw/amplada alcada num_linies num_columnes/}))  {
        $out = STDERR 
     } 
#     print join(',',keys(%mides)) ;
#     print "\n";
# marge_superior,pas_horitzontal,amplada,marge_esquerra,pas_vertical,num_columnes,alcada,etiq_fulla,num_linies
#     push @$refs, ("APLI$REF (" . join (',',@mides{qw/amplada alcada num_linies num_columnes ample_paper alt_paper marge_superior marge_esquerra marge_dret marge_inferior/}) . ")\n" );
     $nom = sprintf("%scm x %scm",$mides{'amplada'}/10 , $mides{'alcada'}/10);    
     for $k  (keys(%mides)) {
	 if ($mides{$k} =~ /\./) {
	   $mides{$k} = sprintf("%.2f",$mides{$k});
         }
     }
     print $out ("APLI$REF ". join(' - ',@path) .", ". $nom . " (" . join (',',@mides{qw/amplada alcada num_linies num_columnes ample_paper alt_paper marge_superior marge_esquerra marge_dret marge_inferior/}) . ")\n" );
     $out->flush;
 }
else {
   print STDERR "referència $REF falla : " , $res->status_line, "\n";
}

}

sub unafam {
 my ($FAM) = @_;
my $pre_url = "http://www.apli.es/cat/catalog_refs.asp?Fam=";
 my $url = $pre_url . $FAM;
 print STDERR "baixo $url\n";
my $res = $ua->get($url );

# Check the outcome of the response
if ($res->is_success) {
     my %refs=();
     my @path=();
     my $soc_al_path = 0;
     my $p = HTML::TokeParser::Simple->new( string => $res->content );
     my $token= $p->get_tag('a' ) ;
     while ($token && ( $token->get_attr('href') ne "catalog_refs.asp?Fam=$FAM")) {
	 my $text= $p->get_token;
      
         if ($text->is_text ) {
             if ($soc_al_path) {
		 push @path , $text->as_is ;
	     }
            $soc_al_path ||= ($text->as_is eq 'ETIQUETES');   
	 }
         $token = $p->get_tag('a' ) ;
     }  
     print STDERR ("path és " . join('-',@path) . "\n"); 
     while (  $token = $p->get_tag('a' ) ) {
       my $value = $token->get_attr('href');
       if ($value =~ /obrir_popup\('(\d+)'\)/) {
	  $refs{$1}=\@path;
       }

     }

     for $r (keys(%refs)) {
         print STDERR "provo $r\n";
         unaref($r,[], @path);
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
