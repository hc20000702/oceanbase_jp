//
// Created by zx on 10/17/22.
//

#include "ConstExpression.h"

RC ConstExpression::mapToRelation(Relation* relation){
    return SUCCESS;
}

RC ConstExpression::calculate(Row& row){
  return status;
}

std::string ConstExpression::toString() const{
    return literal_;
}

AttrType ConstExpression::type() const{
  return attrType;
}

void ConstExpression::parse(){
    status = RC::SUCCESS;
    switch(attrType){
      case INTS:
      case DATES:
        try{
            intValue = atoi(literal_);
        }catch(...){
          status = RC::CONST_PARSE_ERROR;
        }
        break;
      case FLOATS:
        try{
          floatValue = atof(literal_);
        }catch(...){
          status = RC::CONST_PARSE_ERROR;
        }
        break;
      case CHARS:
        break;
      case NULLS:
        isNull_ = true;
        break;
      default:
        status = RC::CONST_PARSE_ERROR;
    }
}

RC ConstExpression::get_value(const Tuple &tuple, TupleCell &cell) const
{
  switch(type()){
    case INTS:
      cell.set_data(intValue);

      break ;
    case DATES:
      cell.set_data(intValue);
      cell.set_type(DATES);
      break;
    case FLOATS:
      cell.set_data(floatValue);
      break ;
    case CHARS:
      cell.set_data(literal_);
      cell.set_type(CHARS);
      cell.set_length(strlen(literal_)); // used in comparing
      break ;
    case NULLS:
      cell.set_type(NULLS);
      cell.set_null(true);
      break;
    default:
      return GENERIC_ERROR;
  }
  return SUCCESS;
}

const char* ConstExpression::fromSingleTable() const{
  return "*";
}