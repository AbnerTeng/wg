CXX = g++
# CXXFLAGS = -std=c++17 -O2 -mmacosx-version-min=14.1
CXXFLAGS = -std=c++17 -O2
LDFLAGS = -lyaml-cpp
TARGET = wg

all:
	$(CXX) $(CXXFLAGS) -o $(TARGET) src/wg.cpp src/gpu_utils.cpp src/cli_handler.cpp $(LDFLAGS)

install: all
	install -m 755 $(TARGET) /usr/local/bin/$(TARGET)

clean:
	rm -f $(TARGET)
