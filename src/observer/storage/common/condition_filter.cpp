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
// Created by Meiyi & Wangyunlai on 2021/5/7.
//

#include <stddef.h>
#include <math.h>
#include "condition_filter.h"
#include "storage/record/record_manager.h"
#include "common/log/log.h"
#include "storage/common/table.h"
#include "util/util.h"

using namespace common;

RC compare_all(AttrType &t1, AttrType &t2, void *data1, void *data2, int len1, int len2, int &result, const CompOp &comp);

//   UNDEFINED, CHARS, INTS, FLOATS, DATES, NULLS
const bool field_type_compare_compatible_table[6][6] = {
  {0,0,0,0,0,0},
  {0,1,1,1,0,1},
  {0,1,1,1,0,1},
  {0,1,1,1,0,1},
  {0,0,0,0,1,1},
  {0,1,1,1,1,1}
};

ConditionFilter::~ConditionFilter()
{}

DefaultConditionFilter::DefaultConditionFilter()
{
  left_.is_attr = false;
  left_.attr_length = 0;
  left_.attr_offset = 0;
  left_.value = nullptr;

  right_.is_attr = false;
  right_.attr_length = 0;
  right_.attr_offset = 0;
  right_.value = nullptr;
}
DefaultConditionFilter::~DefaultConditionFilter()
{}

RC DefaultConditionFilter::init(const ConDesc &left, const ConDesc &right, AttrType type_left, AttrType type_right, CompOp comp_op)
{
  if (type_left <= UNDEFINED || type_right <= UNDEFINED) {
    LOG_ERROR("Invalid condition with unsupported attribute type: %d, %d", type_left, type_right);
    return RC::INVALID_ARGUMENT;
  }

  if (comp_op < EQUAL_TO || comp_op >= NO_OP) {
    LOG_ERROR("Invalid condition with unsupported compare operation: %d", comp_op);
    return RC::INVALID_ARGUMENT;
  }

  left_ = left;
  right_ = right;
  type_left_ = type_left;
  type_right_ = type_right;
  comp_op_ = comp_op;
  return RC::SUCCESS;
}

RC DefaultConditionFilter::init(Table &table, const Condition &condition)
{
  const TableMeta &table_meta = table.table_meta();
  ConDesc left;
  ConDesc right;

  AttrType type_left = UNDEFINED;
  AttrType type_right = UNDEFINED;

  if (condition.leftExpr->type == FIE) {
    left.is_attr = true;
    const FieldMeta *field_left = table_meta.field_and_index(condition.leftExpr->fieldExpression.attr_name, left.attr_i);
    if (nullptr == field_left) {
      LOG_WARN("No such field in condition. %s.%s", table.name(), condition.leftExpr->fieldExpression.attr_name);
      return RC::SCHEMA_FIELD_MISSING;
    }
    left.attr_length = field_left->len();
    left.attr_offset = field_left->offset();
    left.attr_sysnull = table_meta.null_field()->offset();
    left.value = nullptr;

    type_left = field_left->type();
  } else if(condition.leftExpr->type == CONST){
    left.is_attr = false;
    type_left = condition.leftExpr->constExpr.type;
    switch(type_left){
      case INTS:
      case DATES:
        left.value = parse2int(condition.leftExpr->constExpr.data);
        break;
      case FLOATS:
        left.value = parse2float(condition.leftExpr->constExpr.data);
        break ;
      case CHARS:
        left.value = condition.leftExpr->constExpr.data;
        break ;
      case NULLS:
        // no value
        break;
      default:
        return RC::GENERIC_ERROR;
    }

    left.attr_length = 0;
    left.attr_offset = 0;
  } else{
    LOG_ERROR("Not finished");
    return RC::GENERIC_ERROR;
  }

  if (condition.rightExpr->type == FIE) {
    right.is_attr = true;
    const FieldMeta *field_right = table_meta.field_and_index(condition.rightExpr->fieldExpression.attr_name, right.attr_i);
    if (nullptr == field_right) {
      LOG_WARN("No such field in condition. %s.%s", table.name(), condition.rightExpr->fieldExpression.attr_name);
      return RC::SCHEMA_FIELD_MISSING;
    }
    right.attr_length = field_right->len();
    right.attr_offset = field_right->offset();
    right.attr_sysnull = table_meta.null_field()->offset();
    right.value = nullptr;

    type_right = field_right->type();
  } else if(condition.rightExpr->type == CONST){
    right.is_attr = false;
    type_right = condition.rightExpr->constExpr.type;
    switch(type_right){
      case INTS:
      case DATES:
        right.value = parse2int(condition.rightExpr->constExpr.data);
        break;
      case FLOATS:
        right.value = parse2float(condition.rightExpr->constExpr.data);
        break ;
      case CHARS:
        right.value = condition.rightExpr->constExpr.data;
        break ;
      case NULLS:
        break;
      default:
        return RC::GENERIC_ERROR;
    }

    right.attr_length = 0;
    right.attr_offset = 0;
    return RC::SUCCESS;
  } else {
    LOG_ERROR("Not finished");
    return RC::GENERIC_ERROR;
  }

  // 校验和转换
  if (!field_type_compare_compatible_table[type_left][type_right]) {
    // 不能比较的两个字段， 要把信息传给客户端
    return RC::SCHEMA_FIELD_TYPE_MISMATCH;
  }
  // 不同类型的数据比较，比如整数跟浮点数之间的对比
  // 丢到filter里做

  return init(left, right, type_left, type_right, condition.comp);
}

