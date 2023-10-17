//
// Created by zx on 10/23/22.
//

#include "AggregateFunction.h"
#include "float.h"
#include "util/util.h"

// Attribute
RC Identity::beforeStart() {
  type_ = inferType();
  return SUCCESS;
}
RC Identity::treatRow() {
  RC rc;
  std::string keyString;
  subExpression_->mapToRelation(relation_);
  if(groupBy_.empty()){
    keyString = "non#key#set#";
  }
  for(auto it: groupBy_){
    rc = it->calculate(currentRow);
    if(rc != SUCCESS) return rc;
    if(it->isCurrentNull()){
      return RC::SUCCESS;
    }
    keyString.append(it->toItem()->toString()).append("#");
  }

  rc = subExpression_->calculate(currentRow);
  if(rc != SUCCESS) return rc;

  switch(type()){
    case INTS:
    case DATES:
    case FLOATS:
    case CHARS:{
      if(context.count(keyString) == 0){
        context[keyString] = subExpression_->toItem();
        order.emplace_back(keyString);
      }
      break;
    }
    default:
      return RC::UNKNOWN_TYPE;
  }
  return SUCCESS;
}
RC Identity::afterDealAllRow() {
  return SUCCESS;
}

//=======================================================================================================
// MAX
Max::Max(BaseExpression* subExpression){
  subExpression_ = subExpression;
  type_ = inferType();
}
std::string Max::toString() const {
  std::string maxStr;
  maxStr.append("MAX(").append(subExpression_->toString()).append(")");
  return maxStr;
}
std::string Max::displayName(bool b) const {
  std::string maxStr;
  maxStr.append("MAX(").append(subExpression_->displayName(b)).append(")");
  return maxStr;
}
RC Max::beforeStart(){
  type_ = type();
  return RC::SUCCESS;
}
RC Max::treatRow(){
  RC rc;
  std::string keyString;
  if(groupBy_.empty()){
    keyString = "non#key#set#";
  }
  for(auto it: groupBy_){
    rc = it->calculate(currentRow);
    if(rc != SUCCESS) return rc;
    if(it->isCurrentNull()){
      return RC::SUCCESS;
    }
    keyString.append(it->toItem()->toString()).append("#");
  }

  rc = subExpression_->calculate(currentRow);
  if(rc != SUCCESS) return rc;

  switch(type_){
    case INTS:
    case DATES:{
      if(context.count(keyString) == 0 || context[keyString]->isNull()){
        if(context.count(keyString) == 0){
          order.emplace_back(keyString);
        }
        context[keyString] = subExpression_->toItem();
        break;
      }
      Item* item = subExpression_->toItem();
      if(context[keyString]->getInt() < item->getInt()){
        context[keyString]->release();
        delete context[keyString];
        context[keyString] = item;
      }else{
        item->release();
        delete item;
      }
      break;
    }
    case FLOATS:{
      if(context.count(keyString) == 0 || context[keyString]->isNull()){
        if(context.count(keyString) == 0){
          order.emplace_back(keyString);
        }
        context[keyString] = subExpression_->toItem();
        break;
      }
      Item* item = subExpression_->toItem();
      if(context[keyString]->getFloat() < item->getFloat()){
        context[keyString]->release();
        delete context[keyString];
        context[keyString] = item;
      }else{
        item->release();
        delete item;
      }
      break;
    }
    case CHARS:{
      if(context.count(keyString) == 0 || context[keyString]->isNull()){
        if(context.count(keyString) == 0){
          order.emplace_back(keyString);
        }
        context[keyString] = subExpression_->toItem();
        break;
      }
      Item* item = subExpression_->toItem();
      if(strcmp(context[keyString]->getText(),item->getText()) < 0){
        context[keyString]->release();
        delete context[keyString];
        context[keyString] = item;
      }else{
        item->release();
        delete item;
      }
      break;
    }
    default:
      return RC::ERROR_INPUT_TYPE;
  }
  return RC::SUCCESS;
}
RC Max::afterDealAllRow() {
  return RC::SUCCESS;
}
// ==================================================================================
// MIN
std::string Min::toString() const {
  std::string maxStr;
  maxStr.append("MIN(").append(subExpression_->toString()).append(")");
  return maxStr;
}
std::string Min::displayName(bool b) const {
  std::string maxStr;
  maxStr.append("MIN(").append(subExpression_->displayName(b)).append(")");
  return maxStr;
}
RC Min::beforeStart(){
  type_ = type();
  return RC::SUCCESS;
}
RC Min::treatRow(){
  RC rc;
  std::string keyString;
  if(groupBy_.empty()){
    keyString = "non#key#set#";
  }
  for(auto it: groupBy_){
    rc = it->calculate(currentRow);
    if(rc != SUCCESS) return rc;
    if(it->isCurrentNull()){
      return RC::SUCCESS;
    }
    keyString.append(it->toItem()->toString()).append("#");
  }

  rc = subExpression_->calculate(currentRow);
  if(rc != SUCCESS) return rc;

  switch(type_){
    case INTS:
    case DATES:{
      if(context.count(keyString) == 0 || context[keyString]->isNull()){
        if(context.count(keyString) == 0){
          order.emplace_back(keyString);
        }
        context[keyString] = subExpression_->toItem();
        break;
      }
      Item* item = subExpression_->toItem();
      if(context[keyString]->getInt() > item->getInt()){
        context[keyString]->release();
        delete context[keyString];
        context[keyString] = item;
      }else{
        item->release();
        delete item;
      }
      break;
    }
    case FLOATS:{
      if(context.count(keyString) == 0 || context[keyString]->isNull()){
        if(context.count(keyString) == 0){
          order.emplace_back(keyString);
        }
        context[keyString] = subExpression_->toItem();
        break;
      }
      Item* item = subExpression_->toItem();
      if(context[keyString]->getFloat() > item->getFloat()){
        context[keyString]->release();
        delete context[keyString];
        context[keyString] = item;
      }else{
        item->release();
        delete item;
      }
      break;
    }
    case CHARS:{
      if(context.count(keyString) == 0 || context[keyString]->isNull()){
        if(context.count(keyString) == 0){
          order.emplace_back(keyString);
        }
        context[keyString] = subExpression_->toItem();
        break;
      }
      Item* item = subExpression_->toItem();
      if(strcmp(context[keyString]->getText(),item->getText()) > 0){
        context[keyString]->release();
        delete context[keyString];
        context[keyString] = item;
      }else{
        item->release();
        delete item;
      }
      break;
    }
    default:
      return RC::ERROR_INPUT_TYPE;
  }
  return RC::SUCCESS;
}
RC Min::afterDealAllRow() {
  return RC::SUCCESS;
}

