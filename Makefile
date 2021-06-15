CXXFLAGS = -I include/ -O2 -g -Wall -fmessage-length=0 -std=c++11 -pthread

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

CLEAR_OLD:
	rm -f bin/*
	rm -f Client
	rm -f RegServ

$(OBJ_DIR_EXE)/%.o:	$(SRC_DIR_EXE)/%.cpp $(OBJ_FILES_LIB) $(HEAD_FILES)
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(OBJ_DIR_LIB)/%.o:	$(SRC_DIR_LIB)/%.cpp $(HEAD_FILES)
	$(CXX) -o $@ -c $< $(CXXFLAGS)
	
$(BIN_DIR)/%:	$(OBJ_DIR_EXE)/%.o
	$(CXX) -o $@ -s $(subst $(BIN_DIR)/,$(OBJ_DIR_EXE)/,$@).o $(OBJ_FILES_LIB) $(HEAD_FILES) $(LDFLAGS) $(CXXFLAGS)

all:	CLEAR_OLD $(EXEC_FILES) $(OBJ_FILES_LIB)
	@echo "Cleaning and Symlinking."
	rm -f obj/lib/*.o
	rm -f obj/exe/*.o
	ln -s ./bin/Client Client
	ln -s ./bin/RegServ RegServ
	@echo "****************************************************************************"
	@echo "************************ BUILD COMPLETE ************************************"
	@echo "Start registration server by running ./RegServ"
	@echo ""
	@echo "Then, start clients with './Client <code> <hostname>'"
	@echo "Example: To start client 'A' and connect to a registration server on 10.0.0.3"
	@echo "Execute Command:        ./Client a 10.0.0.3"
	@echo "****************************************************************************"

show:
	@echo "SRC_FILES_LIB=$(SRC_FILES_LIB)"
	@echo "HEADERS=$(HEAD_FILES)"
	@echo "SRC_FILES_EXE=$(SRC_FILES_EXE)"
	@echo "$(EXEC_FILES)"

clean:
	rm -rf *.o
