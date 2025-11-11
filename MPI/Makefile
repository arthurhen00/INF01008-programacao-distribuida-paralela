CC = mpicc
CFLAGS = -O2 -Wall

SRC_DIR = src
BUILD_DIR = build

TARGETS = mpi_coletiva mpi_p2p_bloqueante mpi_p2p_naobloqueante

SRCS = $(SRC_DIR)/mpi_coletiva.c \
       $(SRC_DIR)/mpi_p2p_bloqueante.c \
       $(SRC_DIR)/mpi_p2p_naobloqueante.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

all: $(TARGETS)

mpi_coletiva: $(BUILD_DIR)/mpi_coletiva.o | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

mpi_p2p_bloqueante: $(BUILD_DIR)/mpi_p2p_bloqueante.o | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

mpi_p2p_naobloqueante: $(BUILD_DIR)/mpi_p2p_naobloqueante.o | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f $(TARGETS)
	rm -rf $(BUILD_DIR)
