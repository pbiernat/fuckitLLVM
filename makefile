DEBUG = yes

CXX = g++
CFLAGS = -fPIC -Wall -Wextra -Wpedantic
LDFLAGS = -fPIC -shared
ifdef DEBUG
	LDFLAGS += -g
else
	CFLAGS += -O2
	LDFLAGS += -s
endif

# Add new source files here
SRC = exitcode.cpp
OBJ = ${SRC:.cpp=.o}

all: libfuckitLLVM.so

libfuckitLLVM.so: ${OBJ}
	$(CXX) ${LDFLAGS} -o $@ ${OBJ}

# Make a copy of this for each new pass
exitcode.o: exitcodepass.cpp
	$(CXX) -DfuckitLLVM_EXPORTS ${CFLAGS} -o exitcode.o -c exitcodepass.cpp

options:
	@echo "CFLAGS = ${CFLAGS}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "CXX = ${CXX}"

# Add sample runs for your test here
run: libfuckitLLVM.so
	opt -load $$PWD/libfuckitLLVM.so -fuckit-exitcode samples/exitcode1.ll -o samples/exitcode1.fucked.ll -S

clean-run:
	rm -f samples/*.fucked.ll

clean:
	rm -f libfuckitLLVM.so ${OBJ}

.PHONY: all options clean run clean-run
