
TARGET  = libbtds4_sample_app.elf

CC := gcc
CFLAGS := -g -MMD -MP -Wall -Wextra
LDFLAGS = -lz ../libbtds4.so
SRC_DIR = .
OBJ_DIR = .
INCLUDE = -I../include

SOURCES = $(shell ls $(SRC_DIR)/*.c)
OBJS    = $(subst $(SRC_DIR),$(OBJ_DIR), $(SOURCES:.c=.o))
DEPENDS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	$(RM) $(OBJS) $(TARGET) $(DEPENDS)

-include $(DEPENDS)

.PHONY: all clean
