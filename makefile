CC = gcc
CXX = g++
CXXFLAGS = -fPIC -Wall -Wextra -Wpedantic
LDFLAGS = -fPIC -shared -lstdc++

# Add new source files here
SRC = exitcodepass.cpp stderrpass.cpp signalpass.cpp
OBJ = ${SRC:pass.cpp=.o}

all: libfuckitLLVM.so fuckitfuncs.o

libfuckitLLVM.so: ${OBJ}
	${CXX} ${LDFLAGS} -o $@ ${OBJ}

# Make a copy of this for each new pass
exitcode.o: exitcodepass.cpp
	${CXX} ${CXXFLAGS} -o $@ -c $^

stderr.o: stderrpass.cpp
	${CXX} ${CXXFLAGS} -o $@ -c $^

signal.o: signalpass.cpp
	${CXX} ${CXXFLAGS} -o $@ -c $^

# Add sample runs for your test here
samples: all
	# Exit code passes
	opt -load $$PWD/libfuckitLLVM.so -fuckit-exitcode-exit -fuckit-exitcode-return samples/exitcode1.ll -o samples/exitcode1.fucked.ll -S
	clang samples/exitcode1.fucked.ll -o samples/exitcode1.fucked
	opt -load $$PWD/libfuckitLLVM.so -fuckit-exitcode-exit -fuckit-exitcode-return samples/exitcode2.ll -o samples/exitcode2.fucked.ll -S
	clang samples/exitcode2.fucked.ll -o samples/exitcode2.fucked
	# Signal handler pass
	opt -load $$PWD/libfuckitLLVM.so -fuckit-signals samples/segfault.ll -o samples/segfault.fucked.ll -S
	clang -c samples/segfault.fucked.ll -o samples/segfault.fucked.o
	${CC} samples/segfault.fucked.o fuckitfuncs.o -o samples/segfault.fucked
	opt -load $$PWD/libfuckitLLVM.so -fuckit-signals samples/killme.ll -o samples/killme.fucked.ll -S
	clang -c samples/killme.fucked.ll -o samples/killme.fucked.o
	${CC} samples/killme.fucked.o fuckitfuncs.o -o samples/killme.fucked
	# stderr pass
	opt -load $$PWD/libfuckitLLVM.so -fuckit-stderr samples/stderr.ll -o samples/stderr.fucked.ll -S
	clang -c samples/stderr.fucked.ll -o samples/stderr.fucked.o
	${CC} samples/stderr.fucked.o fuckitfuncs.o -o samples/stderr.fucked

clean-samples:
	rm -f samples/*.fucked.ll samples/*.fucked samples/*.fucked.o

clean: clean-samples
	rm -f libfuckitLLVM.so ${OBJ} fuckitfuncs.o

.PHONY: all clean samples clean-samples
