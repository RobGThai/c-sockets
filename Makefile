TARGET = bin/final
# List all files matching the pattern
SRC = $(wildcard src/*.c)
# Substring all the files provided(3) and convert using pattern provided (1) 
# into (2)
OBJ = $(patsubst src/%.c,obj/%.o, $(SRC))

default: clean $(TARGET)

new: $(TARGET)

$(TARGET): $(OBJ)
	$(info Linking objects: [$(@)] [$(?)])
	gcc -Wall -o $@ $? -Iinclude

obj/%.o: src/%.c
	$(info Compiling: [$(<)] -> [$(@)])
	gcc -Wall -c $< -o $@ -Iinclude

compile_server:
	gcc -Wall -o obj/server.o src/server.c -Iinclude

compile_client:
	gcc -Wall -o obj/client.o src/client.c -Iinclude

trace: 
	ktrace trace -S -c ./$(TARGET)

clean:
	rm -rf obj/*
	rm -rf bin/*
	rm *.o
