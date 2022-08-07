OBJS = *.cpp
COMPILER = g++
COMPILER_FLAGS = -w
LINKER_FLAGS = -std=c++20 -lSDL2 -lSDL2_image
OBJ_NAME = main
all : $(OBJS)
	$(COMPILER) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME); ./main
