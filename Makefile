CC = clang++
FLAG = -std=c++14 -g -Wall -I.
BOOST_FLAG=
TARTGET = sharp.out


CPP_SOURCES = $(wildcard *.cpp )
CPP_HEADERS = $(wildcard ./utils/*.hpp) 
OBJ = ${CPP_SOURCES:.cpp=.o} 


${TARTGET}: ${OBJ}
	${CC} ${FLAG} -o $@ $^


%.o: %.cpp ${CPP_HEADERS}
	${CC} ${FLAG} -c $< -o $@


run: ${TARTGET}
	./${TARTGET}

test: ${TARTGET}
	${TARTGET}

.PHONY: clean run

clean:
	rm -f *.o
	rm -f ${TARTGET}
