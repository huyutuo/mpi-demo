#Makefile
FC = mpif90
OPT = -O2
LIBPATH = /home/hyt/mpi/mpi-demo/demo
MYLIB = mympi
LIBMYMPI = libmympi.a
LIBOBJS = mybegin.o myend.o ring.o datatype.o mpistruct.o \
	  diagomal.o proc2d.o sscan.o gemmv.o iteration.o
CC = mpicc

mybegin.o: mybegin.c
	$(CC) -c $(OPT) mybegin.c

myend.o: myend.c
	$(CC) -c $(OPT) myend.c

ring.o: ring.c
	$(CC) -c $(OPT) ring.c

datatype.o: datatype.c
	$(CC) -c $(OPT) datatype.c

mpistruct.o: mpistruct.c
	$(CC) -c $(OPT) mpistruct.c

diagomal.o: diagomal.c
	$(CC) -c $(OPT) diagomal.c

proc2d.o: proc2d.c
	$(CC) -c $(OPT) proc2d.c

sscan.o: sscan.c
	$(CC) -c $(OPT) sscan.c

gemmv.o: gemmv.c
	$(CC) -c $(OPT) gemmv.c

iteration.o: iteration.c
	$(CC) -c $(OPT) iteration.c

main.o: main.c
	$(CC) -c $(OPT) -c main.c

main: main.o
	$(CC) $(OPT) -o main main.o -L$(LIBPATH) -l$(MYLIB) -lm

mylib: $(LIBOBJS)
	ar rcs $(LIBMYMPI) $(LIBOBJS)

clean: 
	rm -f *.o *.a a.out main


