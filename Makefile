MAIN = main
PROV = provider
MAN = manager
SQL = sqlengine
COMP = g++
# we can add to these as well if we want to do so.
CFLAGS = -g -Wall

all: $(MAIN)

$(MAIN): $(MAIN).o $(PROV).o $(MAN).o $(SQL).o
	$(COMP) $(CFLAGS) -o $@ $^

$(MAIN).o: $(MAIN).cpp $(MAIN).h
	$(COMP) $(CFLAGS) -c $<

$(SQL).o: $(SQL).cpp $(SQL).h
	$(COMP) $(CFLAGS) -c $<

$(MAN).o: $(MAN).cpp $(MAN).h $(SQL).h
	$(COMP) $(CFLAGS) -c $<

$(PROV).o: $(PROV).cpp $(PROV).h $(SQL).h
	$(COMP) $(CFLAGS) -c $<

# clean or cls will clear out all extra parts of the program like object files
# and executables.
clean cls:
	rm -rf *.o $(MAIN) $(SQL) $(MAN) $(PROV)

# When using make git, it will prompt you for a commit message,
# type it and then write and quit, you still must call git push afterwards.
git:
	git add *.cpp *.h ?akefile
	git commit

# Note: we must figure out a method for unit testing, this may be separate cpp files that
# will need to be added to this makefile as we iterate through the process.
