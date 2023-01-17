CC=g++
CFLAGS:=$(CFLAGS) -I. -Wall
SRC=$(wildcard *.cpp)
DEPS=$(SRC:.cpp=.d)
OBJ=$(SRC:.cpp=.o)
USERID=123456789
QUANID=105490012
THEOID=405462550

all: server
server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.cpp Makefile
	$(CC) -c -o $@ $< $(CFLAGS) -MMD -MP

-include $(DEPS)

clean:
	rm -rf *.o *.d server *.tar.gz

dist: tarball
tarball: clean
	tar -cvzf /tmp/$(USERID).tar.gz --exclude=./.vagrant . && mv /tmp/$(USERID).tar.gz .
tarballquan: clean
	tar -cvzf /tmp/$(QUANID).tar.gz --exclude=./.vagrant . && mv /tmp/$(QUANID).tar.gz .
tarballtheo: clean
	tar -cvzf /tmp/$(THEOID).tar.gz --exclude=./.vagrant . && mv /tmp/$(THEOID).tar.gz .
