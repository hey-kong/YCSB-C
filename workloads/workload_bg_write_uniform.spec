# Yahoo! Cloud System Benchmark
# Workload A: Update heavy workload
#   Application example: compaction
#                        
#   Insert ratio: 100
#   Default data size: 8 bytes each key, 248 bytes each value
#   Request distribution: uniform

keylength=8
fieldcount=1
fieldlength=248

recordcount=500000
operationcount=400000
workload=com.yahoo.ycsb.workloads.CoreWorkload

readallfields=true

readproportion=0.5
updateproportion=0.5
scanproportion=0
insertproportion=0

requestdistribution=uniform
