WX_PATH=/usr/local
CXXFLAGS= -D_DEBUG -std=c++11 `$(WX_PATH)/bin/wx-config --cflags`  -I/usr/local/include -I../../include
LDFLAGS= `$(WX_PATH)/bin/wx-config --libs`  templibs/libCore.a templibs/libSDLMedia.a templibs/libUtility.a `sdl2-config --static-libs`

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix ,$(notdir $(CPP_FILES:.cpp=.o)))

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

alL: $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o wXChip  $(LDFLAGS)

clean:
	rm -f wXChip *.o
emulator:
	$(CXX) -std=c++11 -I../../include emu/emu.cpp -o XChipEmulator `sdl2-config --cflags --static-libs` templibs/libCore.a templibs/libSDLMedia.a templibs/libUtility.a
