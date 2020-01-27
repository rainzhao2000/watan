CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD -g -Werror=vla
EXEC = watan
OBJECTS = board.o  criterion.o	dice.o  fairdice.o  goal.o  loaddice.o  main.o  state.o  student.o textdisplay.o  textinput.o  tile.o  watan.o wataninput.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
