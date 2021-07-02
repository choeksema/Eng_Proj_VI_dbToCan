CC=g++
EXE=dbTesting
#Clear to turn off warnings
WARN=-Wall

SDIR=src

IDIR=include
CFLAGS=-I$(IDIR)

# Likely needs to change per platform
LDIR=-L/usr/lib/arm-linux-gnueabihf
LIBS=-lmysqlcppconn -lpcan

DEPS= $(wildcard $(IDIR)/*.h)
#$(info	Header files: $(DEPS))

SRCS=$(wildcard $(SDIR)/*.cpp)
#$(info	Source files: $(SRCS))

ODIR=obj
OBJ=$(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SRCS))
#$(info	Obj files: $(OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) $(WARN) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ)
	$(CC) $(WARN) -o $@ $^ $(CFLAGS) $(LDIR) $(LIBS)

.PHONY: clean # don't mess up any files named "clean"

clean:
	rm -f $(ODIR)/*.o
