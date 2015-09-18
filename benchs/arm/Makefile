
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

test-arm-all.eps : test-arm-all.gnuplot test-*.dat
	gnuplot test-arm-all.gnuplot

test-arm-relative.eps : test-arm-relative.gnuplot test-*.dat
	gnuplot test-arm-relative.gnuplot
