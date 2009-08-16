#!/usr/bin/perl -w

use XML::EasyOBJ;


sub mm {
#passa a milímetres
    my ($mida,$omisio) = @_;
    my %unitats = ( mm => 1, cm => 10, dm => 100, m => 1000, 
                    dam => 10000, hm => 100000, km => 1000000, 
                    in => 25.4 , yd => 914.4 , ft => 304.8, 
                    pt=> 0.3527777777778 , pc=>4.2333333333 );
    my $regex = join('|', keys(%unitats));
    if ( $mida =~ /^\s*(\d*\.\d+|\d+\.?)\s*($regex)\s*$/io ) {
	return "0$1" * $unitats{$2} ;
    } else {
        if ( $mida =~ /^\s*(\d*\.\d+|\d+\.?)\s*$/o) {
            return "0$1" * $unitats{'pt'};
 	} else {
           if ( $mida =~ /^\s*($regex)\s*$/io ) {
               return $unitats{$1};
	   } else {
              if (defined($omisio) && ((!defined($mida)) || ($mida =~ /^\s*$/))) {
                  return $omisio;
              } else {
                  die "unitat desconeguda $mida\n";
              }
           }
	}
    }
}

sub mm_o_no {
#passa a milímetres
    my ($mida,$omisio) = @_;
  }

my %models_paper =( Other => {} );

if ($#ARGV < 1) {
    die "ús: glabels.pl fitxer_mides_paper.xml fitxer_mides_etiquetes1.xml ... fitxer_mides_etiquetes2.xml\n per exemple glabels.pl /usr/share/glabels/templates/paper-sizes.xml /usr/share/glabels/templates/*\n";
}

$fitxer_paper = shift @ARGV ;
my $papers = new XML::EasyOBJ($fitxer_paper);
for $ps ($papers->getElement('Paper-size')) {
    my $nom = $ps->getAttr('id');
    my $mides = { ample_paper => mm($ps->getAttr('width')) , 
                 alt_paper => mm($ps->getAttr('height')) };
    $models_paper{$nom} =$mides;
#    print STDERR "paper $nom\n";
}
#print STDERR "ja tinc papers";
for $f (@ARGV) {
 # print STDERR "plantilla $f\n";
  my $doc = new XML::EasyOBJ($f);
  for $t ($doc->Template) {
      my $nom = $t->getAttr('name'). " ". $t->getAttr('description') . " " ;
  #    print STDERR "template $nom\n";
      my $mides = $models_paper{ $t->getAttr('size') } ;
      if ($t->getAttr('width')) {
	  $mides->{'ample_paper'} = mm($t->getAttr('width'));          
      }
      if ($t->getAttr('height')) {
          $mides->{'alt_paper'} = mm($t->getAttr('height'));
      }
      my @r = $t->getElement('Label-rectangle');
      if (@r) {
	my $r = $r[0];
        if ($r) {
          #print STDERR ("rectangle \n");
	  $mides->{'amplada'} = mm($r->getAttr('width'));
          $mides->{'alcada'} = mm($r->getAttr('height'));
          my @l = $r->Layout;
          #print STDERR ("layouts: ". $#l . "\n");
          if ($#l == 0) { 

            my $l = $l[0];
            if ($l) {
              # print STDERR "layout \n";
	      $mides->{'num_linies'} = $l->getAttr('ny');
              $mides->{'num_columnes'} = $l->getAttr('nx');
              $mides->{'marge_superior'} = mm($l->getAttr('y0'),0);
              $mides->{'marge_esquerra'} = mm($l->getAttr('x0'),0);
              $mides->{'marge_dret'} = $mides->{'ample_paper'} - ($mides->{'marge_esquerra'} + $mides->{'amplada'} + ($mides->{'num_columnes'}-1) * mm($l->getAttr('dx'),$mides->{'amplada'}));
              $mides->{'marge_inferior'} = $mides->{'alt_paper'} - ($mides->{'marge_superior'} + $mides->{'alcada'} + ($mides->{'num_linies'}-1) * mm($l->getAttr('dy'),$mides->{'alcada'}));
                    
              my $out = STDOUT; 
              if ( (grep { $_ < 0 }  values(%$mides))
                    ||(grep { $_ < 1 } @{$mides}{qw/amplada alcada num_linies num_columnes/}))  {
                 $out = STDERR 
              } 
              #     print join(',',keys(%mides)) ;
              #     print "\n";
         

              for $k  (keys(%$mides)) {
	         if ($mides->{$k} =~ /\./) {
	             $mides->{$k} = sprintf("%.2f",$mides->{$k});
                 }
              }
#              $nom .= sprintf("%scm x %scm",$mides->{'amplada'}/10 , $mides->{'alcada'}/10);    
#              print $out ( $nom . " (" . join (',',@{$mides}{qw/amplada alcada num_linies num_columnes ample_paper alt_paper marge_superior marge_esquerra marge_dret marge_inferior/}) . ")\n" );
              for $a ($t , $t->Alias) {
                 $nom = $a->getAttr('name'). " ". $t->getAttr('description') . " " ;
                 $nom .= sprintf("%scm x %scm",$mides->{'amplada'}/10 , $mides->{'alcada'}/10);   
                 $nom =~ s/\s+/ /g; 
                 print $out ( $nom . " (" . join (',',@{$mides}{qw/amplada alcada num_linies num_columnes ample_paper alt_paper marge_superior marge_esquerra marge_dret marge_inferior/}) . ")\n" );
              
	      }
              $out->flush; 
              
            }     
	  }
	}
      } 
      

      
  }
  
}
