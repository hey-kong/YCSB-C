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
    result.push_back(std::make_pair(key, value));
  } else if (s.IsNotFound()) {
    noResult++;
  } else {
    cerr << "read error" << endl;
    exit(0);
  }
  return DB::kOK;
}

int LevelDB::Scan(const std::string& table, const std::string& key, int len,
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
  for (KVPair p : values) {
    value += p.second;
  }
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

}  // namespace ycsbc
