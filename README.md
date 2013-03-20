Compile:
	make

Clean:
	make clean

Run test:
	chmod a+x difftest.pl
	perl difftest.pl -1 "rpal/rpal -ast -noout FILE" -2 "./p1 -ast FILE" -t ./rpal/tests/


Supported Switch;
-ast
-l