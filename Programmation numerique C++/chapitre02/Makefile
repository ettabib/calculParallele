  
PROG=ClassABC

CXXFLAGS= -g

obj= CheckPtr.o $(PROG).o

all: $(obj) 
	g++   $(obj) -o $(PROG).exe

.cpp.o:
	g++ -c $(CXXFLAGS) $< 

clean:
	rm $(PROG).exe $(obj)