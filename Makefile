all:
	gcc -Wall -fPIC -shared -I /usr/include/freetype2/ -o fixtype.so fixtype.c -ldl

clean:
	-rm -f fixtype.so
