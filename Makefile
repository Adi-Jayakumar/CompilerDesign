CC		:= g++

# -fsanitize=leak -fsanitize=address -fsanitize=undefined
# `llvm-config-12 --cxxflags --ldflags --system-libs --libs all` expands to:

# -I/usr/lib/llvm-12/include -std=c++14 -fno-exceptions -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
# -L/usr/lib/llvm-12/lib 
# -lLLVM-12

# the use of C++14 is problematic as I want to use C++17 features so I define my own flag
LL_FLAGS := -I/usr/lib/llvm-12/include -std=c++17 -fno-exceptions -D_GNU_SOURCE	\
			-D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS\
			-L/usr/lib/llvm-12/lib \
			-lLLVM-12\


C_FLAGS := -g -fsanitize=leak -fsanitize=address -fsanitize=undefined -O0 -Wall -Wextra -Wpedantic -march=native \
-Wno-unused-function -Wno-unknown-warning-option $(LL_FLAGS)

FLAGS_FOR_DCS_SYSTEMS=-stdlib=libstdc++ -cxx-isystem /local/java/gcc-9.2.0/include/c++/9.2.0/ -cxx-isystem /local/java/gcc-9.2.0/include/c++/9.2.0/x86_64-pc-linux-gnu/ -L/local/java/gcc-9.2.0/lib64 -L/local/java/gcc-9.2.0/lib/gcc/x86_64-pc-linux-gnu/9.2.0/  

BIN		:= bin
SRC		:= src
INCLUDE := include
LIB		:= lib

LIBRARIES	:=

EXECUTABLE	:= mccomp

all: $(BIN)/$(EXECUTABLE)

clean:
	$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*
	$(CC) $(C_FLAGS) -I$(INCLUDE) $^ -o $@