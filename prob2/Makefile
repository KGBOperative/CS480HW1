RM=rm -f
CXX=g++
LD=ld
CFLAGS=-c -Wall -std=c++11 -MMD
LDFLAGS=

MAIN=hw1_2main.cpp
SOURCES=hw1_2.cpp $(MAIN) 
OBJECTS=$(SOURCES:.cpp=.o)
DEPS=$(OBJECTS:.o=.d)
EXECUTABLE=a.star

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

-include $(DEPS)

.cpp.o:
	$(CXX) $(CFLAGS) -MF $(patsubst %.o,%.d,$@) $< -o $@

clean: 
	$(RM) $(OBJECTS) $(DEPS) $(EXECUTABLE)
