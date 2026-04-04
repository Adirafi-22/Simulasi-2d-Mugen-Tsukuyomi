CC     = gcc
CFLAGS = -Wall -O2 -I. -Isrc -Iui -Ialgo $(shell pkg-config --cflags raylib)
LIBS   = $(shell pkg-config --libs raylib) -lm -ldl -lpthread

TARGET = mugen_tsukuyomi

# Semua file C
SRCS = main.c ui/menu.c src/animasi.c src/pola.c algo/algoritma.c ui/about.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

run: all
	LIBGL_ALWAYS_SOFTWARE=1 ./$(TARGET)