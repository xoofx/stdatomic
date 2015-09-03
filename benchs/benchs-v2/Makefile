
SOURCES = *.txt

DATA = ${SOURCES:.txt=.dat}

%.csv : %.txt
	grep performance $< | grep -v thrds | sed 's/^.*://1' | sed 's/,//g' |sort -n > $@

%-per-thread.csv : %.txt
	grep locks $< | grep -v thrds | sed 's/^.*://1' | sed 's/,//g' |sort -n > $@

%.dat : %.csv
	../summarize.pl < $< > $@

test-all-v2.eps : test-all-v2.gnuplot test-*.dat
	gnuplot test-all-v2.gnuplot

test-relative-v2.eps : test-relative-v2.gnuplot test-*.dat
	gnuplot test-relative-v2.gnuplot
