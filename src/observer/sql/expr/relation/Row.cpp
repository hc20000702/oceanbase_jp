//
// Created by zx on 10/17/22.
//

#include "Row.h"
void Row::add(int index, Item* value){
  values[index] = value->copyItem();
}

void Row::add(Item* value){
  values.emplace_back(value->copyItem());
}

int Row::getInt(int index){
  return values[index]->getInt();
}

float Row::getFloat(int index){
  return values[index]->getFloat();
}

bool Row::getBool(int index){
  return values[index]->getBool();
}

const char* Row::getText(int index){
  return values[index]->getText();
}

char Row::getChar(int index) {
  return values[index]->getChar();
}

Item* Row::getItem(int index){
  return values[index];
}

RC Row::rewrite(int index, Item* item){
  if(index >= size()){
    return RC::GENERIC_ERROR;
  }
  delete values[index];
  values[index] = item;
  return RC::SUCCESS;
}

Row* Row::tupleToRow(RowTuple* tuple){
  Row* row = new Row;
  int width = tuple->cell_num();
  // const TupleCellSpec *cellSpec; unused
  TupleCell cell;
  // 0: trx, 1: null
  for(int i = 2; i < width; ++i){
    tuple->cell_at(i, cell);

    Item* item;
    switch(cell.attr_type()){
      case INTS:
        item = new IntItem(cell.getDataAsInt(), cell.is_null());
        row->add(item);
        break;
      case DATES:
        item = new DateItem(cell.getDataAsInt(), cell.is_null());
        row->add(item);
        break;
      case FLOATS:
        item = new FloatItem(cell.getDataAsFloat(), cell.is_null());
        row->add(item);
        break;
      case CHARS:
        item = new TextItem(cell.getDataAsText(), cell.is_null());
        row->add(item);
        break;
    //  case NULLS:
    //     item = new NullItem();
    //     row->add(item);
    //     break;
      default:
        LOG_WARN("should not happen in Row.cpp");
        break;
    }
  }

  return row;
}

void Row::releaseAll(){
//  for(auto it: values) {
////    it->release();
////    delete it;
//  }
}

void Row::insertRow(const Row& row){
  for(auto it: row.values){
    values.emplace_back(it->copyItem());
  }
}

Row& Row::operator=(const Row& toCopy){
  this->values.clear();
  for(size_t i = 0 ; i < toCopy.width(); ++i){
    this->values.emplace_back(toCopy.at(i)->copyItem());
  }
  return *this;
}