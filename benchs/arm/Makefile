
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

%.png : %.gnuplot
	gnuplot -e "set output '$@'" -e "set terminal png size 1280,960" $<

test-arm-u64.eps test-arm-u64.png : test-arm-u64.gnuplot test-*.dat

test-arm-relative.eps test-arm-relative.png : test-arm-relative.gnuplot test-*.dat
	gnuplot test-arm-relative.gnuplot
