CXXFLAGS= `wx-config --cflags` -I./../../include/wXChip
LDFLAGS= `wx-config --libs`
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix ,$(notdir $(CPP_FILES:.cpp=.o)))

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

alL: $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o wXChip  $(LDFLAGS)

clean:
	rm -f wXChip *.o
