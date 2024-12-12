TARGET = bin/final
# List all files matching the pattern
SRC = $(wildcard src/*.c)
# Substring all the files provided(3) and convert using pattern provided (1) 
# into (2)
OBJ = $(patsubst src/%.c,obj/%.o, $(SRC))

default: clean $(TARGET)

$(TARGET): $(OBJ)
	$(info Linking objects: [$(@)] [$(?)])
	gcc -Wall -o $@ $? -Iinclude

obj/%.o: src/%.c
	$(info Compiling: [$(<)] -> [$(@)])
	gcc -Wall -c $< -o $@ -Iinclude

trace: 
	ktrace trace -S -c ./$(TARGET)

clean:
	rm -rf obj/*
	rm -rf bin/*
	rm *.o
