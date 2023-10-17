//
// Created by zx on 10/17/22.
//

#ifndef MINIDB_ROW_H
#define MINIDB_ROW_H

#include "Item.h"
#include "vector"
#include "rc.h"
#include "sql/expr/tuple.h"

class Row {
public:
  Row() = default;
  Row(int length): values(length){}
  void add(int index, Item* value);
  void add(Item* value);
  RC rewrite(int index, Item* item);
  int size() const{return values.size();}

  void clean(){values.clear();}
  int getInt(int index);
  float getFloat(int index);
  bool getBool(int index);
  const char* getText(int index);
  char getChar(int index);

  Item* at(int index) const{return values[index];}

  static Row* tupleToRow(RowTuple* tuple);

  Item* getItem(int index);

  void releaseAll();

  void insertRow(const Row& row);
  int width() const { return values.size();}
  bool empty(){ return values.empty();}

  Row& operator=(const Row& p);

  void append(Row row){
    for(int i = 0 ; i < row.size(); ++i){
      values.emplace_back(row.at(i)->copyItem());
    }
  }
private:
  std::vector<Item*> values;
};

#endif  // MINIDB_ROW_H
