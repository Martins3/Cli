CC = clang++
FLAG = -std=c++11 -g -Wall
TARTGET = /home/shen/.Application/sharp

BUILD_PROJECT=yes


CPP_HEADERS = $(wildcard *.hpp) 
CPP_SOURCES = $(wildcard *.cpp)
OBJ = ${CPP_SOURCES:.cpp=.o} 


${TARTGET}: ${OBJ}
	${CC} ${FLAG} -o $@ ${OBJ}


%.o: %.cpp ${CPP_HEADERS}
	${CC} ${FLAG} -c $< -o $@


run: ${TARTGET}
	${TARTGET}

test: ${TARTGET}
	${TARTGET}

subsystem:
	$(MAKE) -C timer

.PHONY: clean run test subsystem



clean:
	rm -f *.o
	rm -f ./utils/*.o
	rm -f ${TARTGET}
