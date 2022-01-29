aprox: main.o splines.o points.o aproksymator_czebyszew.o gaus/libge.a
	$(CC) -o aprox  main.o splines.o points.o aproksymator_czebyszew.o -L gaus -l ge

aproksymator_czebyszew.o: makespl.h points.h gaus/piv_ge_solver.h
	$(CC) -I gaus -c aproksymator_czebyszew.c

.PHONY: clean

clean:
	-rm *.o aprox
