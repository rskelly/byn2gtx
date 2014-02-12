all: 
	gcc util.c byn.c gtx.c byn2gtx.c -o ../bin/byn2gtx
	g++ -I/usr/local/include -I/opt/local/include -L/usr/local/lib pointsampler.cpp -llas -o ../bin/pointsampler

clean:
	rm -f *.o ../bin/byn2gtx
