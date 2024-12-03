# SOURCES
SOURCES += src/main.cpp

BFLT_PATH=src/bflt
BFLT_SOURCES=bfl_translator bflt_visitor bflt_exception
for (src, BFLT_SOURCES): SOURCES += $$BFLT_PATH/$${src}.cpp
for (src, BFLT_SOURCES): HEADERS += $$BFLT_PATH/$${src}.h

ANTLR4_GENERTD=$$BFLT_PATH/generated
SOURCES+=$$ANTLR4_GENERTD/*.cpp
HEADERS+=$$ANTLR4_GENERTD/*.h

ALGC_PATH=src/alg_core
ALGC_SOURCES=bf quine_opt
for (src, ALGC_SOURCES): SOURCES += $$ALGC_PATH/$${src}.cpp
for (src, ALGC_SOURCES): HEADERS += $$ALGC_PATH/$${src}.h

LGGR_PATH=src/logger
LGGR_SOURCES=logger
for (src, LGGR_SOURCES): SOURCES += $$LGGR_PATH/$${src}.cpp
for (src, LGGR_SOURCES): HEADERS += $$LGGR_PATH/$${src}.h

CMMN_PATH=src/common
CMMN_SOURCES=
for (src, CMMN_SOURCES): SOURCES += $$CMMN_PATH/$${src}.cpp
for (src, CMMN_SOURCES): HEADERS += $$CMMN_PATH/$${src}.h
HEADERS += $$CMMN_PATH/formatter.h

GUI_PATH=src/gui
GUI_SOURCES=bfc_gui bfc_frame_container frames/editor frames/output_panel frames/side_panel
for (src, GUI_SOURCES): SOURCES += $$GUI_PATH/$${src}.cpp
for (src, GUI_SOURCES): HEADERS += $$GUI_PATH/$${src}.h

# LIBS
ANTLR4_HEADERS=lib/antlr4-4.13.2/include/antlr4-runtime/
ANTLR4_SO_PATH=lib/antlr4-4.13.2/
ANTLR4_SO=antlr4-runtime
ANTLR4_GRAMMAR=$$BFLT_PATH/BFL.g4

INCLUDEPATH += $${BFLT_PATH} $${ALGC_PATH} $${LGGR_PATH} $${CMMN_PATH} $${GUI_PATH} $${ANTLR4_HEADERS}
LIBS += -L$${ANTLR4_SO_PATH} -l$${ANTLR4_SO}

# QT
TEMPLATE = app
TARGET = bflcad
QT += gui widgets

# BUILD
DESTDIR = build/
OBJECTS_DIR = $${DESTDIR}/.objects
MOC_DIR = $${DESTDIR}/.moc
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $${DESTDIR}/.ui

# MAKEFILE CONFIGURATION
antlr4GenerateTarget.target = antlr4-generate
antlr4GenerateTarget.depends = $$ANTLR4_GRAMMAR
antlr4GenerateTarget.commands = antlr4 -no-listener -visitor -Dlanguage=Cpp -o $$ANTLR4_GENERTD -Xexact-output-dir $$ANTLR4_GRAMMAR
QMAKE_EXTRA_TARGETS += antlr4GenerateTarget
