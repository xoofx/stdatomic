#!/bin/sh -f
#  This may look like complete nonsense, but it really is -*- mode: perl; coding: utf-8 -*-
eval 'exec perl -wS -x $0 ${1+"$@"}'
if 0;               #### for this magic, see findSvnAuthors ####
#!perl

my $x = 1;
my $y = 6;

my $line = <>;
chomp $line;
my @words = split(/\s+/, $line);

my $act = $words[$x];
my $n = 1;
my $est = $words[$y];
my $sum = 0;
my $sum2 = 0;

sub printstat() {
    if ($n == 1) {
        print "\t${act}\t${est}\t0\n";
    } else {
        my $mean = $est - ($sum / $n);
        my $var = ($sum2 - ($sum * $sum) / $n) / ($n - 1);
        my $dev = sqrt($var);
        printf("\t%d\t%e\t%e\t%d\n", ${act}, ${mean}, ${dev}, $n);
    }
}

foreach my $line (<>) {
    my @words = split(/\s+/, $line);
    if ($words[$x] == $act) {
        ++$n;
        my $val = ($est - $words[$y]);
        $sum += $val;
        $sum2 += $val * $val;
    } else {
        printstat;
        $act = $words[$x];
        $est = $words[$y];
        $n = 1;
        $sum = 0;
        $sum2 = 0;
    }
}

printstat;
