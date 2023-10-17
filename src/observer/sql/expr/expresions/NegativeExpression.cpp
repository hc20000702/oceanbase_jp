//
// Created by zx on 10/17/22.
//

#include "NegativeExpression.h"
#include "IdentifierExpression.h"
#include "typeinfo"
#include "sql/expr/relation/Relation.h"

RC NegativeExpression::mapToRelation(Relation* relation) {
  if(tryToFindColumn(relation)){
    isInput_ = true;
    return SUCCESS;
  }else{
    isInput_ = false;
  }
  isInput_ = false;
  RC rc = expression->mapToRelation(relation);
  return rc;
}

RC NegativeExpression::calculate(Row& row) {
  isNull_ = false;
  if(isInput_){
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
        break ;
      default:
        return GENERIC_ERROR;
    }
    return SUCCESS;
  }
  
  RC rc;
  rc = expression->calculate(row);
  if(rc!=SUCCESS){
    return rc;
  }

  if(expression->isCurrentNull()){
    isNull_ = true;
    return RC::SUCCESS;
  }

  switch (type()) {
    case INTS:{
      intValue = - expression->currentInt();
      break;
    }
    case FLOATS:{
      floatValue = - expression->currentFloat();
      break ;
    }
    default:
      return GENERIC_ERROR;
  }
  return SUCCESS;
}

std::string NegativeExpression::toString() const{
  std::string s;
  if(expression->isConst() || expression->isId()){
    s.append("-(").append(expression->toString()).append(")");
    return s;
  }
  return s.append("-").append(expression->toString());
}

std::string NegativeExpression::displayName(bool needTableName) const
{
  std::string s;
  if(expression->isBracketed()){
    s.append("-(").append(expression->displayName(needTableName)).append(")");
    return s;
  }
  return s.append("-").append(expression->displayName(needTableName));
}

std::string NegativeExpression::displayName(bool flag, std::unordered_map<std::string, std::string>& map) const{
  std::string s;
  if(expression->isBracketed()){
    s.append("-(").append(expression->displayName(flag, map)).append(")");
    return s;
  }
  return s.append("-").append(expression->displayName(flag, map));
}

AttrType NegativeExpression::inferType()
{
  switch(expression->type()){
    case INTS:
      return INTS;
    case FLOATS:
      return FLOATS;
    default:
      return UNDEFINED;
  }
}

AttrType NegativeExpression::type() const
{
  return type_;
}

RC NegativeExpression::get_value(const Tuple &tuple, TupleCell &cell) const{
  RC rc;
  switch(type()){
    case INTS:
      rc = expression->get_value(tuple, cell);
      if(rc != SUCCESS){
        return GENERIC_ERROR;
      }
      cell.set_data(cell.getDataAsInt());
      break ;
    case FLOATS:
      rc = expression->get_value(tuple, cell);
      if(rc != SUCCESS){
        return GENERIC_ERROR;
      }
      cell.set_data(cell.getDataAsFloat());
      break ;
    default:
      return GENERIC_ERROR;
  }
  return SUCCESS;
}

const char* NegativeExpression::fromSingleTable() const{
  return expression->fromSingleTable();
}