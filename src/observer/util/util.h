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
// Created by wangyunlai on 2022/9/28
//

#pragma once

#include <string>
#include "map"
#include "sql/parser/parse_defs.h"
#include "sql/expr/expresions/BaseExpression.h"
#include "storage/common/db.h"
#include "unordered_map"

std::string double2string(double v);
double char2double(const char *str);


BaseExpression* constructExpression(const Expr* expr, Table* defaultTable, std::unordered_map<std::string, Table *> *tables, Db *db);

char* parse2int(const char* literal);

char* parse2float(const char* literal);

std::string date2string(int v);

RC type_cast(Value *value, const FieldMeta *fieldMeta);

RC relation2item(Relation *relation, Item *&item, AttrType &type);
