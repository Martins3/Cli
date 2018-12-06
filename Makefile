CC = clang++
FLAG = -std=c++14 -g -Wall
BOOST_FLAG= $(FLAG) -lboost_system -pthread -lboost_thread
TARTGET = sharp.out

CPP_HEADERS = $(wildcard ./utils/*.hpp *.hpp ./timer/*.hpp) 

CPP_SOURCES = $(wildcard *.cpp )
BOOST_SOURCES = $(wildcard ./timer/*.cpp )
OBJ = ${CPP_SOURCES:.cpp=.o} 
BOOST_OBJ = ${BOOST_SOURCES:.cpp=.o} 

ALL_OBJ = ${OBJ} ${BOOST_OBJ}

$(OBJ): CXXFLAGS := $(FLAG)
$(BOOST_OBJ): CXXFLAGS := $(BOOST_FLAG)


${TARTGET}: ${ALL_OBJ}
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