bool DefaultConditionFilter::filter(const Record &rec) const
{
  char *left_value = nullptr;
  char *right_value = nullptr;
  int left_is_null = 0, right_is_null = 0;

  AttrType t1 = type_left_;
  AttrType t2 = type_right_;

  if (left_.is_attr) {  // value
    left_value = (char *)(rec.data() + left_.attr_offset);
    left_is_null = *(int32_t *)(rec.data() + left_.attr_sysnull) & (1 << (31 - left_.attr_i));
    if (left_is_null) t1 = NULLS;
  } else {
    left_value = (char *)left_.value;
  }

  if (right_.is_attr) {
    right_value = (char *)(rec.data() + right_.attr_offset);
    right_is_null = *(int32_t *)(rec.data() + right_.attr_sysnull) & (1 << (31 - right_.attr_i));
    if (right_is_null) t2 = NULLS;
  } else {
    right_value = (char *)right_.value;
  }

  int cmp_result = 0;
  int left_value_size = 4, right_value_size = 4;
  if (t1 == CHARS) left_value_size = strlen((char*)left_value);
  if (t2 == CHARS) right_value_size = strlen((char*)right_value);
  compare_all(t1, t2, left_value, right_value, left_value_size, right_value_size, cmp_result, comp_op_);

  switch (comp_op_) {
    case EQUAL_TO:
    case LIKE:
    case IS:
    case IS_NOT:
    // case IN:
      return 0 == cmp_result;
    case LESS_EQUAL:
      return cmp_result <= 0;
    case NOT_EQUAL:
    case NOT_LIKE:
    // case NOT_IN:
      return cmp_result != 0;
    case LESS_THAN:
      return cmp_result < 0;
    case GREAT_EQUAL:
      return cmp_result >= 0;
    case GREAT_THAN:
      return cmp_result > 0;

    default:
      break;
  }

  LOG_PANIC("Never should print this.");
  return cmp_result;  // should not go here
}

CompositeConditionFilter::~CompositeConditionFilter()
{
  if (memory_owner_) {
    delete[] filters_;
    filters_ = nullptr;
  }
}

RC CompositeConditionFilter::init(const ConditionFilter *filters[], int filter_num, bool own_memory)
{
  filters_ = filters;
  filter_num_ = filter_num;
  memory_owner_ = own_memory;
  return RC::SUCCESS;
}
RC CompositeConditionFilter::init(const ConditionFilter *filters[], int filter_num)
{
  return init(filters, filter_num, false);
}

RC CompositeConditionFilter::init(Table &table, const Condition *conditions, int condition_num)
{
  if (condition_num == 0) {
    return RC::SUCCESS;
  }
  if (conditions == nullptr) {
    return RC::INVALID_ARGUMENT;
  }

  RC rc = RC::SUCCESS;
  ConditionFilter **condition_filters = new ConditionFilter *[condition_num];
  for (int i = 0; i < condition_num; i++) {
    DefaultConditionFilter *default_condition_filter = new DefaultConditionFilter();
    rc = default_condition_filter->init(table, conditions[i]);
    if (rc != RC::SUCCESS) {
      delete default_condition_filter;
      for (int j = i - 1; j >= 0; j--) {
        delete condition_filters[j];
        condition_filters[j] = nullptr;
      }
      delete[] condition_filters;
      condition_filters = nullptr;
      return rc;
    }
    condition_filters[i] = default_condition_filter;
  }
  return init((const ConditionFilter **)condition_filters, condition_num, true);
}

bool CompositeConditionFilter::filter(const Record &rec) const
{
  // 这里只需要AND
  for (int i = 0; i < filter_num_; i++) {
    if (!filters_[i]->filter(rec)) {
      return false;
    }
  }
  return true;
}
