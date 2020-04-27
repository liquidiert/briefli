CC=gcc
CFLAGS=-Wall
CXXFLAGS=-g -Wall
BRIEFLI_TARGETS=main.o keylogger.o networking.o find_event_file.o keys.o
DEBUG_TARGETS=main.c keylogger.c networking.c find_event_file.c keys.c

all: briefli

briefli: $(BRIEFLI_TARGETS)
	$(CC) $^ -o $@ -lhiredis -lxdo
	
debug: $(DEBUG_TARGETS)
	$(CC) $(CXXFLAGS) $^ -o $@ -lhiredis -lxdo

.PHONY: install
install:
	@echo "Installing briefli"
	cp briefli /usr/local/bin
	@echo "Installed briefli"

.PHONY: uninstall
uninstall:
	@echo "Uninstalling briefli"
	cp briefli /usr/local/bin
	@echo "Successfully uninstalled briefli"

.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f briefli debug *.o
	@echo "Finished cleaning."
