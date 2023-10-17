//
// Created by zx on 10/19/22.
//

#include "BaseExpression.h"
#include "sql/expr/relation/Relation.h"
#include "sql/expr/expresions/RelationExpression.h"

bool compare_like(const char *s, const char *t);
double char2double(const char *str);
RC relation2item(Relation *relation, Item *&item, AttrType &type);

bool checkTypeMatch(AttrType left, AttrType right){
  if(left == UNDEFINED || right == UNDEFINED){
    return false;
  } else if (left == NULLS || right == NULLS) {
    return true;
  } else if (left == DATES && right != DATES) {
    return false;
  } else if (left != DATES && right == DATES) {
    return false;
  }
  return true;
}

bool textComp(const char* a, const char* b, CompOp op){
  switch (op) {
    case EQUAL_TO: {
      return strcmp(a, b) == 0;
    }
    case LESS_EQUAL: {
      return strcmp(a, b) <= 0;
    }
    case NOT_EQUAL: {
      return strcmp(a, b) != 0;
    }
    case LESS_THAN: {
      return strcmp(a, b) < 0;
    }
    case GREAT_EQUAL: {
      return strcmp(a, b) >= 0;
    }
    case GREAT_THAN: {
      return strcmp(a, b) > 0;
    }
    case LIKE: {
      return compare_like(a, b);
    }
    case NOT_LIKE: {
      return !compare_like(a, b);
    }
    default: {
      return false;
    }
  }
}

template <class T1, class T2>
bool numberComp(T1 a, T2 b, CompOp op){
  switch (op) {
    case EQUAL_TO: {
      return a == b;
    }
    case LESS_EQUAL: {
      return a <= b;
    }
    case NOT_EQUAL: {
      return a != b;
    }
    case LESS_THAN: {
      return a < b;
    }
    case GREAT_EQUAL: {
      return a >= b;
    }
    case GREAT_THAN: {
      return a > b;
    }
    default: {
      return false;
    }
  }
}
bool floatComp(float a, float b, CompOp op){
  // refer to util/comparator
  const double epsilon = 1E-6;
  switch (op) {
    case EQUAL_TO: {
      return a - b > -epsilon && a - b < epsilon;
    }
    case LESS_EQUAL: {
      return a - b <= epsilon;
    }
    case NOT_EQUAL: {
      return a - b < -epsilon || a - b > epsilon;
    }
    case LESS_THAN: {
      return a - b < -epsilon;
    }
    case GREAT_EQUAL: {
      return a - b >= -epsilon;
    }
    case GREAT_THAN: {
      return a - b > epsilon;
    }
    default: {
      return false;
    }
  }
}

bool itemComp(AttrType &t1, AttrType &t2, Item *i1, Item *i2, const CompOp &op)
{
  bool result = false;
  if (op == IS || op == IS_NOT || t1 == NULLS || t2 == NULLS) {
    if (t1 == NULLS && t2 == NULLS) {
      result = op == IS ? true : false;
    } else {
      result = op == IS_NOT ? true : false;
    }
  } else if (t1 == CHARS && t2 == CHARS) {
    result = textComp(i1->getText(), i2->getText(), op);
  } else if (t1 == CHARS && t2 == INTS) {
    result = floatComp((float)char2double(i1->getText()), float(i2->getInt()), op);
  } else if (t1 == CHARS && t2 == FLOATS) {
    result = floatComp((float)char2double(i1->getText()), i2->getFloat(), op);
  } else if (t1 == INTS && t2 == CHARS) {
    result = floatComp(float(i1->getInt()), (float)char2double(i2->getText()), op);
  } else if (t1 == FLOATS && t2 == CHARS) {
    result = floatComp(i1->getFloat(), (float)char2double(i2->getText()), op);
  } else if((t1 == INTS && t2 == INTS) || (t1 == DATES && t2 == DATES)) {
    result = numberComp(i1->getInt(), i2->getInt(), op);
  } else if (t1 == INTS && t2 == FLOATS){
    result = floatComp(float(i1->getInt()), i2->getFloat(), op);
  } else if (t1 == FLOATS && t2 == INTS){
    result = floatComp(i1->getFloat(), float(i2->getInt()), op);
  } else if (t1 == FLOATS && t2 == FLOATS){
    result = floatComp(i1->getFloat(), i2->getFloat(), op);
  } else {
    LOG_ERROR("any else type excluded: %d, %d", t1, t2);
  }
  return result;
}

