EXE = Simulation.exe

INCLUDE 	= -I ./Include/glfw/include/ -I ./Include/ 
ARG			= -L ./lib/ -lglfw3 -lopengl32 -lgdi32 -lmingw32 -luser32 -g

SRC			= $(wildcard src/*.cpp)
OBJ 		= $(subst src, build, $(patsubst %.cpp, %.o, $(SRC)))

all: link
remake: clean all

run:
	$(EXE)

clean:
	erase $(subst build/, build\, $(OBJ))

mkbuild:
	mkdir build

build/%.o: src/%.cpp
	g++ $(INCLUDE) -c src/$*.cpp -o build/$*.o $(ARG) $(DBG)


link: $(OBJ)
	g++ $(OBJ) -o $(EXE) $(DBG) $(ARG)

