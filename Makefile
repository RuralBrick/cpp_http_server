CC=g++
CFLAGS=-I.
DEPS=
OBJ=server.o
USERID=123456789
QUANID=
THEOID=405462550

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: server
server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o server *.tar.gz

dist: tarball
tarball: clean
	tar -cvzf /tmp/$(USERID).tar.gz --exclude=./.vagrant . && mv /tmp/$(USERID).tar.gz .
tarballquan: clean
	tar -cvzf /tmp/$(QUANID).tar.gz --exclude=./.vagrant . && mv /tmp/$(QUANID).tar.gz .
tarballtheo: clean
	tar -cvzf /tmp/$(THEOID).tar.gz --exclude=./.vagrant . && mv /tmp/$(THEOID).tar.gz .
