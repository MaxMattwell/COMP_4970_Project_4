all: main.o ga_main.o ./src/datapoint.o ./src/webcrawler.o ./src/grnn.o
	g++ -std=c++11 main.o ./src/datapoint.o ./src/webcrawler.o ./src/grnn.o -o myMDA5 -g


ga: ga_main.o ./src/datapoint.o ./src/grnn.o ./src/featga.o
	g++ -std=c++11 ga_main.o ./src/datapoint.o ./src/grnn.o ./src/featga.o -o GA -g

ga_main.o: ga_main.cpp ./src/featga.h
	g++ -std=c++11 ga_main.cpp -c -g

main.o: main.cpp ./src/datapoint.h
	g++ -std=c++11 main.cpp -c -g
	
featga.o: ./src/featga.cpp ./src/featga.h
	g++ -st=c++11 ./src/featga.cpp -c -g

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