// IN: left equal to any element of relation
// NOT IN: left not equal to every element of relation
// return : if IN / NOT IN, return true
RC inComp(Item* left, AttrType typeL, Relation *relation, const CompOp &op, bool isColumn, bool & flag)
{
  if(isColumn){
    if(relation->getWidth() > 1) return MISMATCH;
    if(relation->getWidth() == 0) {
      flag = op != IN;
      return RC::SUCCESS;
    }
    AttrType typeR = relation->getSchema(0).attrType();
    for (int i = 0; i < relation->getLength(); i++) {
      Item *right = relation->getItem(i, 0);
      if (right->isNull()) {
        typeR = NULLS;
      }
      bool result = op == IN ? itemComp(typeL, typeR, left, right, EQUAL_TO)
                             : itemComp(typeL, typeR, left, right, NOT_EQUAL);
      if (op == IN) {
        if (result) {
          flag = true;
          return RC::SUCCESS;
        }
      } else {
        if (!result) {
          flag = false;
          return RC::SUCCESS;
        }
      }
    }
  }else{
    if(relation->getLength() > 1) return MISMATCH;
    if(relation->getLength() == 0) {
      flag = op != IN;
      return RC::SUCCESS;
    }
    for (int j = 0; j < relation->getWidth(); j++){
      AttrType typeR = relation->getSchema(j).attrType();
      Item *right = relation->getItem(0, j);
      if (right->isNull()) {
        typeR = NULLS;
      }
      bool result = op == IN ? itemComp(typeL, typeR, left, right, EQUAL_TO)
                             : itemComp(typeL, typeR, left, right, NOT_EQUAL);
      if (op == IN) {
        if (result) {
          flag = true;
          return RC::SUCCESS;
        }
      } else {
        if (!result) {
          flag = false;
          return RC::SUCCESS;
        }
      }
    }
  }
  flag = op != IN ;
  return RC::SUCCESS;
}

