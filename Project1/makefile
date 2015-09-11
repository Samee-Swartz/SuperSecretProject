SRC_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(SRC_FILES:.cpp=.o)))

CC_FLAGS := -c

PROGRAM_NAME := program

all: program
	echo "Compilation Complete!"

clean:
	rm -f $(OBJ_FILES) $(PROGRAM_NAME)

program: $(OBJ_FILES)
	g++ -o $(PROGRAM_NAME) $(OBJ_FILES)

obj/%.o: %.cpp
	g++ $(CC_FLAGS) -o $@ $<