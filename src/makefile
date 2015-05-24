all: byn2gtx pointsampler

byn2gtx:
	gcc -lm util.c byn.c gtx.c byn2gtx.c -o ../bin/byn2gtx

neggtx:
	gcc util.c gtx.c neggtx.c -o ../bin/neggtx
	
pointsampler:
	g++ -I/usr/local/include -I/opt/local/include -L/usr/local/lib pointsampler.cpp -llas -o ../bin/pointsampler

clean:
	rm -f *.o ../bin/byn2gtx ../bin/neggtx ../bin/pointsampler
