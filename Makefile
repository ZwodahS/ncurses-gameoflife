BIN=life
NCURSES=-l ncurses
OBJDIR=obj

CXX=clang++ ${WARNING} -std=c++11

all : $(BIN)

$(BIN) : main.cpp
	$(CXX) -o $(BIN) main.cpp $(NCURSES)

clean:
	rm obj/*.o
	rm $(BIN)

remake:
	make clean
	make all

debug:
	@echo $(zf_sfml)

.PHONY: clean remake debug
