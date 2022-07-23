target:=app
source:=$(wildcard *.c)
objects:=$(source:.c=.o)
CFLAGS:= -g -Wall

all: $(target)

$(target): $(objects)
	gcc $(CFLAGS) $^ -o $@

.PHONY: all clean print

clean:
	rm $(objects) $(target) -rf

print:
	@echo source=$(source)
	@echo objects=$(objects)