//====================================================================================================
// AVG
RC Avg::beforeStart() {
  type_ = FLOATS;
  subExprType = subExpression_->type();
  return RC::SUCCESS;
}
RC Avg::treatRow() {
  RC rc;
  std::string keyString;
  if(groupBy_.empty()){
    keyString = "non#key#set#";
  }
  for(auto it: groupBy_){
    rc = it->calculate(currentRow);
    if(rc != SUCCESS) return rc;
    if(it->isCurrentNull()){
      return RC::SUCCESS;
    }
    keyString.append(it->toItem()->toString()).append("#");
  }

  rc = subExpression_->calculate(currentRow);
  if(rc != SUCCESS) return rc;

  switch (subExprType) {
    case INTS:
      if(sum.count(keyString) == 0){
        order.emplace_back(keyString);
        if(!subExpression_->isCurrentNull()){
          sum[keyString] = (float) subExpression_->currentInt();
          count[keyString] = 1;
        }else{
          sum[keyString] = 0;
          count[keyString] = 0;
        }
        break;
      }

      if(!subExpression_->isCurrentNull()){
        sum[keyString] += (float) subExpression_->currentInt();
        count[keyString] ++;
      }
      break;
    case FLOATS:
      if(sum.count(keyString) == 0){
        order.emplace_back(keyString);
        if(!subExpression_->isCurrentNull()){
          sum[keyString] = subExpression_->currentFloat();
          count[keyString] = 1;
        }else{
          sum[keyString] = 0;
          count[keyString] = 0;
        }
        break;
      }

      if(!subExpression_->isCurrentNull()){
        sum[keyString] += subExpression_->currentFloat();
        count[keyString] ++;
      }
      break;
    case CHARS:
      if(sum.count(keyString) == 0){
        order.emplace_back(keyString);
        if(!subExpression_->isCurrentNull()){
          sum[keyString] = (float) char2double(subExpression_->currentText());
          count[keyString] = 1;
        }else{
          sum[keyString] = 0;
          count[keyString] = 0;
        }
        break;
      }

      if(!subExpression_->isCurrentNull()){
        sum[keyString] += (float) char2double(subExpression_->currentText());
        count[keyString] ++;
      }
      break;
    default:
      return RC::ERROR_INPUT_TYPE;
  }

  return RC::SUCCESS;
}
RC Avg::afterDealAllRow() {
  for(const auto& it: order){
    if(count[it] == 0){
      context[it] = new NullItem();
    }else{
      context[it] = new FloatItem(sum[it] / count[it], false);
    }
  }
  return RC::SUCCESS;
}

