ifdef CLANG
CC = clang++ 
CFLAGS = -mcx16 -O3 -I../c++ -fcilkplus -ldl -std=c++17 -march=native -w
else
CC = g++
# works with g++ (GCC) 5.4.1
CFLAGS = -O3 -I../c++ -mcx16 -march=native -fcilkplus -std=c++17 -w
endif

all:	test

test:		test.cpp
	$(CC) $(CFLAGS) -DNDEBUG test.cpp -o test

clean:
	rm -f test