CXX = g++
CFLAG = -Wno-write-strings -g

all: main

main: main.cpp
	${CXX} ${LIB} ${INC} $< -o $@ ${CFLAG}
	${CXX} ${LIB} ${INC} $< -o $@_g ${CFLAG} -DDEBUG

clean:
	rm -r main main_g
