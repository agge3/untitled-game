#!/usr/bin/env perl

#use strict;
use warnings;
#use Data::Dumper;

if (0+@ARGV != 2) {
    print "Search all files in a directory for a string, print the full line\n";
    print "FULL PATH of directory: ";
    $ARGV[0] = <>;
    print "String to search for: ";
    $ARGV[1] = <>;
    print "FULL PATH of directory and string can also be passed as arguments! "
        . "directory string\n";
}

#if (substr($ARGV[0], scaler $ARGV[0] - 1) != "/") {
    #    $ARGV[0] . "/";
#}
@dir = glob "$ARGV[0]*";
#print Dumper(@dir);
for my $fh (@dir){
    open(FILE, "<", $fh);
    while (<FILE>) {
        my $line = $_;
        print "$fh: $line" if $line =~ qq/$ARGV[1]/;
    }
    close FILE;
}
