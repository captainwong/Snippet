
INC := $(wildcard *.h)
SRC := $(wildcard *.c)

CC := gcc
CXX := g++
OBJS := $(patsubst %cpp, %o, $(SRC))
CFLAGS := -g -Wall
CXXFLAGS :=
LDFLAGS :=
TARGET := calendar

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c $(INC)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f $(TARGET) $(OBJS)


