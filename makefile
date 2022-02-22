CC=cc
CFLAGS= -g -O2 -Wall -I/usr/include/freetype2
LDFLAGS= 
LDLIBS=-lftgl -lglfw -lGLU -lGL -lX11 -lXrandr -lm 

clean	:
	${RM} *.o *~
