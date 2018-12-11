CC		:= gcc
C_FLAGS := -g -Wall -Wextra -std=c11


LIBRARIES	:=

EXECUTABLE	:= main.out

all: $(EXECUTABLE)

clean:
	-$(RM) $(EXECUTABLE)

run: all
	./$(EXECUTABLE)

$(EXECUTABLE): *.c
	$(CC) $(C_FLAGS) -I*.h $^ -o $@ $(LIBRARIES)