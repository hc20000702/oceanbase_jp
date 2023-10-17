//
// Created by zx on 10/17/22.
//

#include "BinaryExpression.h"
#include "math.h"
#include "sql/expr/relation/Item.h"

bool isDigital(AttrType type){
  return type == INTS || type == FLOATS;
}

std::string opToString(ExprOp op){
  switch (op) {
    case OP_ADD:
      return "+";
    case OP_SUB:
      return "-";
    case OP_MUL:
      return "*";
    case OP_DIV:
      return "/";
    case OP_MOD:
      return "%";
    default:
      return "";
  }
}

int evaluate(int left, int right, ExprOp op){
  switch (op) {
    case OP_ADD:
      return left + right;
    case OP_SUB:
      return left - right;
    case OP_MUL:
      return left * right;
    case OP_MOD:
      return left % right;
    default:
      return -1;
  }
}

float evaluate(int left, float right, ExprOp op){
  switch (op) {
    case OP_ADD:
      return left + right;
    case OP_SUB:
      return left - right;
    case OP_MUL:
      return left * right;
    case OP_DIV:
      return left / right;
    default:
      return -1;
  }
}

float evaluate(float left, float right, ExprOp op){
  switch (op) {
    case OP_ADD:
      return left + right;
    case OP_SUB:
      return left - right;
    case OP_MUL:
      return left * right;
    case OP_DIV:
      return left / right;
    default:
      return -1;
  }
}

float evaluate(float left, int right, ExprOp op){
  switch (op) {
    case OP_ADD:
      return left + right;
    case OP_SUB:
      return left - right;
    case OP_MUL:
      return left * right;
    case OP_DIV:
      return left / right;
    default:
      return -1;
  }
}

RC BinaryExpression::mapToRelation(Relation* relation) {
  relation_ = relation;
  if(tryToFindColumn(relation_)){
    isInput_ = true;
    return SUCCESS;
  }else{
    isInput_ = false;
  }

  RC rc;
  rc = leftExpression->mapToRelation(relation);
  if(rc!=SUCCESS) return rc;
  rc = rightExpression->mapToRelation(relation);
  if(rc!=SUCCESS) return rc;
  return SUCCESS;
}

RC BinaryExpression::calculate(Row& row) {
  isNull_ = false;
  if(isInput()){
    if(row.at(column)->isNull()){
      isNull_ = true;
      return RC::SUCCESS;
    }
    switch (type()) {
      case INTS:
        intValue = row.at(column)->getInt();
        break ;
      case FLOATS:
        floatValue = row.at(column)->getFloat();
      default:
        return GENERIC_ERROR;
    }
    return SUCCESS;
  }

  RC rc;
  int leftInt, rightInt;
  float leftFloat, rightFloat;

  rc = leftExpression->calculate(row);
  if(rc != SUCCESS){
    return GENERIC_ERROR;
  }
  rc = rightExpression->calculate(row);
  if(rc != SUCCESS){
    return GENERIC_ERROR;
  }

  if(leftExpression->isCurrentNull() || rightExpression->isCurrentNull()){
    isNull_ = true;
    return RC::SUCCESS;
  }

  leftInt = leftExpression->currentInt();
  rightInt = rightExpression->currentInt();

  leftFloat = leftExpression->currentFloat();
  rightFloat = rightExpression->currentFloat();

  AttrType leftType = leftExpression->type();
  AttrType rightType = rightExpression->type();

  if(leftType == INTS && rightType == INTS){
    if (op == OP_DIV) {
      if(rightInt == 0){
        isNull_ = true;
      }else{
        floatValue = evaluate((float)leftInt, rightInt, op);
      }
    } else {
      intValue = evaluate(leftInt, rightInt, op);
    }
  }else if(leftType == INTS && rightType == FLOATS){
    if(op == OP_DIV){
      if(abs(rightFloat) < 1e-6){
        isNull_ = true;
        return SUCCESS;
      }
    }
    floatValue = evaluate(leftInt, rightFloat, op);
  }else if(leftType == FLOATS && rightType == FLOATS) {
    if(op == OP_DIV){
      if(abs(rightFloat) < 1e-6){
        isNull_ = true;
        return SUCCESS;
      }
    }
    floatValue = evaluate(leftFloat, rightFloat, op);
  }else if(leftType == FLOATS && rightType == INTS){
    if(op == OP_DIV){
      if(rightInt == 0){
        isNull_ = true;
        return SUCCESS;
      }
    }
    floatValue = evaluate(leftFloat, rightInt, op);
  }
  return SUCCESS;
}

