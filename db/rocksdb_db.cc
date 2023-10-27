#include "rocksdb_db.h"

#include <iostream>

#include "lib/coding.h"

using namespace std;

namespace ycsbc {

RocksDB::RocksDB(const char* dbfilename, utils::Properties& props)
    : noResult(0), dbstats_(nullptr), write_sync_(false) {
  // set option first
  rocksdb::Options options;
  SetOptions(&options, props);

  rocksdb::Status s = rocksdb::DB::Open(options, dbfilename, &db_);
  if (!s.ok()) {
    cerr << "Can't open rocksdb " << dbfilename << " " << s.ToString() << endl;
    exit(0);
  }
}

void RocksDB::SetOptions(rocksdb::Options* options, utils::Properties& props) {
  options->create_if_missing = true;
  options->compression = rocksdb::kLZ4Compression;
  options->max_background_jobs = 2;

  // memtable size
  options->write_buffer_size = 64 * 1024 * 1024;
  // file size
  options->target_file_size_base = 64 * 1024 * 1024;
  // L1 size
  options->max_bytes_for_level_base = 256 * 1024 * 1024;
  // block size
  options->block_size = 4 * 1024;
  // block cache size
  options->block_cache_size = 8 * 1024 * 1024;

  options->use_direct_reads = true;
  options->use_direct_io_for_flush_and_compaction = true;
}

int RocksDB::Read(const std::string& table, const std::string& key,
                  const std::vector<std::string>* fields,
                  std::vector<KVPair>& result) {
  string value;
  rocksdb::Status s = db_->Get(rocksdb::ReadOptions(), key, &value);
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

int RocksDB::Scan(const std::string& table, const std::string& key, int len,
                  const std::vector<std::string>* fields,
                  std::vector<std::vector<KVPair>>& result) {
  auto it = db_->NewIterator(rocksdb::ReadOptions());
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

int RocksDB::Insert(const std::string& table, const std::string& key,
                    std::vector<KVPair>& values) {
  rocksdb::Status s;
  string value;
  for (KVPair p : values) {
    value += p.second;
  }
  rocksdb::WriteOptions write_options = rocksdb::WriteOptions();
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

int RocksDB::Update(const std::string& table, const std::string& key,
                    std::vector<KVPair>& values) {
  return Insert(table, key, values);
}

int RocksDB::Delete(const std::string& table, const std::string& key) {
  rocksdb::Status s;
  rocksdb::WriteOptions write_options = rocksdb::WriteOptions();
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

void RocksDB::PrintStats() {
  if (noResult) cout << "read not found:" << noResult << endl;
  string stats;
  db_->GetProperty("rocksdb.stats", &stats);
  cout << stats << endl;

  if (dbstats_.get() != nullptr) {
    fprintf(stdout, "STATISTICS:\n%s\n", dbstats_->ToString().c_str());
  }
}

RocksDB::~RocksDB() { delete db_; }

}  // namespace ycsbc
