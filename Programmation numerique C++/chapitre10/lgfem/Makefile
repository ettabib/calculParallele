CXX=g++
CXXOPTS=-g
OBJ=expr.o fem.tab.o sfemmat.o
lgfem: $(OBJ)
	$(CXX) $(CXXOPT)  -o  $@  $(OBJ)
fem.tab.o:fem.tab.c
	$(CXX) $(CXXOPTS) -c $<
.c.o:
	$(CXX) $(CXXOPTS) -c $< 
.cpp.o:
	$(CXX) $(CXXOPTS) -c $< 
fem.tab.c:fem.y
	bison -v $<
clean:
	-rm *~ *.o  fem.output fem.tab.c 
	-rm lgfem $(OBJ)