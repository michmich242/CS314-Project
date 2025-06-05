# Compiler and Platform Detection
COMP = g++

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    PLATFORM := macOS
else ifeq ($(UNAME_S),Linux)
    PLATFORM := Linux
else ifneq (,$(findstring MINGW,$(UNAME_S)))
    PLATFORM := Windows
else ifneq (,$(findstring MSYS,$(UNAME_S)))
    PLATFORM := Windows
endif

ifeq ($(PLATFORM),macOS)
    CFLAGS = -g -Wall -I /opt/homebrew/include -I /opt/homebrew/opt/googletest/include -std=c++17
    LDFLAGS = -L /opt/homebrew/lib -L /opt/homebrew/opt/googletest/lib -L/opt/homebrew/opt/libpq/lib -lgtest -lgtest_main -lpthread -lpqxx -lpq
else ifeq ($(PLATFORM),Windows)
    CFLAGS = -g -Wall -I ./includes_library/pqxx/include -std=c++17
    LDFLAGS = -lpqxx -lpq
else
    CFLAGS = -g -Wall -std=c++17
    LDFLAGS = -lpqxx -lpq -pthread -lgtest -lgtest_main
endif

$(info Detected platform: $(PLATFORM))

# Directories and Sources
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests
BIN = bin
REPORT = Reports 
PR = Reports/ProviderReports
MR = Reports/MemberReports
MS = Reports/ManagerSummary 
EFT = Reports/EFTData 

# Source File Groups
MAIN = main
PROV = provider
PROV_USER = provider_user
MAN = manager
MEMBER = member
SQL = sqlengine
UTILS = utils
SERVICE = service
SERVICE_RECORD = service_record

# Binaries
TEST_BIN = runtests
TEST_SRC = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJ = $(patsubst $(TEST_DIR)/%.cpp,$(BIN)/%.o,$(TEST_SRC))

# Ensure bin/ exists
$(shell mkdir -p $(BIN))
$(shell mkdir -p $(REPORT) $(PR) $(MR) $(MS) $(EFT))

# Targets
all: main

main: $(BIN)/$(MAIN).o $(BIN)/$(PROV).o $(BIN)/$(PROV_USER).o $(BIN)/$(MAN).o \
      $(BIN)/$(SQL).o $(BIN)/$(UTILS).o $(BIN)/$(MEMBER).o \
      $(BIN)/$(SERVICE).o $(BIN)/$(SERVICE_RECORD).o
	$(COMP) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile Rules
$(BIN)/$(MAIN).o: $(SRC_DIR)/$(MAIN).cpp $(INCLUDE_DIR)/$(MAIN).h
	$(COMP) $(CFLAGS) -c $< -o $@

$(BIN)/$(PROV).o: $(SRC_DIR)/$(PROV).cpp $(INCLUDE_DIR)/$(PROV).h $(INCLUDE_DIR)/$(SQL).h
	$(COMP) $(CFLAGS) -c $< -o $@

$(BIN)/$(PROV_USER).o: $(SRC_DIR)/$(PROV_USER).cpp $(INCLUDE_DIR)/$(PROV_USER).h
	$(COMP) $(CFLAGS) -c $< -o $@

$(BIN)/$(MAN).o: $(SRC_DIR)/$(MAN).cpp $(INCLUDE_DIR)/$(MAN).h $(INCLUDE_DIR)/$(SQL).h
	$(COMP) $(CFLAGS) -c $< -o $@

$(BIN)/$(SQL).o: $(SRC_DIR)/$(SQL).cpp $(INCLUDE_DIR)/$(SQL).h
	$(COMP) $(CFLAGS) -c $< -o $@

$(BIN)/$(UTILS).o: $(SRC_DIR)/$(UTILS).cpp $(INCLUDE_DIR)/$(UTILS).h
	$(COMP) $(CFLAGS) -c $< -o $@

$(BIN)/$(MEMBER).o: $(SRC_DIR)/$(MEMBER).cpp $(INCLUDE_DIR)/$(MEMBER).h
	$(COMP) $(CFLAGS) -c $< -o $@

$(BIN)/$(SERVICE).o: $(SRC_DIR)/$(SERVICE).cpp $(INCLUDE_DIR)/$(SERVICE).h
	$(COMP) $(CFLAGS) -c $< -o $@

$(BIN)/$(SERVICE_RECORD).o: $(SRC_DIR)/$(SERVICE_RECORD).cpp $(INCLUDE_DIR)/$(SERVICE_RECORD).h
	$(COMP) $(CFLAGS) -c $< -o $@

# Test build
$(TEST_BIN): $(TEST_OBJ) $(BIN)/$(PROV).o $(BIN)/$(PROV_USER).o $(BIN)/$(MAN).o \
             $(BIN)/$(SQL).o $(BIN)/$(UTILS).o $(BIN)/$(MEMBER).o \
             $(BIN)/$(SERVICE).o $(BIN)/$(SERVICE_RECORD).o
	$(COMP) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile test files
$(BIN)/%.o: $(TEST_DIR)/%.cpp
	$(COMP) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Utility Targets
clean cls:
	rm -rf $(BIN)/*.o main $(TEST_BIN)

git:
	git add src/*.cpp include/*.h tests/*.cpp Makefile .gitignore
	@read -p "Commit message: " msg; git commit -m "$$msg"

test: $(TEST_BIN)
	./$(TEST_BIN)

.PHONY: clean cls git test
