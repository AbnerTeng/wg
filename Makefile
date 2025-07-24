CXX = g++
CXXFLAGS = -std=c++17 -O2
TARGET = wg

all:
	$(CXX) $(CXXFLAGS) -o $(TARGET) src/wg.cpp

install: all
	install -m 755 $(TARGET) /usr/local/bin/$(TARGET)

clean:
	rm -f $(TARGET)
