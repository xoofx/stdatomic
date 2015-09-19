
SOURCES = *.txt

DATA = ${SOURCES:.txt=.dat}

%.csv : %.txt
	grep performance $< | grep -v thrds | sed 's/^.*://1' | sed 's/,//g' |sort -n > $@

%-per-thread.csv : %.txt
	grep locks $< | grep -v thrds | sed 's/^.*://1' | sed 's/,//g' |sort -n > $@

%.dat : %.csv
	../summarize.pl < $< > $@

%.eps : %.gnuplot
	gnuplot -e "set output '$@'" $<

test-spin-shortcut-all.eps : test-spin-shortcut-all.gnuplot test-*.dat
	gnuplot test-spin-shortcut-all.gnuplot

test-spin-no-shortcut-all.eps : test-spin-no-shortcut-all.gnuplot test-*.dat
	gnuplot test-spin-no-shortcut-all.gnuplot

test-spin0-all.eps : test-spin0-all.gnuplot test-*.dat
	gnuplot test-spin0-all.gnuplot

test-spin-shortcut-relative.eps : test-spin-shortcut-relative.gnuplot test-*.dat
	gnuplot test-spin-shortcut-relative.gnuplot
