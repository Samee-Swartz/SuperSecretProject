SRC_FILES := $(wildcard ThinkerLib/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(SRC_FILES:.cpp=.o)))

CC_FLAGS := -c -std=c++11

PROGRAM_NAME := pacman

all: pacman
	echo "Compilation Complete!"

clean:
	rm -f $(OBJ_FILES) $(PROGRAM_NAME)

pacman: $(OBJ_FILES)
	g++ -o $(PROGRAM_NAME) $(OBJ_FILES)

obj/%.o: ThinkerLib/%.cpp
	g++ $(CC_FLAGS) -o $@ $<
