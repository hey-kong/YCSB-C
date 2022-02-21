#include "leveldb_db.h"

#include <iostream>

#include "lib/coding.h"

using namespace std;

namespace ycsbc {

LevelDB::LevelDB(const char* dbpath, utils::Properties& props)
    : noResult(0), write_sync_(false) {
  // set options first
  leveldb::Options options;
  SetOptions(&options, props);

  leveldb::Status s = leveldb::DB::Open(options, dbpath, &db_);
  if (!s.ok()) {
    cerr << "Can't open leveldb " << dbpath << " " << s.ToString() << endl;
    exit(0);
  }
}

void LevelDB::SetOptions(leveldb::Options* options, utils::Properties& props) {
  options->create_if_missing = true;
  options->compression = leveldb::kNoCompression;
  options->write_buffer_size = 128 * 1024 * 1024;  // memtable max size, 128MB
  options->max_file_size = 128 * 1024 * 1024;      // sstable size, 128MB
}

int LevelDB::Read(const std::string& table, const std::string& key,
                  const std::vector<std::string>* fields,
                  std::vector<KVPair>& result) {
  string value;
  leveldb::Status s = db_->Get(leveldb::ReadOptions(), key, &value);
  if (s.ok()) {
    DeserializeValue(value, result);
    return DB::kOK;
  }
  if (s.IsNotFound()) {
    noResult++;
    return DB::kOK;
  } else {
    cerr << "read error" << endl;
    exit(0);
  }
}

int LevelDB::Scan(const std::string& table, const std::string& key,
                  const std::string& max_key, int len,
                  const std::vector<std::string>* fields,
                  std::vector<std::vector<KVPair>>& result) {
  auto it = db_->NewIterator(leveldb::ReadOptions());
  it->Seek(key);
  std::string val;
  std::string k;
  for (int i = 0; i < len && it->Valid(); i++) {
    k = it->key().ToString();
    val = it->value().ToString();
    it->Next();
  }
  delete it;
  return DB::kOK;
}

int LevelDB::Insert(const std::string& table, const std::string& key,
                    std::vector<KVPair>& values) {
  leveldb::Status s;
  string value;
  SerializeValue(values, value);
  leveldb::WriteOptions write_options = leveldb::WriteOptions();
  if (write_sync_) {
    write_options.sync = true;
  }
  s = db_->Put(write_options, key, value);
  if (!s.ok()) {
    cerr << "insert error\n" << endl;
    exit(0);
  }

  return DB::kOK;
}

int LevelDB::Update(const std::string& table, const std::string& key,
                    std::vector<KVPair>& values) {
  return Insert(table, key, values);
}

int LevelDB::Delete(const std::string& table, const std::string& key) {
  leveldb::Status s;
  leveldb::WriteOptions write_options = leveldb::WriteOptions();
  if (write_sync_) {
    write_options.sync = true;
  }
  s = db_->Delete(write_options, key);
  if (!s.ok()) {
    cerr << "Delete error\n" << endl;
    exit(0);
  }
  return DB::kOK;
}

void LevelDB::PrintStats() {
  if (noResult) cout << "read not found:" << noResult << endl;
  string stats;
  db_->GetProperty("leveldb.stats", &stats);
  cout << stats << endl;
}

LevelDB::~LevelDB() { delete db_; }

void LevelDB::SerializeValue(std::vector<KVPair>& kvs, std::string& value) {
  value.clear();
  PutFixed64(&value, kvs.size());
  for (unsigned int i = 0; i < kvs.size(); i++) {
    PutFixed64(&value, kvs[i].first.size());
    value.append(kvs[i].first);
    PutFixed64(&value, kvs[i].second.size());
    value.append(kvs[i].second);
  }
}

void LevelDB::DeserializeValue(std::string& value, std::vector<KVPair>& kvs) {
  uint64_t offset = 0;
  uint64_t kv_num = 0;
  uint64_t key_size = 0;
  uint64_t value_size = 0;

  kv_num = DecodeFixed64(value.c_str());
  offset += 8;
  for (unsigned int i = 0; i < kv_num; i++) {
    ycsbc::DB::KVPair pair;
    key_size = DecodeFixed64(value.c_str() + offset);
    offset += 8;

    pair.first.assign(value.c_str() + offset, key_size);
    offset += key_size;

    value_size = DecodeFixed64(value.c_str() + offset);
    offset += 8;

    pair.second.assign(value.c_str() + offset, value_size);
    offset += value_size;
    kvs.push_back(pair);
  }
}

}  // namespace ycsbc
