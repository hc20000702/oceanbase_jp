//
// Created by zx on 10/24/22.
//

#include "SimpleFunction.h"
#include "math.h"

RC SimpleFunction::get_value(const Tuple &tuple, TupleCell &cell) const {
  return RC::UNIMPLENMENT;
}

// Length
RC Length::setSubExpressions(std::vector<BaseExpression*> expressions) {
  if(expressions.size()!=1){
    return RC::MISMATCH;
  }
  expression_ = expressions[0];
  if(expression_->type()!=CHARS){
    return RC::MISMATCH;
  }
  return RC::SUCCESS;
}

RC Length::mapToRelation(Relation* relation) {
  if(tryToFindColumn(relation)){
    return RC::SUCCESS;
  }
  return expression_->mapToRelation(relation);
}

RC Length::calculate(Row& row) {
  isNull_ = false;
  if(column!=-1){
    if(row.at(column)->isNull()){
      isNull_ = true;
      return RC::SUCCESS;
    }
    intValue = row.at(column)->getInt();
    return RC::SUCCESS;
  }

  RC rc;
  rc = expression_->calculate(row);
  if(rc!=RC::SUCCESS) return rc;

  if(expression_->isCurrentNull()){
    isNull_ = true;
    return RC::SUCCESS;
  }

  intValue = strlen(expression_->currentText());
  return RC::SUCCESS;
}

std::string Length::displayName(bool flag) const {
  return "LENGTH("+expression_->displayName(flag) + ")";
}

AttrType Length::type() const {
  return INTS;
}

RC Length::get_value(const Tuple &tuple, TupleCell &cell) const {
  return RC::UNIMPLENMENT;
}

const char * Length::fromSingleTable() const {
  return expression_->fromSingleTable();
}

std::string Length::toString() const{
  return "LENGTH("+ expression_->toString()+")";
}

std::string Length::displayName(bool flag, std::unordered_map<std::string, std::string>& map) const{
  return "LENGTH("+ expression_->displayName(flag, map)+")";
}

// ========================================================================
// ROUND(float, int): round(1.25,1) = 1.3
const int Round::defaultInt = 0;

RC Round::setSubExpressions(std::vector<BaseExpression*> expressions) {
  if(expressions.size()==1){
    int_ = nullptr;
    float_ = expressions[0];
    if(float_->type()!=FLOATS && float_->type()!=INTS){
      return RC::MISMATCH;
    }
  }else if(expressions.size()==2){
    float_ = expressions[1];
    int_ = expressions[0];
    if((float_->type()!=FLOATS && float_->type()!=INTS) || int_->type()!=INTS){
      return RC::MISMATCH;
    }
  } else {
    return RC::MISMATCH;
  }
  return RC::SUCCESS;
}

RC Round::mapToRelation(Relation* relation) {
  if(tryToFindColumn(relation)){
    return RC::SUCCESS;
  }

  RC rc;
  rc = float_->mapToRelation(relation);
  if(rc!=SUCCESS) return rc;

  if(int_){
    rc = int_->mapToRelation(relation);
  }
  return rc;
}

RC Round::calculate(Row& row) {
  isNull_ = false;
  if(column!=-1){
    if(row.at(column)->isNull()){
      isNull_ = true;
      return RC::SUCCESS;
    }
    floatValue = row.at(column)->getFloat();
    return RC::SUCCESS;
  }

  RC rc;
  rc = float_->calculate(row);
  if(rc!=SUCCESS) return rc;
  if(float_->isCurrentNull()){
    isNull_ = true;
    return RC::SUCCESS;
  }
  float f;
  if(float_->type() == FLOATS) f = float_->currentFloat();
  else if(float_->type() == INTS) f = float_->currentInt();
  else return RC::ERROR_INPUT_TYPE;

  int i;
  if(int_!= nullptr){
    rc = int_->calculate(row);
    if(rc!=SUCCESS) return rc;
    if(int_->isCurrentNull()) {
      isNull_ = true;
      return RC::SUCCESS;
    }
    i = int_->currentInt();
  }else{
    i = defaultInt;
  }

  if(i<0){
    isNull_ = true;
    return RC::SUCCESS;
  }
  floatValue = round(f*pow(10,i) + 1E-2)/pow(10,i) + 1E-5;
  return RC::SUCCESS;
}

std::string Round::displayName(bool flag) const{
  if(int_== nullptr){
    return "ROUND(" + float_->displayName(flag)+")";
  }else{
    return "ROUND(" + float_->displayName(flag)+","+int_->displayName(flag)+")";
  }
}

AttrType Round::type() const {
  return FLOATS;
}

const char * Round::fromSingleTable() const {
  if(int_== nullptr){
    return float_->fromSingleTable();
  }

  const char * left = float_->fromSingleTable();
  const char * right = int_->fromSingleTable();
  if(left == nullptr || right== nullptr) return nullptr;
  if(0==strcmp("*", left)) return right;
  if(0==strcmp("*", right)) return left;

  if(0== strcmp(left, right)){
    return left;
  }else{
    return nullptr;
  }
}

