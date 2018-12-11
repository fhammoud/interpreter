CC		:= gcc
C_FLAGS := -g -Wall -Wextra -std=c11


LIBRARIES	:=

EXECUTABLE	:= main.out

all: $(BIN)/$(EXECUTABLE)

clean:
	-$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): *.cpp
	$(CC) $(C_FLAGS) -I*.h -L$(LIB) $^ -o $@ $(LIBRARIES)