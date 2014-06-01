CXX=g++ 

all: mesh


mesh: Mesh.o main.o
	$(CXX) $^ -o $@ $(GLLIBS)

.cpp.o:
	$(CXX) -c  $< $(GLINS)

clean:
	-rm mesh *.o *~
#  les dependances 

mesh.o: R2.hpp Mesh.hpp
main.o: Mesh.hpp
