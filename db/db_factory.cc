//
//  basic_db.cc
//  YCSB-C
//
//  Created by Jinglei Ren on 12/17/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#include "db/db_factory.h"

#include <string>

#include "db/basic_db.h"

#ifdef YCSB_LEVELDB
#include "db/leveldb_db.h"
#endif

using namespace std;
using ycsbc::DB;
using ycsbc::DBFactory;

DB* DBFactory::CreateDB(utils::Properties& props) {
  if (props["dbname"] == "basic") {
    return new BasicDB;
  }

#ifdef YCSB_LEVELDB
  else if (props["dbname"] == "leveldb") {
    std::string dbpath = props.GetProperty("dbpath", "/tmp/test-leveldb");
    return new LevelDB(dbpath.c_str(), props);
  }
#endif
  else
    return NULL;
}
