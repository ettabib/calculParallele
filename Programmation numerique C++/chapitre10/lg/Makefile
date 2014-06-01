all: exp
exp.cpp:exp.y
	bison -dtv -p exp  $< -o exp.cpp
	  -mv exp.cpp.h  exp.hpp # pour un  pb entre des versions de bison 
test: exp
	./exp prog.lg
clean:
	-rm *.o exp exp.cpp.h exp.cpp *.output *~