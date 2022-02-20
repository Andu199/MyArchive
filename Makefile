build: main.cpp Huffman.h Huffman.cpp MyTree.h Parser.h Parser.cpp
	g++ -g main.cpp Huffman.h Huffman.cpp MyTree.h Parser.h Parser.cpp -o program
clean:
	rm -rf Parser.h.gch MyTree.h.gch Huffman.h.gch program