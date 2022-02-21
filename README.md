# YCSB-C

Yahoo! Cloud Serving Benchmark in C++, a C++ version of YCSB (https://github.com/brianfrankcooper/YCSB/wiki)

## Quick Start

The current repo only supports LevelDB test. Before running the test, it is necessary to:
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

## Reference

https://github.com/a993096281/YCSB-HWDB