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
// Created by wangyunlai on 2021/6/11.
//

#include <string.h>
#include <algorithm>
#include "sql/parser/parse_defs.h"
#include "rc.h"
#include "common/log/log.h"
#include "util.h"

const double epsilon = 1E-6;

int compare_int(void *arg1, void *arg2)
{
  int v1 = *(int *)arg1;
  int v2 = *(int *)arg2;
  return v1 - v2;
}

int compare_float(void *arg1, void *arg2)
{
  float v1 = *(float *)arg1; 
  float v2 = *(float *)arg2; 
  float cmp = v1 - v2;
  if (cmp > epsilon) {
    return 1;
  }
  if (cmp < -epsilon) {
    return -1;
  }
  return 0;
}

int compare_string(void *arg1, int arg1_max_length, void *arg2, int arg2_max_length)
{
  const char *s1 = (const char *)arg1;
  const char *s2 = (const char *)arg2;
  int maxlen = std::min(arg1_max_length, arg2_max_length);
  int result =  strncmp(s1, s2, maxlen);
  if (0 != result) {
    return result;
  }

  if (arg1_max_length > maxlen) {
    return s1[maxlen] - 0;
  }

  if (arg2_max_length > maxlen) {
    return 0 - s2[maxlen];
  }
  return 0;
}

bool compare_like(const char *s, const char *p)
{
  int s_len = strlen(s);
  int p_len = strlen(p);
  if (s_len > 0 && p_len == 0) {
    return false;
  }
  int i = 0, j = 0;
  for (; i < s_len; i++) {
    while (j < p_len && p[j] == '%') {
      j++;
    }
    if (j == p_len) {
      if (p[j - 1] == '%') {
        return true;
      }
      while (j > 0 && p[j - 1] != '%') {
        j--;
      }
      if (j == 0) {
        return false;
      }
    }
    if (s[i] == p[j] || p[j] == '_') {
      j++;
      continue;
    }
    while (j >= 0 && p[j] != '%') {
      j--;
    }
    if (j < 0) {
      return false;
    }
  }
  while (j < p_len && p[j] == '%') {
    j++;
  }
  // if (j == p_len && p[j - 1] == '%') {
  //   return true;
  // }
  return j == p_len;
}

RC compare_all(AttrType &t1, AttrType &t2, void *data1, void *data2, int len1, int len2, int &result, const CompOp &comp)
{
  result = 0;
  RC rc = RC::SUCCESS;
  if (comp == IS || comp == IS_NOT || t1 == NULLS || t2 == NULLS) {
    if (t1 == NULLS && t2 == NULLS) {
      result = comp == IS ? 0 : 1;
    } else if (t1 == NULLS || t2 == NULLS) {
      result = comp == IS_NOT ? 0 : 1;
    } else {
      result = 1;
    }
  } else if (comp == LIKE || comp == NOT_LIKE) {
    if (t1 != CHARS || t2 != CHARS) {
      // TODO: compare between char and others
    }
    result = !compare_like((char *)data1, (char *)data2);
  } else if (t1 == t2) {
    switch (t1) {
      case INTS:
      case DATES:
        result = compare_int(data1, data2);
        break;
      case FLOATS:
        result = compare_float(data1, data2);
        break;
      case CHARS:
        result = compare_string(data1, len1, data2, len2);
        break;
      default:
        LOG_WARN("unsupported type: %d", t1);
        rc = RC::RECORD_RECORD_NOT_EXIST;
    }
  } else if (t1 == CHARS && t2 == INTS) {
    float this_data = char2double((char *)data1);
    float other_data = *(int *)data2;
    result = compare_float(&this_data, &other_data);
  } else if (t1 == CHARS && t2 == FLOATS) {
    float this_data = char2double((char *)data1);
    result = compare_float(&this_data, data2);
  } else if (t1 == INTS && t2 == CHARS) {
    float this_data = *(int *)data1;
    float other_data = char2double((char *)data2);
    result = compare_float(&this_data, &other_data);
  } else if (t1 == FLOATS && t2 == CHARS) {
    float other_data = char2double((char *)data2);
    result = compare_float(data1, &other_data);
  } else if (t1 == INTS && t2 == FLOATS) {
    float this_data = *(int *)data1;
    result = compare_float(&this_data, data2);
  } else if (t1 == FLOATS && t2 == INTS) {
    float other_data = *(int *)data2;
    result = compare_float(data1, &other_data);
  } else {
    LOG_WARN("not supported, type: %d, %d", t1, t2);
    rc = RC::RECORD_RECORD_NOT_EXIST;
  }
  return rc;
}
