CXXFLAGS =	-I include/ -O2 -g -Wall -fmessage-length=0

SRC_DIR_LIB=src
SRC_DIR_EXE=main
OBJ_DIR_LIB=obj/lib
OBJ_DIR_EXE=obj/exe
BIN_DIR=bin
HEAD_DIR=include

SRC_FILES_LIB = $(wildcard $(SRC_DIR_LIB)/*.cpp)
SRC_FILES_EXE = $(wildcard $(SRC_DIR_EXE)/*.cpp)
HEAD_FILES    = $(wildcard $(HEAD_DIR)/*.h)

OBJ_FILES_LIB = $(patsubst $(SRC_DIR_LIB)/%.cpp,$(OBJ_DIR_LIB)/%.o,$(SRC_FILES_LIB))
OBJ_FILES_EXE = $(patsubst $(SRC_DIR_EXE)/%.cpp,$(OBJ_DIR_EXE)/%.o,$(SRC_FILES_EXE))

EXEC_FILES  = $(patsubst $(SRC_DIR_EXE)/%.cpp,$(BIN_DIR)/%,$(SRC_FILES_EXE))

$(OBJ_DIR_EXE)/%.o:	$(SRC_DIR_EXE)/%.cpp $(OBJ_FILES_LIB) $(HEAD_FILES)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(OBJ_DIR_LIB)/%.o:	$(SRC_DIR_LIB)/%.cpp $(HEAD_FILES)
	$(CXX) -o $@ -c $< $(CXXFLAGS)
	
$(BIN_DIR)/%:	$(OBJ_DIR_EXE)/%.o
	$(CXX) -o $@ -s $(subst $(BIN_DIR)/,$(OBJ_DIR_EXE)/,$@).o $(OBJ_FILES_LIB) $(HEAD_FILES) $(LDFLAGS)

all:	$(EXEC_FILES) $(OBJ_FILES_LIB)
	@echo "BUILD SUCCESSFUL; CLEANING"
	rm -f obj/lib/*.o
	rm -f obj/exe/*.o

show:
	@echo "SRC_FILES_LIB=$(SRC_FILES_LIB)"
	@echo "HEADERS=$(HEAD_FILES)"
	@echo "SRC_FILES_EXE=$(SRC_FILES_EXE)"
	@echo "$(EXEC_FILES)"

clean:
	rm -rf *.o
