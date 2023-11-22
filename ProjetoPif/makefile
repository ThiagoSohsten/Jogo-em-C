# Define o compilador
CC=gcc

# Define as flags de compilação
CFLAGS=-Wall -g

# Define o nome do executável
TARGET=ex

# Define o diretório onde os arquivos fonte estão localizados
SRC_DIR=cobra

all: $(TARGET)

$(TARGET):
    $(CC) $(CFLAGS) -o $(TARGET) $(SRC_DIR)/main.c $(SRC_DIR)/snake.c

clean:
    rm -f $(TARGET) $(SRC_DIR)/*.o
