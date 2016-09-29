CC = gcc
CFLAGS = -O0 -std=gnu99 -Wall -fopenmp -g
EXECUTABLE = \
	time_test_baseline time_test_openmp_2 time_test_openmp_4 \
	time_test_avx time_test_avxunroll \
	benchmark_clock_gettime

LDFLAGS = -lm

default: computepi.o
	$(CC) $(CFLAGS) computepi.o time_test.c -DBASELINE -o time_test_baseline $(LDFLAGS)
	$(CC) $(CFLAGS) computepi.o time_test.c -DOPENMP_2 -o time_test_openmp_2 $(LDFLAGS)
	$(CC) $(CFLAGS) computepi.o time_test.c -DOPENMP_4 -o time_test_openmp_4 $(LDFLAGS)
	#$(CC) $(CFLAGS) computepi.o time_test.c -DAVX -o time_test_avx
	#$(CC) $(CFLAGS) computepi.o time_test.c -DAVXUNROLL -o time_test_avxunroll
	$(CC) $(CFLAGS) computepi.o benchmark_clock_gettime.c -o benchmark_clock_gettime $(LDFLAGS)

.PHONY: clean default

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ 

check: default
	time ./time_test_baseline
	time ./time_test_openmp_2
	time ./time_test_openmp_4
	#time ./time_test_avx
	#time ./time_test_avxunroll

gencsv: default
	./benchmark_clock_gettime 500000 > result_clock_gettime.csv
	#for i in `seq 100 5000 25000`; do \
	#	printf "%d " $$i;\
	#	./benchmark_clock_gettime $$i; \
	#done > result_clock_gettime.csv

clean:
	rm -f $(EXECUTABLE) *.o *.s result_clock_gettime.csv
