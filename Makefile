all:
	g++ -O2 -g p1.cc -o p1
cl:
	rm -f p1
test:
	chmod a+x difftest.pl
	perl difftest.pl -1 "rpal/rpal -ast -noout FILE" -2 "./p1 -ast FILE" -t ./rpal/tests/