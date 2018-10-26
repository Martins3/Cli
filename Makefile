CC = clang++
FLAG = -std=c++14 -g
TARTGET = Sharp

CPP_SOURCES = $(wildcard ./timestamp/*.cpp)
CPP_HEADERS = $(wildcard ./timestamp/*.hpp ./utils/Type.hpp) 
OBJ = ${CPP_SOURCES:.cpp=.o} 


# 需要实现如何的效果
${TARTGET}: ${OBJ}
	${CC} -o $@ $^


# all: library.cpp main.cpp
# In this case:
    # $@ evaluates to all
    # $< evaluates to library.cpp
    # $^ evaluates to library.cpp main.cpp
%.o: %.cpp ${HEADERS}
	${CC} ${FLAG} -c $< -o $@


run: ${TARTGET}
	./${TARTGET}


.PHONY: clean run


clean:
	rm -f *.o
	rm -f ${TARTGET}
