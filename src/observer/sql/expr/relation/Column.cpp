//
// Created by zx on 10/17/22.
//

#include "Column.h"

bool Column::isNull(int index){
  return relation_->getItem(index, columnId_)->isNull();
}
int Column::getInt(int index){
  return relation_->getItem(index, columnId_)->getInt();
}
float Column::getFloat(int index){
  return relation_->getItem(index, columnId_)->getFloat();
}
char Column::getChar(int index){
  return relation_->getItem(index, columnId_)->getChar();
}
char Column::getBool(int index){
  return relation_->getItem(index, columnId_)->getBool();
}
const char* Column::getText(int index){
  return relation_->getItem(index, columnId_)->getText();
}