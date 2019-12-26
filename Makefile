all: easia



easia: Tokenizer.o parser.o
	g++ Tokenizer.o parser.o Cal.cpp -o easia

Tokenizer.o: Tokenizer.cpp Tokenizer.h
	g++  Tokenizer.cpp -c

parser.o: parser.cpp parser.h
	g++  parser.cpp -c

clean:
	$(RM) *.o easia