SRC_FILES := $(wildcard Core/*.cpp) $(wildcard Puzzle3/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(SRC_FILES:.cpp=.o)))

CC_FLAGS := -c -std=c++11

PROGRAM_NAME := ga

all: ga
	echo "Compilation Complete!"

clean:
	rm -f $(OBJ_FILES) $(PROGRAM_NAME)

ga: $(OBJ_FILES)
	g++ -o $(PROGRAM_NAME) $(OBJ_FILES) -lboost_system -lboost_thread -lpthread

obj/%.o: %.cpp
	g++ $(CC_FLAGS) -o $@ $< -lboost_system -lboost_thread -lpthread

obj/%.o: Core/%.cpp
	g++ $(CC_FLAGS) -o $@ $< -lboost_system -lboost_thread -lpthread

obj/%.o: Puzzle3/%.cpp
	g++ $(CC_FLAGS) -o $@ $< -lboost_system -lboost_thread -lpthread
