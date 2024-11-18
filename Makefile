BIN_NAME=bfcad
BUILD_PATH=./build
BIN_PATH=$(BUILD_PATH)/$(BIN_NAME)

SOURCES=src/main.cpp src/bf.cpp

OBJECTS=$(SOURCES:cpp=o)
CC=g++

%.o: %.cpp
	$(CC) -c -o $@ $^

$(BIN_PATH): $(OBJECTS)
	@-mkdir $(BUILD_PATH)
	$(CC) -o $@ $^

run: $(BIN_PATH)
	$(BIN_PATH)

clean:
	@-rm -r $(BUILD_PATH)
	@-rm $(OBJECTS)

tst:
	echo $(OBJECTS)
