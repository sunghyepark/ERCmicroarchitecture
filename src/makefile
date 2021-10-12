
EXE = QC2021_Base
CXX = g++
SYSTEM = x86-64_linux
LIBFORMAT = static_pic
OBJECT = ../build

.SUFFIXES : .c .o

CXXOPTS = -m64 -O3 -fopenmp -fPIC -fno-strict-aliasing -fexceptions -fpermissive -DNDEBUG -DIL_STD -Wno-ctor-dtor-privacy
CFLAGS = -std=c++11 -static-libstdc++ -O3 -fopenmp -fpermissive -DNDEBUG -DIL_STD -pg -Wno-ctor-dtor-privacy $(LIBINCLUDE)

# include library settings
LIBINCLUDE = -I$(SRCHASH)/include -I$(BOOST)/include

# linking flag settings
LINKCFLAGS = $(CFLAGS) -static-libstdc++ -lX11 -ldl -lm -lpthread#$(LIBGOMP)

# source settings
BOOST   	= /tool/cpplib/boost/install/1.67.0
SRCHASH 	= /tool/cpplib/sparsehash/install
LIBGOMP 	= -L/tool/gcc/install/5.4.0/lib64/libgomp.a -static


SRC = 	$(OBJECT)/main.o \
		$(OBJECT)/QASMtoken.o \
		$(OBJECT)/QASMscanner.o \
		$(OBJECT)/QASMparser.o \
		$(OBJECT)/graph.o \
		$(OBJECT)/graphFunction.o \
		$(OBJECT)/parser.o \
		$(OBJECT)/print.o \
		$(OBJECT)/couplinggraph.o \
		$(OBJECT)/initial_mapping.o \
		$(OBJECT)/mappingFunction.o \
		$(OBJECT)/mappingPreparation.o \
		$(OBJECT)/mapping.o \
		$(OBJECT)/outputwriter.o \
		$(OBJECT)/mymeasure.o 

$(EXE): $(SRC)
	@echo " "
	@echo "Linking..."
	@echo " "
	$(CXX) $(CXXOPTS) $^ -o $@ $(LINKCFLAGS)
	@echo " "
	@echo "Linking is Completed !!"
	@echo " "
	@echo "Tagging..."
	@echo " "
	ctags -R
	@echo " "
	@echo "Finished..!"
	@echo " "
	@echo "GNU GCC/G++ compiler has been used as"
	@g++ --version


$(OBJECT)/main.o: main.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJECT)/QASMtoken.o: QASMtoken.cpp QASMtoken.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJECT)/QASMscanner.o: QASMscanner.cpp QASMscanner.hpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJECT)/QASMparser.o: QASMparser.cpp QASMparser.h circuit.h
	$(CXX) $(CFLAGS) -c -o $@ $<
	
$(OBJECT)/graph.o: graph.cpp circuit.h
	$(CXX) $(CFLAGS) -c -o $@ $<
	
$(OBJECT)/graphFunction.o: graphFunction.cpp circuit.h
	$(CXX) $(CFLAGS) -c -o $@ $<
	
$(OBJECT)/parser.o: parser.cpp circuit.h
	$(CXX) $(CFLAGS) -c -o $@ $<
	
$(OBJECT)/print.o: print.cpp circuit.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJECT)/couplinggraph.o: couplinggraph.cpp circuit.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJECT)/initial_mapping.o: initial_mapping.cpp circuit.h 
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJECT)/mappingFunction.o: mappingFunction.cpp circuit.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJECT)/mappingPreparation.o: mappingPreparation.cpp circuit.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJECT)/mapping.o: mapping.cpp circuit.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJECT)/outputwriter.o: outputwriter.cpp circuit.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(OBJECT)/mymeasure.o: mymeasure.cpp circuit.h
	$(CXX) $(CFLAGS) -c -o $@ $<

.PHONY: clean cleanlog
clean: 
	/bin/rm -f ../build/*.o 2> /dev/null;  /bin/rm -f $(EXE) 2> /dev/null;
cleanlog:
	/bin/rm -f ../log/*.log 2> /dev/null;


