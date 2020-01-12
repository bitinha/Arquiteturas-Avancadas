SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build

CFLAGS = -Wall -L/share/apps/papi/5.4.1/lib -I/share/apps/papi/5.4.1/include -O2 -lpapi

CC_GPU = nvcc
CC_FLAGS = -O3

CC_intel = icpc
CFLAGS_intel = -qopenmp -O3 -std=c++11 -qop-report-phase=vec -qopt-report=2 -march=native

all: directories cuda matMult

cuda: directories $(BUILD_DIR)/matMultGPU.o
	$(CC_GPU) $(CC_FLAGS) -o $(BIN_DIR)/matMultGPU $(BUILD_DIR)/matMultGPU.o

$(BUILD_DIR)/matMultGPU.o: $(SRC_DIR)/matMultGPU.cu
	$(CC_GPU) -c $(CC_FLAGS) $(INCLUDES) $< -o $@ $(LIBS)

matMult: directories $(SRC_DIR)/matMult.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/matMult $(SRC_DIR)/matMult.c

manyCore: directories $(SRC_DIR)/matMult.c
	$(CC_intel) $(CFLAGS_intel) -o $(BIN_DIR)/matMult_xeon $(SRC_DIR)/matMult.c

directories: $(BUILD_DIR) $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(BUILD_DIR)/* $(BIN_DIR)/*
