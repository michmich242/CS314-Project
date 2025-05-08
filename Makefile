MAIN = main
PROV = provider
MAN = manager
SQL = sqlengine
COMP = g++
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

clean cls:
	rm -rf *.o $(MAIN) $(SQL) $(MAN) $(PROV)

git:
	git add *.cpp *.h ?akefile
	git commit
