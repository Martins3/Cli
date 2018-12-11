CC = clang++
FLAG = -std=c++14 -g -Wall
LINK_FLAG = $(FLAG) -pthread -lboost_system -lboost_thread
TARTGET = hubachelar.timer.out

CPP_HEADERS = $(wildcard *.hpp) 
CPP_SOURCES = $(wildcard *.cpp  ./test/main.cpp)
OBJ = ${CPP_SOURCES:.cpp=.o} 

ifndef BUILD_PROJECT
${TARTGET}: ${OBJ}
	${CC} ${LINK_FLAG} -o $@ $^
	echo "Build from the outside"
else
# gcc -shared -o libfoo.so foo.o
${TARTGET}: ${OBJ}
	${CC} ${LINK_FLAG} -o $@ $^
endif


%.o: %.cpp ${CPP_HEADERS}
	${CC} ${FLAG} -c $< -o $@


server: ${TARTGET}
	./${TARTGET} server

client: ${TARTGET}
	./${TARTGET} client
	

.PHONY: clean run server

clean:
	rm -f *.o
	rm -f *.out
	rm -f ${TARTGET}
