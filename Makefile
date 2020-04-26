CC=gcc
CFLAGS=-g -Wall
KEYLOG_TARGETS=main.o keylogger.o networking.o find_event_file.o keys.o

all: briefli

briefli: $(KEYLOG_TARGETS)
	$(CC) $^ -o $@ -lhiredis -lxdo

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
	-rm -f keylog server *.o
	@echo "Finished cleaning."
