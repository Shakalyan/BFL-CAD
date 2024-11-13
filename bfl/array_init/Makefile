grammar = ArrayInit
binname = bfl-translate

ANTLR4_HEADERS=/home/shakalyan/programs/antlr4/run/usr/local/include/antlr4-runtime/
ANTLR4_SO_PATH=/home/shakalyan/programs/antlr4/run/usr/local/lib
ANTLR4_SO=antlr4-runtime
ANTLR4_GDIR=generate/

antrl4-generate:
	antlr4 -Dlanguage=Cpp -o $(ANTLR4_GDIR) $(grammar).g4

compile:
	g++ -I $(ANTLR4_HEADERS) -I $(ANTLR4_GDIR) main.cpp $(ANTLR4_GDIR)/*.cpp -L $(ANTLR4_SO_PATH) -l$(ANTLR4_SO) -o $(binname)

run:
	./$(binname)

clean:
	-rm $(binname)
	-rm -r $(ANTLR4_GDIR)