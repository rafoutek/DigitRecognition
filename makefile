# Read the comments for compiling with macOS

CC = gcc
CFLAGS = -Wall -O3 -c
LDFLAGS = graphiclib.a  -lm -lglut -lGL -lX11
# For macOS, comment the preceding command line (put a # at the beginning)
# and uncomment the following command line:
#LDFLAGS = graphiclib.a -framework OpenGL -framework GLUT
TARGET = reconnaissance_chiffres

.PHONY: clean deepclean

$(TARGET): $(TARGET).o graphiclib.a fonctions.o
	$(CC) -o $@.exe $^ $(LDFLAGS)

$(TARGET).o: $(TARGET).c definitions.h
	$(CC) $(CFLAGS) $<

fonctions.o: fonctions.c definitions.h fonctions.h
	gcc -Wall $(CFLAGS) -c fonctions.c

graphiclib.a: BmpLib.o ErreurLib.o ESLib.o GfxLib.o  OutilsLib.o
	ar r graphiclib.a BmpLib.o ErreurLib.o ESLib.o GfxLib.o OutilsLib.o
	ranlib graphiclib.a

BmpLib.o: BmpLib.c BmpLib.h OutilsLib.h
	gcc -Wall -O2 -c BmpLib.c

ESLib.o: ESLib.c ESLib.h ErreurLib.h
	gcc -Wall -O2 -c ESLib.c

ErreurLib.o: ErreurLib.c ErreurLib.h
	gcc -Wall -O2 -c ErreurLib.c

GfxLib.o: GfxLib.c GfxLib.h ESLib.h
	gcc -Wall -O2 -c GfxLib.c -I/usr/include/GL
# For macOS, comment the preceding command line (put a # at the beginning)
# and uncomment the following command line:
#	gcc -Wall -O2 -c GfxLib.c -Wno-deprecated-declarations

OutilsLib.o: OutilsLib.c OutilsLib.h
	gcc -Wall -O2 -c OutilsLib.c


zip:
	tar -cvzf $(TARGET).tgz *.[ch] *.pdf makefile

clean:
	rm -f *~ *.o

deepclean: clean
	rm -f $(TARGET) graphiclib.a
