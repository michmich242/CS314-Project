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
	CFLAGS = -g -Wall -I /opt/homebrew/include -std=c++17
else ifeq ($(PLATFORM),Windows)
	CFLAGS = -g -Wall -I ./includes_library/pqxx/include -std=c++17
else
	CFLAGS = -g -Wall -lpqxx -lpq
endif


$(info Detected platform: $(PLATFORM))


$(shell mkdir -p bin)


#---------------- FOR MAC --------------

# To install brew on the terminal:
# /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

#brew install postgresql
#brew install libpqxx


#CFLAGS = -g -Wall -I $(shell brew --prefix)/include -std=c++17


#----------------- FOR WINDOWS --------------

# Go on Google and search up MSYS2 UCRT64 and install it, or you can install MINGW64 which installs all compiling managers (Clang64, UCRT64, MINGW64)

# pacman -S mingw-w64-ucrt-x86_64-libpqxx mingw-w64-ucrt-x86_64-postgresql

#CFLAGS = -g -Wall -I C:\msys64\ucrt64\include -std=c++17


#---------------------------------------------------





# Linker flags
LDFLAGS = -lpqxx -lpq
GTEST_FLAGS = -lgtest -lgtest_main -pthread


SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests
BIN = bin


TEST_BIN = runtests
TEST_SRC = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJ = $(patsubst $(TEST_DIR)/%.cpp,$(BIN)/%.o,$(TEST_SRC))

MAIN = main
PROV = provider
MAN = manager
SQL = sqlengine


all: $(MAIN)

$(MAIN): $(BIN)/$(MAIN).o $(BIN)/$(PROV).o $(BIN)/$(MAN).o $(BIN)/$(SQL).o
	$(COMP) $(CFLAGS) -o $@ $^

$(BIN)/$(MAIN).o: $(SRC_DIR)/$(MAIN).cpp $(INCLUDE_DIR)/$(MAIN).h
	$(COMP) $(CFLAGS) -c $< -o $@

$(BIN)/$(SQL).o: $(SRC_DIR)/$(SQL).cpp $(INCLUDE_DIR)/$(SQL).h
	$(COMP) $(CFLAGS) -c $< -o $@

$(BIN)/$(MAN).o: $(SRC_DIR)/$(MAN).cpp $(INCLUDE_DIR)/$(MAN).h $(INCLUDE_DIR)/$(SQL).h
	$(COMP) $(CFLAGS) -c $< -o $@

$(BIN)/$(PROV).o: $(SRC_DIR)/$(PROV).cpp $(INCLUDE_DIR)/$(PROV).h $(INCLUDE_DIR)/$(SQL).h
	$(COMP) $(CFLAGS) -c $< -o $@


#install google tests for use
$(TEST_BIN): $(TEST_OBJ) $(BIN)/$(MAN).o $(BIN)/$(PROV).o $(BIN)/$(SQL).o
	$(COMP) $(CFLAGS) -o $@ $^ $(GTEST_FLAGS)

$(BIN)/%.o: $(TEST_DIR)/%.cpp
	$(COMP) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# clean or cls will clear out all extra parts of the program like object files
# and executables.
clean cls:
	rm -rf $(BIN)/*.o $(MAIN) $(TEST_BIN)

# The "git:" part allows you to just type "make git" instead of
# typing git add and git commit individually
# Note: You'll still have to type the commit message
# Note: You'll still have to type "git push" to make changes to the repo
git:
	git add *.cpp *.h ?akefile .gitignore
	git commit

# Run all tests
test: $(TEST_BIN)
	./$(TEST_BIN)
