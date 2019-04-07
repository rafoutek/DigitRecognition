CC = gcc 
FLAGS = -Wall -O2 -g
CFLAGS = -Wall -O2 -g -c
LDFLAGS = -lm -lglut -lGL -lX11

TARGET = learning_graph

$(TARGET): reconnaissance_chiffres.o graphic.o IHM.o fonctions.o fonctionsTraitementImage.o libisentlib.a
	$(CC) -o $@ $(FLAGS) $^ $(LDFLAGS)

reconnaissance_chiffres.o: reconnaissance_chiffres.c GfxLib.h BmpLib.h NNdefinitions.h fonctions.h fonctionsTraitementImage.h graphic.h
	$(CC) $(CFLAGS) $<

graphic.o: graphic.c GfxLib.h BmpLib.h ESLib.h graphic.h graphicdefinitions.h IHM.h fonctionsTraitementImage.h NNdefinitions.h fonctions.h
	$(CC) $(CFLAGS) $<
	
IHM.o: IHM.c GfxLib.h ESLib.h BmpLib.h graphicdefinitions.h IHM.h fonctionsTraitementImage.h
	$(CC) $(CFLAGS) $<
	
fonctionsTraitementImage.o: fonctionsTraitementImage.c graphicdefinitions.h BmpLib.h fonctionsTraitementImage.h
	$(CC) $(CFLAGS) $<

fonctions.o: fonctions.c NNdefinitions.h fonctions.h BmpLib.h fonctionsTraitementImage.h
	gcc -Wall $(CFLAGS) $<

#lib isen ------------------------------------------
libisentlib.a: BmpLib.o ErreurLib.o ESLib.o GfxLib.o OutilsLib.o
	ar r libisentlib.a BmpLib.o ErreurLib.o ESLib.o GfxLib.o OutilsLib.o
	ranlib libisentlib.a

BmpLib.o: BmpLib.c BmpLib.h OutilsLib.h
	$(CC) $(CFLAGS) BmpLib.c

ESLib.o: ESLib.c ESLib.h ErreurLib.h
	$(CC) $(CFLAGS) ESLib.c

ErreurLib.o: ErreurLib.c ErreurLib.h
	$(CC) $(CFLAGS) ErreurLib.c

GfxLib.o: GfxLib.c GfxLib.h ESLib.h
	$(CC) $(CFLAGS) GfxLib.c -I/usr/include/GL
# Sous MacOSX, commenter la ligne de commande ci-dessus (en mettant un # au debut)
# et de-commenter la ligne de commande ci-apres :
#	gcc -Wall -O2 -c GfxLib.c -Wno-deprecated-declarations

OutilsLib.o: OutilsLib.c OutilsLib.h
	$(CC) $(CFLAGS) OutilsLib.c

#SocketLib.o: SocketLib.c SocketLib.h
#	gcc -Wall -O2 -c SocketLib.c

#ThreadLib.o: ThreadLib.c ThreadLib.h
#	gcc -Wall -O2 -c ThreadLib.c

#VectorLib.o: VectorLib.c VectorLib.h
#	gcc -Wall -O2 -c VectorLib.c -msse3

#WavLib.o: WavLib.c WavLib.h OutilsLib.h
#	gcc -Wall -O2 -c WavLib.c -Wno-unused-result
# Sous MacOSX, commenter la ligne de commande ci-dessus (en mettant un # au debut)
# et de-commenter la ligne de commande ci-apres :
#	gcc -Wall -O2 -c WavLib.c
#lib isen ------------------------------------------

zip:
	tar -cvzf libisentlib.tgz *.[ch] *.bmp *.pdf makefile

clean:
	rm -f *~ *.o

deepclean: clean
	rm -f $(TARGET)

