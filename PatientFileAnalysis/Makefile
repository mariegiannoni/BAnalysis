EXE = PatientFileAnalysis

SRC_DIR = src
OBJ_DIR = obj


MYSQL_PATH = /usr

MYSQL_HEAD_DIR = -I$(MYSQL_PATH)/include/mysql
MYSQL_LIB_DIR = -L$(MYSQL_PATH)/lib/mysql
MYSQL_LINKING = -lmysqlclient


# N.B: clang can be used instead of gcc:
CC = g++
CPPFLAGS = -std=c++11 -Wall -O2 $(MYSQL_HEAD_DIR)
CFLAGS =
LDFLAGS =
LDLIBS = $(MYSQL_LIB_DIR) $(MYSQL_LINKING)


SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)


# The following names are not associated with files:
.PHONY: all clean

# All executables to be created:
all: $(EXE)

# Linking the program:
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Compiling other source files:
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Cleaning with 'make clean' the object files:
clean:
	rm -fv $(EXE) $(OBJ_DIR)/*