// =============================================================================================
// SUM
RC Sum::beforeStart() {
  subExprType = subExpression_->type();
  if(subExprType == CHARS){
    type_ = FLOATS;
  }else{
    type_ = subExprType;
  }
  return RC::SUCCESS;
}
RC Sum::treatRow() {
  RC rc;
  std::string keyString;
  if(groupBy_.empty()){
    keyString = "non#key#set#";
  }
  for(auto it: groupBy_){
    rc = it->calculate(currentRow);
    if(rc != SUCCESS) return rc;
    if(it->isCurrentNull()){
      return RC::SUCCESS;
    }
    keyString.append(it->toItem()->toString()).append("#");
  }

  rc = subExpression_->calculate(currentRow);
  if(rc != SUCCESS) return rc;

  switch (subExprType) {
    case INTS:
      if(sumInt.count(keyString) == 0){
        order.emplace_back(keyString);
        if(!subExpression_->isCurrentNull()){
          sumInt[keyString] = subExpression_->currentInt();
          count[keyString] = 1;
        }else{
          sumInt[keyString] = 0;
          count[keyString] = 0;
        }
        break;
      }

      if(!subExpression_->isCurrentNull()){
        sumInt[keyString] += subExpression_->currentInt();
        count[keyString] ++;
      }
      break;
    case FLOATS:
      if(sumFloat.count(keyString) == 0){
        order.emplace_back(keyString);
        if(!subExpression_->isCurrentNull()){
          sumFloat[keyString] = subExpression_->currentFloat();
          count[keyString] = 1;
        }else{
          sumFloat[keyString] = 0;
          count[keyString] = 0;
        }
        break;
      }

      if(!subExpression_->isCurrentNull()){
        sumFloat[keyString] += subExpression_->currentFloat();
        count[keyString] ++;
      }
      break;
    case CHARS:
      if(sumFloat.count(keyString) == 0){
        order.emplace_back(keyString);
        if(!subExpression_->isCurrentNull()){
          sumFloat[keyString] = (float) char2double(subExpression_->currentText());
          count[keyString] = 1;
        }else{
          sumFloat[keyString] = 0;
          count[keyString] = 0;
        }
        break;
      }

      if(!subExpression_->isCurrentNull()){
        sumFloat[keyString] += (float) char2double(subExpression_->currentText());
        count[keyString] ++;
      }
      break;
    default:
      return RC::ERROR_INPUT_TYPE;
  }

  return RC::SUCCESS;
}
RC Sum::afterDealAllRow() {
  switch(type_){
    case INTS:
      for(const auto& it: order){
        if(count[it] == 0){
          context[it] = new NullItem();
        }else{
          context[it] = new IntItem(sumInt[it], false);
        }
      }
      break;
    case FLOATS:
      for(const auto& it: order){
        if(count[it] == 0){
          context[it] = new NullItem();
        }else{
          context[it] = new FloatItem(sumFloat[it], false);
        }
      }
      break;
    default:
      return RC::ERROR_INPUT_TYPE;
  }
  return RC::SUCCESS;
}

//============================================================================
// COUNT
RC Count::beforeStart() {
  type_ = INTS;
  return RC::SUCCESS;
}

RC Count::treatRow() {
  RC rc;
  std::string keyString;
  if(groupBy_.empty()){
    keyString = "non#key#set#";
  }
  for(auto it: groupBy_){
    rc = it->calculate(currentRow);
    if(rc != SUCCESS) return rc;
    if(it->isCurrentNull()){
      return RC::SUCCESS;
    }
    keyString.append(it->toItem()->toString()).append("#");
  }

  rc = subExpression_->calculate(currentRow);
  if(rc != SUCCESS) return rc;

  if(count.count(keyString) == 0){
    order.emplace_back(keyString);
    if(!subExpression_->isCurrentNull()){
      count[keyString] = 1;
    }else{
      count[keyString] = 0;
    }
    return RC::SUCCESS;
  }

  if(!subExpression_->isCurrentNull()){
    count[keyString] ++;
  }

  return RC::SUCCESS;
}
RC Count::afterDealAllRow() {
  for(const auto& it: order){
    context[it] = new IntItem(count[it], false);
  }
  return SUCCESS;
}

// ===================================================================================
// COUNT_STAR