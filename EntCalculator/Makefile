CC=gcc
CFLAGS=-Wall -g
DEPS = entropy.h levenshtein.h minIni.h readConfig.h segmentation.h sequence.h tools.h wavelet.h minGlue.h
OBJ = main.o entropy.o levenshtein.o minIni.o readConfig.o segmentation.o sequence.o tools.o wavelet.o
LIBS=-lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

entCalculator: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm *.o entCalculator
