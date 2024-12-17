#Compiler
CC=gcc
CFLAGS = -Wall -Wextra -g -I./src/include
#D'C:\Users\HP\Desktop\CENG209 Systems Programming\Homework\Descriptions=“$(CURDIR)"'

# Linker Flags
LDFLAGS = 

# Target
TARGET = main.exe

# Source Files
SRCS = Item.c Creature.c Player.c Room.c main.c

# Build Rules
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	del $(TARGET)