RC BaseExpression::compare(BaseExpression* left, BaseExpression* right, Row& row, CompOp op, bool &result){
  // return true if matched
  result = false; // NULL 的结果是false

  RC rc;
  // exists 左边是空
  if (left) {
    rc = left->calculate(row);
    if(rc != RC::SUCCESS) return rc;
  }
  rc = right->calculate(row);
  if(rc != RC::SUCCESS) return rc;

  AttrType typeL;
  if (!left) {
    typeL = RELATIONS;
  } else if (left->isCurrentNull()) {
    typeL = NULLS;
  } else {
    typeL = left->type();
  }
  AttrType typeR = right->isCurrentNull() ? NULLS : right->type();
  if(!checkTypeMatch(typeL, typeR)) return RC::MISMATCH;

  if (typeL == NULLS || typeR == NULLS) {
    if (typeL == NULLS && typeR == NULLS) {
      result = op == IS ? true : false;
    } else {
      result = op == IS_NOT ? true : false;
    }
  }
  // TODO: compare between char and others
  else if (typeL == CHARS && typeR == CHARS) {
    result = textComp(left->currentText(), right->currentText(), op);
  } else if (typeL == CHARS && typeR == INTS) {
    result = floatComp((float)char2double(left->currentText()), float(right->currentInt()), op);
  } else if (typeL == CHARS && typeR == FLOATS) {
    result = floatComp((float)char2double(left->currentText()), right->currentFloat(), op);
  } else if (typeL == INTS && typeR == CHARS) {
    result = floatComp(float(left->currentInt()), (float)char2double(right->currentText()), op);
  } else if (typeL == FLOATS && typeR == CHARS) {
    result = floatComp(left->currentFloat(), (float)char2double(right->currentText()), op);
  } else if((typeL == INTS && typeR == INTS) || (typeL == DATES && typeR == DATES)) {
    result = numberComp(left->currentInt(), right->currentInt(), op);
  } else if (typeL == INTS && typeR == FLOATS){
    result = floatComp(float(left->currentInt()), right->currentFloat(), op);
  } else if (typeL == FLOATS && typeR == INTS){
    result = floatComp(left->currentFloat(), float(right->currentInt()), op);
  } else if (typeL == FLOATS && typeR == FLOATS){
    result = floatComp(left->currentFloat(), right->currentFloat(), op);
  } else if (typeL == RELATIONS || typeR == RELATIONS) {
    if (op == EXISTS || op == NOT_EXISTS) {
      if (typeR != RELATIONS) {
        rc = RC::ERROR_INPUT_TYPE;
      } else {
        rc = ((RelationExpression*) right)->update();
        if(rc!=SUCCESS) return rc;
        Relation* relation = ((RelationExpression *)right)->relation();
        if (relation->getLength() == 0 || relation->getWidth() == 0) {
          result = op != EXISTS;
        }
        else {
          result = op == EXISTS;
        }
      }
    } else if (op == IN || op == NOT_IN) { // in 左边只能有1个值或者空，右边必须是relation
      if (typeR != RELATIONS) {
        rc = RC::ERROR_INPUT_TYPE;
      } else if (typeL == RELATIONS) {
        Item *item_left;
        AttrType type_left;
        if(left->isRelation()){
          rc = ((RelationExpression*) left)->update();
          if(rc!=SUCCESS) return rc;
        }
        rc = ((RelationExpression*) right)->update();
        if(rc!=SUCCESS) return rc;
        if(relation2item(((RelationExpression *)left)->relation(), item_left, type_left) != RC::SUCCESS) {
          return RC::ERROR_INPUT_TYPE;
        }
        rc = inComp(item_left, type_left, ((RelationExpression *)right)->relation(), op, ((RelationExpression *)right)->needRecalculate(), result);
        if(rc!=SUCCESS) return rc;
      } else {
        rc = ((RelationExpression*) right)->update();
        if(rc!=SUCCESS) return rc;
        rc = inComp(left->toItem(), typeL,  ((RelationExpression *)right)->relation(), op, ((RelationExpression *)right)->needRecalculate(), result);
        if(rc!=SUCCESS) return rc;
      }
    } else { // 其他比较符只能单值
      Item *item_left, *item_right;
      AttrType type_left, type_right;
      if (typeL == RELATIONS) {
        rc = ((RelationExpression*) left)->update();
        if(rc!=SUCCESS) return rc;
        if(relation2item(((RelationExpression *)left)->relation(), item_left, type_left) != RC::SUCCESS) {
          return RC::ERROR_INPUT_TYPE;
        }
      } else {
        item_left = left->toItem();
        type_left = typeL;
      }
      if (typeR == RELATIONS) {
        rc = ((RelationExpression*) right)->update();
        if(rc!=SUCCESS) return rc;
        if(relation2item(((RelationExpression *)right)->relation(), item_right, type_right) != RC::SUCCESS) {
          return RC::ERROR_INPUT_TYPE;
        }
      } else {
        item_right = right->toItem();
        type_right = typeR;
      }
      result = itemComp(type_left, type_right, item_left, item_right, op);
    }
  } else {
    LOG_ERROR("any else type excluded: %d, %d", typeL, typeR);
    rc = RC::UNKNOWN_TYPE;
  }
  return rc;
}

Item* BaseExpression::toItem()
{
  if(isCurrentNull()){
    return new NullItem;
  }
  switch (type()) {
    case INTS:
      return new IntItem(currentInt(), false);
    case DATES:
      return new DateItem(currentInt(), false);
    case FLOATS:
      return new FloatItem(currentFloat(), false);
    case CHARS:
      return new TextItem(currentText(), false);
    default:
      return nullptr;
  }
}

bool BaseExpression::tryToFindColumn(Relation *relation_) {
  column =relation_->findExpressionColumn(this);
  return column != -1;
}