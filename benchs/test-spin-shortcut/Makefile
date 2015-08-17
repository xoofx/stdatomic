
SOURCES = *.txt

DATA = ${SOURCES:.txt=.dat}

%.csv : %.txt
	grep performance $< | grep -v thrds | sed 's/^.*://1' | sed 's/,//g' |sort -n > $@

%.dat : %.csv
	../summarize.pl < $< > $@

test-spin-shortcut-all.eps : test-spin-shortcut-all.gnuplot test-*.dat
	gnuplot test-spin-shortcut-all.gnuplot

test-spin-shortcut-relative.eps : test-spin-shortcut-relative.gnuplot test-*.dat
	gnuplot test-spin-shortcut-relative.gnuplot
