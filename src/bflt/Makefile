grammar = BFL.g4
binname = bfl

ANTLR4_HEADERS=/home/shakalyan/programs/antlr4/run/usr/local/include/antlr4-runtime/
ANTLR4_SO_PATH=/home/shakalyan/programs/antlr4/run/usr/local/lib
ANTLR4_SO=antlr4-runtime
ANTLR4_GDIR=generate/

SOURCES = main.cpp ../bf.cpp

antrl4-generate: $(grammar)
	antlr4 -no-listener -visitor -Dlanguage=Cpp -o $(ANTLR4_GDIR) $(grammar)

compile:
	g++ -I $(ANTLR4_HEADERS) -I $(ANTLR4_GDIR) $(SOURCES) $(ANTLR4_GDIR)/*.cpp -L $(ANTLR4_SO_PATH) -l$(ANTLR4_SO) -o $(binname)

run:
	./$(binname)

clean:
	-rm $(binname)
	-rm -r $(ANTLR4_GDIR)