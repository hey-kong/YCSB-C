#ifndef YCSB_C_LEVELDB_DB_H
#define YCSB_C_LEVELDB_DB_H

#include <leveldb/db.h>

#include <iostream>
#include <string>

#include "core/core_workload.h"
#include "core/db.h"
#include "core/properties.h"

using std::cout;
using std::endl;

namespace ycsbc {

class LevelDB : public DB {
 public:
  LevelDB(const char* dbpath, utils::Properties& props);
  int Read(const std::string& table, const std::string& key,
           const std::vector<std::string>* fields, std::vector<KVPair>& result);

  int Scan(const std::string& table, const std::string& key, int len,
           const std::vector<std::string>* fields,
           std::vector<std::vector<KVPair>>& result);

  int Insert(const std::string& table, const std::string& key,
             std::vector<KVPair>& values);

  int Update(const std::string& table, const std::string& key,
             std::vector<KVPair>& values);

  int Delete(const std::string& table, const std::string& key);

  void PrintStats();

  ~LevelDB();

 private:
  leveldb::DB* db_;
  unsigned noResult;
  bool write_sync_;

  void SetOptions(leveldb::Options* options, utils::Properties& props);
};

}  // namespace ycsbc

#endif  // YCSB_C_LEVELDB_DB_H
