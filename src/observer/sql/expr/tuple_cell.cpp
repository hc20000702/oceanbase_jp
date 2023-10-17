/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by WangYunlai on 2022/07/05.
//

#include "sql/expr/tuple_cell.h"
#include "storage/common/field.h"
#include "common/log/log.h"
#include "util/comparator.h"
#include "util/util.h"

void TupleCell::to_string(std::ostream &os) const
{
  switch (attr_type_) {
  case INTS: {
    os << *(int *)data_;
  } break;
  case FLOATS: {
    float v = *(float *)data_;
    os << double2string(v);
  } break;
  case CHARS: {
    for (int i = 0; i < length_; i++) {
      if (data_[i] == '\0') {
        break;
      }
      os << data_[i];
    }
  } break;
  case DATES: {
    int v = *(int *)data_;
    os << date2string(v);
  } break;
  case NULLS: {
    os << "NULL";
  } break;
  default: {
    LOG_WARN("unsupported attr type: %d", attr_type_);
  } break;
  }
}

int TupleCell::getDataAsInt()
{
  return *(int *)data_;
}

float TupleCell::getDataAsFloat()
{
  return *(float*) data_;
}

bool TupleCell::getDataAsBool()
{
  return *(bool*) data_;
}

char TupleCell::getDataAsChar()
{
  return *(char*) data_;
}

char* TupleCell::getDataAsText(){
  return data_;
}

void TupleCell::set_data(int data){
  set_type(INTS);
  length_ = sizeof(data);
  int* ptr = new int(data);
  data_ = (char *) ptr;
}

void TupleCell::set_data(float data){
  set_type(FLOATS);
  length_ = sizeof(data);
  float* ptr = new float(data);
  data_ = (char *) ptr;
}

RC TupleCell::compare(const TupleCell &other, int &result, const CompOp &comp) const
{
  result = 0;
  RC rc = RC::SUCCESS;
  AttrType t1 = this->is_null_ ? NULLS : this->attr_type_;
  AttrType t2 = other.is_null_ ? NULLS : other.attr_type_;
  rc = compare_all(t1, t2, this->data_, other.data_, this->length_, other.length_, result, comp);

  return rc;
}


