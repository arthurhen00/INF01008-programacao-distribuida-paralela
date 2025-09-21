CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

SRC_DIR = src

SRCS = $(SRC_DIR)/main.cpp
HEADERS = $(SRC_DIR)/state.h $(SRC_DIR)/sim_anneal.h
TARGET = sim_anneal

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
