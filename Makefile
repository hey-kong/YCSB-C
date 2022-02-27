CC=g++
CXXFLAGS=-std=c++11 -g -Wall -I./ 
LDFLAGS= -lpthread -ldl

LIB_SOURCES= \
		core/core_workload.cc  \
		db/db_factory.cc   \

## leveldb
LEVELDB_SOURCES= db/leveldb_db.cc
LEVELDB_LIBRARY= -lleveldb 
LEVELDB_DEFS= -DYCSB_LEVELDB
LEVELDB_OBJECTS=$(LEVELDB_SOURCES:.cc=.o)
##

## rocksdb
ROCKSDB_SOURCES= db/rocksdb_db.cc
ROCKSDB_LIBRARY= -lrocksdb -lsnappy -lz -lbz2 -ltcmalloc -lzstd -llz4
ROCKSDB_DEFS= -DYCSB_ROCKSDB
ROCKSDB_OBJECTS=$(ROCKSDB_SOURCES:.cc=.o)
##

OBJECTS=$(LIB_SOURCES:.cc=.o)
EXEC=ycsbc

ONLY_LEVELDB_SOURCES=$(LIB_SOURCES) $(LEVELDB_SOURCES)
ONLY_ROCKSDB_SOURCES=$(LIB_SOURCES) $(ROCKSDB_SOURCES)
ALL_SOURCES=$(LIB_SOURCES) $(LEVELDB_SOURCES) $(ROCKSDB_SOURCES)

all: clean
	$(CC) $(CXXFLAGS) $(LEVELDB_DEFS) $(ROCKSDB_DEFS) ycsbc.cc $(ALL_SOURCES) -o $(EXEC) $(LDFLAGS) $(LEVELDB_LIBRARY) $(ROCKSDB_LIBRARY)

leveldb: clean
	$(CC) $(CXXFLAGS) $(LEVELDB_DEFS) ycsbc.cc $(ONLY_LEVELDB_SOURCES) -o $(EXEC) $(LDFLAGS) $(LEVELDB_LIBRARY)

rocksdb: clean
	$(CC) $(CXXFLAGS) $(ROCKSDB_DEFS) ycsbc.cc $(ONLY_ROCKSDB_SOURCES) -o $(EXEC) $(LDFLAGS) $(ROCKSDB_LIBRARY)

clean:
	rm -f $(EXEC) 

.PHONY: clean leveldb rocksdb 
