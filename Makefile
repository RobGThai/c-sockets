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

compile_select:
	gcc -Wall -o obj/sel.o src/select_demo.c -Iinclude

compile_poll:
	gcc -Wall -o obj/poll.o src/poll_demo.c -Iinclude

compile_all: compile_server compile_client compile_select compile_poll

trace: 
	ktrace trace -S -c ./$(TARGET)

clean:
	rm -rf obj/*
	rm -rf bin/*
	rm *.o
