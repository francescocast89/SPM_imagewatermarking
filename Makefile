CXX      = g++
CXXFLAGS = -O3 -std=c++11 -I lib/ -D cimg_display=0 #-g -O0
LDFLAGS  = -pthread
OBJS     = imagewatermarking

TARGETS	 = imagewatermarking
.PHONY: all clean cleanall
.SUFFIXES: .cpp 


%: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $< $(LDFLAGS)

all		: $(TARGETS)
clean		: 
	rm -f $(TARGETS)
cleanall	: clean
	\rm -f *.o *~
