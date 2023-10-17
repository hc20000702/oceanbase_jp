//
// Created by zx on 10/17/22.
//

#ifndef MINIDB_COLUMN_H
#define MINIDB_COLUMN_H

#include "Relation.h"

class Column {
private:
  Relation *relation_;
  int columnId_;
  Schema schema_;

public:
 Column(Relation* relation, int columnId): relation_(relation), columnId_(columnId){
   schema_ = relation->getSchema(columnId_);
 }
 AttrType attrType(){return schema_.attrType();}

 bool isNull(int index);
 int getInt(int index);
 float getFloat(int index);
 char getChar(int index);
 char getBool(int);
 const char* getText(int);
};

#endif  // MINIDB_COLUMN_H
