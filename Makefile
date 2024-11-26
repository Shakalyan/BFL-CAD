BIN_NAME=bfcad
BUILD_PATH=./build
BIN_PATH=$(BUILD_PATH)/$(BIN_NAME)

BFLT_PATH=src/bflt
BFLT_SOURCES=bfl_translator.cpp bflt_visitor.cpp bflt_exception.cpp

ALGC_PATH=src/alg_core
ALGC_SOURCES=bf.cpp quine_opt.cpp

LGGR_PATH=src/logger
LGGR_SOURCES=logger.cpp

CMMN_PATH=src/common
CMMN_SOURCES=


ANTLR4_GENERTD=$(BFLT_PATH)/generated
ANTLR4_GRAMMAR=$(BFLT_PATH)/BFL.g4
ANTLR4_HEADERS=lib/antlr4-4.13.2/include/antlr4-runtime/
ANTLR4_SO_PATH=lib/antlr4-4.13.2/
ANTLR4_SO=antlr4-runtime

# QT6_DEFINES= -DQT_NO_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB
# QT6_INCPATH= -I/usr/include/x86_64-linux-gnu/qt6 -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets -I/usr/include/x86_64-linux-gnu/qt6/QtGui -I/usr/include/x86_64-linux-gnu/qt6/QtCore -I. -I/usr/lib/x86_64-linux-gnu/qt6/mkspecs/linux-g++
# QT6_CXXFLGS= -pipe -O2 -Wall -Wextra -D_REENTRANT -fPIC $(QT6_DEFINES)
# QT6_LIBS   = /usr/lib/x86_64-linux-gnu/libQt6Gui.so /usr/lib/x86_64-linux-gnu/libGLX.so /usr/lib/x86_64-linux-gnu/libOpenGL.so /usr/lib/x86_64-linux-gnu/libQt6Core.so -lpthread -lGLX -lOpenGL   
# QT6_LDFLGS = -Wl,-O1 -Wl,-rpath-link,/usr/lib/x86_64-linux-gnu
QT6_CONF_PRFX=qt-conf
include $(QT6_CONF_PRFX).mk

SOURCES=src/main.cpp $(addprefix $(ALGC_PATH)/, $(ALGC_SOURCES)) $(addprefix $(BFLT_PATH)/, $(BFLT_SOURCES)) \
		$(addprefix $(LGGR_PATH)/, $(LGGR_SOURCES)) $(addprefix $(CMMN_PATH)/, $(CMMN_SOURCES)) $(wildcard $(ANTLR4_GENERTD)/*.cpp)
OBJECTS=$(SOURCES:cpp=o)
HEADERS=-I $(ANTLR4_HEADERS) -I $(BFLT_PATH) -I $(ALGC_PATH) -I $(LGGR_PATH) -I $(CMMN_PATH) $(QT6_INCPATH)
LIBS=-L $(ANTLR4_SO_PATH) -l$(ANTLR4_SO) $(QT6_LIBS)
CXX=g++
CXX_FLGS=$(QT6_CXXFLAGS)
LD_FLGS=$(QT6_LFLAGS)

%.o: %.cpp
	@$(CXX) $(CXX_FLGS) -c -o $@ $^ $(HEADERS)
	@echo $@

$(BIN_PATH): $(OBJECTS)
	@-mkdir $(BUILD_PATH)
	$(CXX) $(LD_FLGS) -o $@ $^ $(LIBS)

run: $(BIN_PATH)
	$(BIN_PATH)

antlr4-generate:
	antlr4 -no-listener -visitor -Dlanguage=Cpp -o $(ANTLR4_GENERTD) -Xexact-output-dir $(ANTLR4_GRAMMAR)

$(QT6_CONF_PRFX).mk: $(QT6_CONF_PRFX).pro
	qmake6 -o $@
	./scripts/process_qtmk.sh $@


clean:
	@-rm $(QT6_CONF_PRFX).mk
	@-rm -r $(BUILD_PATH)
	@-rm $(OBJECTS)

tst:
	@echo $(basename $(ANTLR4_SO:lib%=%))
