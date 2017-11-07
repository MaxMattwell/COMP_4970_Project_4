all: main.o ./src/datapoint.o ./src/webcrawler.o ./src/grnn.o
	g++ -std=c++11 main.o ./src/datapoint.o ./src/webcrawler.o ./src/grnn.o -o myMDA5 -g

main.o: main.cpp ./src/datapoint.h
	g++ -std=c++11 main.cpp -c -g

datapoint.o: ./src/datapoint.cpp ./src/datapoint.h
	g++ -std=c++11 ./src/datapoint.cpp -c -g

webcrawler.o: ./src/webcrawler.cpp ./src/webcrawler.h
	g++ -std=c++11 ./src/webcrawler.cpp -c -g

grnn.o: ./src/grnn.cpp ./src/grnn.h
	g++ -std=c++11 ./src/grnn.cpp -c -g

.INTERMEDIATE: main.o

clean:
	rm -f *.o
	rm -f myMDA5
	rm -f *.txt
	rm -f ./src/*.o
	rm -f ./bin/*.*
	rm -f ./bin/html_source/*.*
	rm -f ./bin/unigrams/*.*