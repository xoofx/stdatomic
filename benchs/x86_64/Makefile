
SOURCES = *.txt

DATA = ${SOURCES:.txt=.dat}

CSV = ${SOURCES:.txt=.csv}

%.csv : %.txt
	grep performance $< | grep -v thrds | sed 's/^.*://1' | sed 's/,//g' |sort -n > $@

%.dat : %.csv
	../summarize.pl < $< > $@

%.eps : %.gnuplot ${DATA}
	gnuplot -e "set output '$@'" $<

data : ${DATA}

csv : ${CSV}
