# Yahoo! Cloud System Benchmark
# Workload A: Insert heavy workload
#   Application example: compaction
#                        
#   Insert ratio: 100
#   Default data size: 8 bytes each key, 8 bytes each value
#   Request distribution: zipfian

keylength=8
fieldcount=1
fieldlength=8

recordcount=20000000
operationcount=0
workload=com.yahoo.ycsb.workloads.CoreWorkload

readallfields=true

readproportion=0
updateproportion=0
scanproportion=0
insertproportion=1

requestdistribution=zipfian