std::string BinaryExpression::toString() const
{
  std::string s;

  if(leftExpression->isId() || leftExpression->isConst()){
    s.append(leftExpression->toString());
  }else{
    s.append("(").append(leftExpression->toString()).append(")");
  }

  s.append(opToString(op));

  if(rightExpression->isId() || rightExpression->isConst()){
    s.append(rightExpression->toString());
  }else{
    s.append("(").append(rightExpression->toString()).append(")");
  }

  return s;
}

std::string BinaryExpression::displayName(bool needTableName) const
{
  std::string s;

  if(leftExpression->isBracketed()){
    s.append("(").append(leftExpression->displayName(needTableName)).append(")");
  }else{
    s.append(leftExpression->displayName(needTableName));
  }

  s.append(opToString(op));

  if(rightExpression->isBracketed()){
    s.append("(").append(rightExpression->displayName(needTableName)).append(")");
  }else{
    s.append(rightExpression->displayName(needTableName));
  }

  return s;
}

std::string BinaryExpression::displayName(bool flag, std::unordered_map<std::string, std::string>& map) const{
  std::string s;

  if(leftExpression->isBracketed()){
    s.append("(").append(leftExpression->displayName(flag, map)).append(")");
  }else{
    s.append(leftExpression->displayName(flag, map));
  }

  s.append(opToString(op));

  if(rightExpression->isBracketed()){
    s.append("(").append(rightExpression->displayName(flag, map)).append(")");
  }else{
    s.append(rightExpression->displayName(flag, map));
  }

  return s;
}

AttrType BinaryExpression::type() const{
  return type_;
}

RC BinaryExpression::get_value(const Tuple &tuple, TupleCell &cell) const{
  TupleCell left, right;
  RC rc;
  rc = leftExpression->get_value(tuple, left);
  if(rc != SUCCESS){
    return rc;
  }
  rc = rightExpression->get_value(tuple, right);
  if(rc != SUCCESS){
    return rc;
  }

  int leftInt, rightInt;
  float leftFloat, rightFloat;

  AttrType leftType = leftExpression->type();
  AttrType rightType = rightExpression->type();

  switch (leftType) {
    case INTS:
      leftInt = left.getDataAsInt();
      break;
    case FLOATS:
      leftFloat = left.getDataAsFloat();
      break;
    default:
      return GENERIC_ERROR;
  }

  switch (rightType) {
    case INTS:
      rightInt = right.getDataAsInt();
      break;
    case FLOATS:
      rightFloat = right.getDataAsFloat();
      break;
    default:
      return GENERIC_ERROR;
  }

  if(leftType == INTS && rightType == INTS){
    if (op == OP_DIV) {
      cell.set_data(evaluate((float)leftInt, rightInt, op));
    } else {
      cell.set_data(evaluate(leftInt, rightInt, op));
    }
  }else if(leftType == INTS && rightType == FLOATS){
    cell.set_data(evaluate(leftInt, rightFloat, op));
  }else if(leftType == FLOATS && rightType == FLOATS) {
    cell.set_data(evaluate(leftFloat, rightFloat, op));
  }else if(leftType == FLOATS && rightType == INTS){
    cell.set_data(evaluate(leftFloat, rightInt, op));
  }
  return rc;
}

AttrType BinaryExpression::inferType(){
  if(leftExpression->type() == INTS && rightExpression->type() == INTS){
    if(op == OP_DIV){
      return FLOATS;
    }
    return INTS;
  }
  if(isDigital(leftExpression->type()) && isDigital(rightExpression->type())){
    return FLOATS;
  }
  return UNDEFINED;
}

const char* BinaryExpression::fromSingleTable() const{
  const char* leftTable = leftExpression->fromSingleTable();
  const char* rightTable = rightExpression->fromSingleTable();

  if(leftTable == nullptr || rightTable == nullptr){
    return nullptr;
  }

  if(strcmp(leftTable, rightTable) == 0){
    return leftTable;
  }

  if(strcmp(leftTable, "*") == 0){
    return rightTable;
  }

  if(strcmp(rightTable, "*") == 0){
    return leftTable;
  }

  return nullptr;
}
