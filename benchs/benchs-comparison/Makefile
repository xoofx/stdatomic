
SOURCES = *.txt

DATA = ${SOURCES:.txt=.dat}

%.csv : %.txt
	grep performance $< | grep -v thrds | sed 's/^.*://1' | sed 's/,//g' |sort -n > $@

%.dat : %.csv
	../summarize.pl < $< > $@

test-benchs-all.eps : test-benchs-all.gnuplot test-*.dat
	gnuplot test-benchs-all.gnuplot

test-benchs-relative.eps : test-benchs-relative.gnuplot test-*.dat
	gnuplot test-benchs-relative.gnuplot

test-benchs-O3-all.eps : test-benchs-O3-all.gnuplot test-*.dat
	gnuplot test-benchs-O3-all.gnuplot

test-benchs-O3-relative.eps : test-benchs-O3-relative.gnuplot test-*.dat
	gnuplot test-benchs-O3-relative.gnuplot
