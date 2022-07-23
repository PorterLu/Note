.PHONY:all clean

all: app

app: main.o scope.o
	gcc -Wall $^ -o $@

clean:
	rm app main.o scope.o -rf
