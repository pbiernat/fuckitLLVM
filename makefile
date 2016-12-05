CC = gcc
CXX = g++
CXXFLAGS = -fPIC -Wall -Wextra -Wpedantic
LDFLAGS = -fPIC -shared -lstdc++

# Add new source files here (and object files if not in the *pass.cpp format)
SRC = exitcodepass.cpp signalpass/sigpass.cpp
OBJ = ${SRC:pass.cpp=.o}

all: libfuckitLLVM.so signalpass/siglib/sigs.o

libfuckitLLVM.so: ${OBJ}
	${CXX} ${LDFLAGS} -o $@ ${OBJ}

# Make a copy of this for each new pass
exitcode.o: exitcodepass.cpp
	${CXX} ${CXXFLAGS} -o $@ -c $^

signalpass/sig.o: signalpass/sigpass.cpp
	${CXX} ${CXXFLAGS} -o $@ -c $^

# Add sample runs for your test here
samples: all
	# Exit code passes
	opt -load $$PWD/libfuckitLLVM.so -fuckit-exitcode-exit -fuckit-exitcode-return samples/exitcode1.ll -o samples/exitcode1.fucked.ll -S
	opt -load $$PWD/libfuckitLLVM.so -fuckit-exitcode-exit -fuckit-exitcode-return samples/exitcode2.ll -o samples/exitcode2.fucked.ll -S
	# Signal handler passes
	opt -load $$PWD/libfuckitLLVM.so -fuckit-signals samples/segfault.ll -o samples/segfault.fucked.ll -S
	clang -c samples/segfault.fucked.ll -o samples/segfault.fucked.o
	${CC} samples/segfault.fucked.o signalpass/siglib/sigs.o -o samples/segfault.fucked

clean-samples:
	rm -f samples/*.fucked.ll samples/*.fucked samples/*.fucked.o

clean: clean-samples
	rm -f libfuckitLLVM.so ${OBJ} signalpass/siglib/sigs.o

.PHONY: all clean samples clean-samples