std::string Round::toString() const {
  if(int_){
    return "ROUND("+float_->toString()+","+int_->toString()+")";
  }else{
    return "ROUND("+float_->toString()+")";
  }
}

std::string Round::displayName(bool flag, std::unordered_map<std::string, std::string> &map) const {
  if(int_== nullptr){
    return "ROUND(" + float_->displayName(flag, map)+")";
  }else{
    return "ROUND(" + float_->displayName(flag, map)+","+int_->displayName(flag, map)+")";
  }
}

// =============================================================================================
// DATE_FORMAT
const std::vector<std::string> DateFormat::monthMap({
  "January",
  "February",
  "March",
  "April",
  "May",
  "June",
  "July",
  "August",
  "September",
  "October",
  "November",
  "December"
});

const std::vector<std::string> DateFormat::dayMap({
  "1st","2nd","3rd","4th","5th","6th","7th","8th","9th","10th",
  "11st","12nd","13rd","14th","15th","16th","17th","18th","19th","20th",
  "21st","22nd","23rd","24th","25th","26th","27th","28th","29th","30th",
  "31st"
});

std::string intToString(int i, int length){
  int len = 0, copyi = i;
  while(copyi!=0){
    len ++;
    copyi/=10;
  }

  if(len > length){
    int pow = 1;
    for(int j = 0; j < len - length; ++j){
      pow*=10;
    }
    i %= pow;
  }

  std::string s;
  for(int j = len; j < length; ++j){
    s.push_back('0');
  }
  s.append(std::to_string(i));
  return s;
}

RC DateFormat::setSubExpressions(std::vector<BaseExpression*> expressions) {
  std::cout<<"set subexpression"<<std::endl;
  if(expressions.size()!=2){
    return RC::MISMATCH;
  }
  date = expressions[1];
  format = expressions[0];
  if(date->type()!=DATES || format->type()!=CHARS){
    return RC::MISMATCH;
  }
  return RC::SUCCESS;
}
RC DateFormat::mapToRelation(Relation* relation) {
  if(tryToFindColumn(relation)){
    return RC::SUCCESS;
  }

  RC rc;
  rc = date->mapToRelation(relation);
  if(rc!=SUCCESS) return rc;
  rc = format->mapToRelation(relation);
  if(rc!=SUCCESS) return rc;
  return RC::SUCCESS;
}

RC DateFormat::calculate(Row& row) {
  isNull_ = false;
  if(column!=-1){
    if(row.at(column)->isNull()){
      isNull_ = true;
      return RC::SUCCESS;
    }
    textValue = row.at(column)->getText();
    return RC::SUCCESS;
  }

  RC rc;
  rc = date->calculate(row);
  if(rc!=SUCCESS) return rc;
  rc = format->calculate(row);
  if(rc!=SUCCESS) return rc;

  if(date->isCurrentNull() || format->isCurrentNull()){
    isNull_ = true;
    return RC::SUCCESS;
  }

  int dateAsInt = date->currentInt();
  const char* formatString = format->currentText();

  int day = dateAsInt % 100;
  dateAsInt /= 100;
  int month = dateAsInt % 100;
  int year = dateAsInt / 100;

  std::string dateFormatString;
  for(const char* head = formatString; *head!=0; ++head ){
    if(*head!='%'){
      dateFormatString.push_back(*head);
    }else{
      head++;
      switch (*head) {
        case 'Y':
          dateFormatString.append(intToString(year, 4));
          break;
        case 'y':
          dateFormatString.append(intToString(year, 2));
          break;
        case 'M':
          if(month>12 || month < 1){
            // 不该出现此情况
            isNull_ = true;
            return RC::SUCCESS;
          }
          dateFormatString.append(monthMap[month - 1]);
          break;
        case 'm':
          dateFormatString.append(intToString(month, 2));
          break;
        case 'D':
          if(day > 31 || day < 1){
            // 不该出现此情况
            isNull_ = true;
            return RC::SUCCESS;
          }
          dateFormatString.append(dayMap[day - 1]);
          break;
        case 'd':
          dateFormatString.append(intToString(day, 2));
          break;
        default:
          dateFormatString += *head;
          break;
      }
    }
  }

  textValue = strdup(dateFormatString.c_str());

  return RC::SUCCESS;
}

std::string DateFormat::displayName(bool flag) const {
  return "DATE_FORMAT("+date->displayName(flag)+",'"+format->toString()+"')";
}
AttrType DateFormat::type() const {
  return CHARS;
}
const char * DateFormat::fromSingleTable() const {
//  can just return true;
  return date->fromSingleTable();
}
std::string DateFormat::toString() const {
  return "DATE_FORMAT("+date->toString()+",'"+format->toString()+"')";
}

std::string DateFormat::displayName(bool flag, std::unordered_map<std::string, std::string> &map) const {
  return "DATE_FORMAT("+date->displayName(flag, map)+",'"+format->toString()+"')";
}
