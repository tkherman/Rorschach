CC=			g++
CFLAGS=		-g -gdwarf-2 -Wall -std=c++11
LD=			g++
LDFLAGS=	-L.
AR=			ar
ARFLAGS=	rcs
TARGETS=	rorschach

all: $(TARGETS)

rorschach: main.o loadRules.o utilities.o scan.o detect.o execute.o
	@echo Linking rorschach...
	@$(LD) $(LDFLAGS) -o $@ $^

main.o: main.cpp rorschach.h
	@echo Compiling main.o...
	@$(CC) $(CFLAGS) -o $@ -c $<

loadRules.o: loadRules.cpp rorschach.h
	@echo Compiling loadRules.o...
	@$(CC) $(CFLAGS) -o $@ -c $<

scan.o: scan.cpp rorschach.h
	@echo Compiling scan.o...
	@$(CC) $(CFLAGS) -o $@ -c $<

utilities.o: utilities.cpp rorschach.h
	@echo Compiling utilities.o...
	@$(CC) $(CFLAGS) -o $@ -c $<

detect.o: detect.cpp rorschach.h
	@echo Compiling detect.o...
	@$(CC) $(CFLAGS) -o $@ -c $<

execute.o: execute.cpp rorschach/h
	@echo Compiling execute.o...
	@$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@echo Cleaning...
	@rm -f $(TARGETS) *.o
