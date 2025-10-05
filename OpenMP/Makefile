CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -MMD -MP -g

SRC_DIR = src
BUILD_DIR = build

# executables
STANDARD_TARGET = sim_anneal
PARALLEL_TARGET = sim_anneal_parallel
SEED_PARALLEL_TARGET = sim_anneal_seed_parallel

# source files
STANDARD_SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/state.cpp $(SRC_DIR)/sim_anneal.cpp
PARALLEL_SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/state_parallel.cpp $(SRC_DIR)/sim_anneal.cpp
SEED_PARALLEL_SRCS = $(SRC_DIR)/main_seed_parallel.cpp $(SRC_DIR)/state.cpp $(SRC_DIR)/sim_anneal.cpp

# object files in separate dirs
STD_BUILD = $(BUILD_DIR)/standard
PAR_BUILD = $(BUILD_DIR)/parallel
SEED_PAR_BUILD = $(BUILD_DIR)/seed_parallel

STANDARD_OBJS = $(STANDARD_SRCS:$(SRC_DIR)/%.cpp=$(STD_BUILD)/%.o)
PARALLEL_OBJS = $(PARALLEL_SRCS:$(SRC_DIR)/%.cpp=$(PAR_BUILD)/%.o)
SEED_PARALLEL_OBJS = $(SEED_PARALLEL_SRCS:$(SRC_DIR)/%.cpp=$(SEED_PAR_BUILD)/%.o)

# all targets
all: $(STANDARD_TARGET) $(PARALLEL_TARGET) $(SEED_PARALLEL_TARGET)

# link rules
$(STANDARD_TARGET): $(STANDARD_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(PARALLEL_TARGET): $(PARALLEL_OBJS)
	$(CXX) $(CXXFLAGS) -fopenmp $^ -o $@

$(SEED_PARALLEL_TARGET): $(SEED_PARALLEL_OBJS)
	$(CXX) $(CXXFLAGS) -fopenmp $^ -o $@

# compile rules (standard)
$(STD_BUILD)/%.o: $(SRC_DIR)/%.cpp | $(STD_BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# compile rules (parallel, add -fopenmp)
$(PAR_BUILD)/%.o: $(SRC_DIR)/%.cpp | $(PAR_BUILD)
	$(CXX) $(CXXFLAGS) -fopenmp -c $< -o $@

# compile rules (seed parallel, identical to parallel)
$(SEED_PAR_BUILD)/%.o: $(SRC_DIR)/%.cpp | $(SEED_PAR_BUILD)
	$(CXX) $(CXXFLAGS) -fopenmp -c $< -o $@

# ensure build dirs exist
$(STD_BUILD) $(PAR_BUILD) $(SEED_PAR_BUILD):
	mkdir -p $@

generate: 
	python generate_case.py 100000 0.998 921 100 out.txt

run: 
	./$(STANDARD_TARGET) out.txt

run_p: 
	./$(PARALLEL_TARGET) out.txt

run_seed_p:
	./$(SEED_PARALLEL_TARGET) out.txt

clean:
	rm -f $(STANDARD_TARGET) $(PARALLEL_TARGET) $(SEED_PARALLEL_TARGET) out.txt
	rm -rf $(BUILD_DIR)
	
# export OMP_NUM_THREADS=