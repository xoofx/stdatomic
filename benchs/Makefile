
SOURCES = *.txt

DATA = ${SOURCES:.txt=.dat}

%.csv : %.txt
	grep performance $< | grep -v thrds | sed 's/^.*://1' | sed 's/,//g' |sort -n > $@

%.dat : %.csv
	./summarize.pl < $< > $@

test-all.eps : test-all.gnuplot test-*.dat
	gnuplot test-all.gnuplot

test-relative.eps : test-relative.gnuplot test-*.dat
	gnuplot test-relative.gnuplot

test-musl-relative.eps : test-musl-relative.gnuplot test-musl*.dat
	gnuplot test-musl-relative.gnuplot

test-musl-all.eps : test-musl-all.gnuplot test-musl*.dat
	gnuplot test-musl-all.gnuplot

