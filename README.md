# YCSB-C

Yahoo! Cloud Serving Benchmark in C++, a C++ version of YCSB (https://github.com/brianfrankcooper/YCSB/wiki)

## Quick Start

The current repo only supports LevelDB and RocksDB test.

### LevelDB
Before running the test, it is necessary to:
- copy `libleveldb.a` to the path `/usr/local/lib/`
- copy `include/leveldb/` to the path `/usr/local/include/`

Build YCSB-C:
```
make leveldb
```

Run Workload C of LevelDB, for example:
```
./ycsbc -db leveldb -dbpath /dbtest/leveldb-test -threads 1 -P workloads/workloadc.spec -run true -load true
```

### RocksDB
Before running the test, it is necessary to:
- [install RocksDB](https://github.com/facebook/rocksdb/blob/master/INSTALL.md) - `make static_lib`, compile librocksdb.a, RocksDB static library
- copy `librocksdb.a` to the path `/usr/local/lib/`
- copy `include/rocksdb/` to the path `/usr/local/include/`

Build YCSB-C:
```
make rocksdb
```

Run Workload C of RocksDB, for example:
```
./ycsbc -db rocksdb -dbpath /dbtest/rocksdb-test -threads 1 -P workloads/workloadc.spec -run true -load true
```

## Reference

https://github.com/a993096281/YCSB-HWDB
