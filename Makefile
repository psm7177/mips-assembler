CC = gcc

INCLUDE = -Iinclude/

SRC_DIR = ./src
OBJ_DIR = ./obj
DIRS=$(OBJ_DIR)

$(shell mkdir -p $(DIRS))

TARGET = asmcompile

SRCS = $(notdir $(wildcard $(SRC_DIR)/*.c))

OBJS = $(SRCS:.c=.o)

OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o,$(OBJS))
DEPS = $(OBJECTS:.o=.d)

all: $(TARGET)

$(TARGET) : $(OBJECTS)
	echo $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(INCLUDE) -c $< -o $@ -MD

.PHONY: clean all
clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)

-include $(DEPS)