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
// Created by WangYunlai on 2022/6/7.
//

#pragma once

#include <iostream>
#include <string.h>
#include "storage/common/table.h"
#include "storage/common/field_meta.h"

class TupleCell
{
public: 
  TupleCell() = default;
  
  // TupleCell(FieldMeta *meta, char *data)
  //     : TupleCell(meta->type(), data)
  // {}
  TupleCell(AttrType attr_type, char *data, bool is_null)
      : attr_type_(attr_type), data_(data), is_null_(is_null)
  {
    switch(attr_type) {
      case INTS:
      case DATES:
        length_ = sizeof(int);
        break;
      case FLOATS:
        length_ = sizeof(float);
        break;
      case CHARS:
        length_ = strlen(data);
        break;
      case NULLS:
        is_null_ = true;
      default:
        break;
    }
  }

  // TODO: 可能内存泄露， 记得delete data_

  void set_type(AttrType type) { this->attr_type_ = type; }
  void set_length(int length) { this->length_ = length; }
  void set_data(char *data) { this->data_ = data; }
  void set_data(const char *data) { this->set_data(const_cast<char *>(data)); }

  void set_data(int data);
  void set_data(float data);

  void set_null(bool is_null) { this->is_null_ = is_null; }
  bool is_null() { return this->is_null_; }

  void to_string(std::ostream &os) const;

  RC compare(const TupleCell &other, int & result, const CompOp &comp) const;


  const char *data() const
  {
    return data_;
  }

  int length() const { return length_; }

  AttrType attr_type() const
  {
    return attr_type_;
  }

  int getDataAsInt();
  float getDataAsFloat();
  bool getDataAsBool();
  char getDataAsChar();
  char* getDataAsText();

private:
  AttrType attr_type_ = UNDEFINED;
  int length_ = -1;
  char *data_ = nullptr; // real data. no need to move to field_meta.offset
  bool is_null_ = false;
};
