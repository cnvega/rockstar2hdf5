CC = h5cc
H5CC = h5cc
CCFLAGS = -std=c99 
CCFLAGS += -O2 -Wno-unused-result -Wno-unused-but-set-variable
#CCFLAGS += -g -O0

all: ascii2bin  bin2ascii ascii2hdf5 split_ascii split_binary

.PHONY: clean

%.o: %.c
	$(CC) -c $< -o $@ $(CCFLAGS)

ascii2bin: ascii2bin.o halos.o
	$(CC) ascii2bin.o halos.o -o ascii2bin $(CCFLAGS)

bin2ascii: bin2ascii.o halos.o
	$(CC) bin2ascii.o halos.o -o bin2ascii $(CCFLAGS)

split_ascii: split_ascii.o halos.o
	$(CC) split_ascii.o halos.o -o split_ascii $(CCFLAGS)

split_binary: split_binary.o halos.o
	$(CC) split_binary.o halos.o -o split_binary $(CCFLAGS)

ascii2hdf5: ascii2hdf5.o halos.o
	$(H5CC) ascii2hdf5.o halos.o -o ascii2hdf5 $(CCFLAGS)

clean:
	rm -f ascii2bin bin2ascii ascii2hdf5 split_ascii split_binary *.o

