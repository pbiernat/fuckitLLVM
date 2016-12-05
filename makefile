DEBUG = yes

CXX = g++
CXXFLAGS = -fPIC -Wall -Wextra -Wpedantic
LDFLAGS = -fPIC -shared -lstdc++

ifdef DEBUG
	LDFLAGS += -g
else
	CXXFLAGS += -O2
	LDFLAGS += -s
endif

# Add new source files here (and object files if not in the *pass.cpp format)
SRC = exitcodepass.cpp
OBJ = ${SRC:pass.cpp=.o}

all: libfuckitLLVM.so

libfuckitLLVM.so: ${OBJ}
	${CXX} ${LDFLAGS} -o $@ ${OBJ}

# Make a copy of this for each new pass
exitcode.o: exitcodepass.cpp
	${CXX} ${CXXFLAGS} -o $@ -c $^

# Add sample runs for your test here
run: libfuckitLLVM.so
	opt -load $$PWD/$< -fuckit-exitcode samples/exitcode1.ll -o samples/exitcode1.fucked.ll -S

clean-run:
	rm -f samples/*.fucked.ll

clean:
	rm -f libfuckitLLVM.so ${OBJ}

.PHONY: all clean run clean-run
