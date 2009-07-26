#!/usr/bin/perl -w
while(<>) {
  chomp;
  print "  <item>\n",
        '    <property name="text">',"\n",
        "     <string>$_</string>\n",
        "    </property>\n",
        "  </item>\n";
}
