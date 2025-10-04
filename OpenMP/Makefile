CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

SRC_DIR = src
BUILD_DIR = build
STD_BUILD = $(BUILD_DIR)/standard
PAR_BUILD = $(BUILD_DIR)/parallel

STANDARD_SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/state.cpp $(SRC_DIR)/sim_anneal.cpp
PARALLEL_SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/state_parallel.cpp $(SRC_DIR)/sim_anneal.cpp

STANDARD_OBJS = $(STANDARD_SRCS:$(SRC_DIR)/%.cpp=$(STD_BUILD)/%.o)
PARALLEL_OBJS = $(PARALLEL_SRCS:$(SRC_DIR)/%.cpp=$(PAR_BUILD)/%.o)

STANDARD_TARGET = sim_anneal
PARALLEL_TARGET = sim_anneal_parallel

all: $(STANDARD_TARGET) $(PARALLEL_TARGET)

$(STANDARD_TARGET): $(STANDARD_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(PARALLEL_TARGET): $(PARALLEL_OBJS)
	$(CXX) $(CXXFLAGS_PAR) $^ -o $@ -fopenmp

$(STD_BUILD)/%.o: $(SRC_DIR)/%.cpp | $(STD_BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PAR_BUILD)/%.o: $(SRC_DIR)/%.cpp | $(PAR_BUILD)
	$(CXX) $(CXXFLAGS_PAR) -c $< -o $@

$(STD_BUILD) $(PAR_BUILD):
	mkdir -p $@

generate: 
	python generate_case.py 100000 0.998 921 100000 out.txt

run: 
	./$(STANDARD_TARGET) out.txt

run_p: 
	./$(PARALLEL_TARGET) out.txt

clean:
	rm -f $(STANDARD_TARGET) $(PARALLEL_TARGET) out.txt
	rm -rf $(BUILD_DIR)
	
# export OMP_NUM_THREADS=
