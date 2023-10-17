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
// Created by Meiyi & Wangyunlai.wyl on 2021/5/18.
//

#include "storage/common/index_meta.h"
#include "storage/common/field_meta.h"
#include "storage/common/table_meta.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "rc.h"
#include "json/json.h"

const static Json::StaticString FIELD_NAME("name");
const static Json::StaticString FIELD_FIELD_NAME("field_name");
const static Json::StaticString FIELD_UNIQUE("unique");

RC IndexMeta::init(const char *name, const std::vector<const FieldMeta*> &fields, size_t is_unique)
{
  if (common::is_blank(name)) {
    LOG_ERROR("Failed to init index, name is empty.");
    return RC::INVALID_ARGUMENT;
  }

  name_ = name;
  for (const FieldMeta *fieldMeta: fields) {
    std::string field_name(fieldMeta->name());
    fields_.emplace_back(field_name);
  }
  is_unique_ = is_unique;
  return RC::SUCCESS;
}

void IndexMeta::to_json(Json::Value &json_value) const
{
  json_value[FIELD_NAME] = name_;
  json_value[FIELD_UNIQUE] = (int)is_unique_;
  for (std::string field_ : fields_) {
    json_value[FIELD_FIELD_NAME].append(field_);
  }
}

RC IndexMeta::from_json(const TableMeta &table, const Json::Value &json_value, IndexMeta &index)
{
  const Json::Value &name_value = json_value[FIELD_NAME];
  const Json::Value &is_unique = json_value[FIELD_UNIQUE];
  const Json::Value &field_values = json_value[FIELD_FIELD_NAME];
  const int field_size = field_values.size();
  std::vector<const FieldMeta*> fields;
  if (!name_value.isString()) {
    LOG_ERROR("Index name is not a string. json value=%s", name_value.toStyledString().c_str());
    return RC::GENERIC_ERROR;
  }

  for (int i = 0; i < field_size; i++) {
    if (!field_values[i].isString()) {
      LOG_ERROR("Field name of index [%s] is not a string. json value=%s",
          name_value.asCString(),
          field_values[i].toStyledString().c_str());
      return RC::GENERIC_ERROR;
    }
    const FieldMeta *field = table.field(field_values[i].asCString());
    if (nullptr == field) {
      LOG_ERROR("Deserialize index [%s]: no such field: %s", name_value.asCString(), field_values[i].asCString());
      return RC::SCHEMA_FIELD_MISSING;
    }
    fields.emplace_back(field);
  }

  return index.init(name_value.asCString(), fields, is_unique.asUInt64());
}

const char *IndexMeta::name() const
{
  return name_.c_str();
}

const std::vector<std::string> *IndexMeta::fields() const
{
  return &fields_;
}

const bool IndexMeta::is_multi() const
{
  return fields_.size() > 1;
}

const bool IndexMeta::is_unique() const
{
  return is_unique_ == 1;
}

void IndexMeta::desc(std::ostream &os) const
{
  os << "index name=" << name_ << ", field(s)=";
  for (const std::string &field : fields_) {
    os << field;
  }
}