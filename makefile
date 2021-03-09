ifdef CLANG
CC = clang++ 
CFLAGS = -mcx16 -O2 -I../c++ -fcilkplus -ldl -std=c++17 -march=native
else
CC = g++
# works with g++ (GCC) 5.4.1
CFLAGS = -O3 -I../c++ -mcx16 -march=native -fcilkplus -std=c++11 -w
endif

all:	test

test:		test.cpp
	$(CC) $(CFLAGS) -DNDEBUG test.cpp -o test

clean:
	rm -f test