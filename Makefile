all:
	g++ -O2 -g p1.cc -o p1
cl:
	rm -f p1
t:
	./p1 -ast test > errors