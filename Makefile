BIN_NAME=bfcad
BUILD_PATH=./build
BIN_PATH=$(BUILD_PATH)/$(BIN_NAME)

BFLT_PATH=src/bflt
BFLT_SOURCES=bfl_translator.cpp bflt_visitor.cpp

ALGC_PATH=src/alg_core
ALGC_SOURCES=bf.cpp

ANTLR4_GENERTD=$(BFLT_PATH)/generated
ANTLR4_GRAMMAR=$(BFLT_PATH)/BFL.g4
ANTLR4_HEADERS=lib/antlr4-4.13.2/include/antlr4-runtime/
ANTLR4_SO_PATH=lib/antlr4-4.13.2/
ANTLR4_SO=antlr4-runtime

SOURCES=src/main.cpp $(addprefix $(ALGC_PATH)/, $(ALGC_SOURCES)) $(addprefix $(BFLT_PATH)/, $(BFLT_SOURCES)) $(wildcard $(ANTLR4_GENERTD)/*.cpp)
OBJECTS=$(SOURCES:cpp=o)
HEADERS=-I $(ANTLR4_HEADERS) -I $(BFLT_PATH) -I $(ALGC_PATH)
LIBS=-L $(ANTLR4_SO_PATH) -l$(ANTLR4_SO)
CC=g++

%.o: %.cpp
	@$(CC) -c -o $@ $^ $(LIBS) $(HEADERS)
	@echo $@

$(BIN_PATH): $(OBJECTS)
	@-mkdir $(BUILD_PATH)
	$(CC) -o $@ $^ $(LIBS) $(HEADERS)

run: $(BIN_PATH)
	$(BIN_PATH)

antlr4-generate:
	antlr4 -no-listener -visitor -Dlanguage=Cpp -o $(ANTLR4_GENERTD) -Xexact-output-dir $(ANTLR4_GRAMMAR)

clean:
	@-rm -r $(BUILD_PATH)
	@-rm $(OBJECTS)

tst:
	@echo $(basename $(ANTLR4_SO:lib%=%))
