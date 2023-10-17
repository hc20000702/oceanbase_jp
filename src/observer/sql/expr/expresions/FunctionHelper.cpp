//
// Created by zx on 10/24/22.
//

#include "FunctionHelper.h"

std::map<std::string, Func> funcMap({
  std::make_pair("MAX", MAX),
  std::make_pair("MIN", MIN),
  std::make_pair("SUM", SUM),
  std::make_pair("AVG", AVG),
  std::make_pair("COUNT", COUNT),
  std::make_pair("COUNT_STAR", COUNT_STAR),
  std::make_pair("LENGTH", LENGTH),
  std::make_pair("ROUND", ROUND),
  std::make_pair("DATE_FORMAT", DATE_FORMAT),
});

RC functionFactory(const char* funcName, std::vector<BaseExpression*> paras, BaseExpression*& target){
//  std::map<std::string, Func> funcMap = initFuncMap();
  std::string name = funcName;
  std::transform(name.begin(), name.end(), name.begin(), ::toupper);
  if(funcMap.count(name) == 0){
    return RC::NO_SUCH_FUNC;
  }
  switch (funcMap[name]) {
    case MAX:{
      if(paras.size()!=1){
        return RC::PARA_DISMATCH;
      }
      target = new Max(paras[0]);
      break;
    }
    case MIN:{
      if(paras.size()!=1){
        return RC::PARA_DISMATCH;
      }
      target = new Min(paras[0]);
      break;
    }
    case SUM: {
      if (paras.size() != 1) {
        return RC::PARA_DISMATCH;
      }
      target = new Sum(paras[0]);
      break;
    }
    case AVG: {
      if (paras.size() != 1) {
        return RC::PARA_DISMATCH;
      }
      target = new Avg(paras[0]);
      break;
    }
    case COUNT: {
      if (paras.size() != 1) {
        return RC::PARA_DISMATCH;
      }
      target = new Count(paras[0]);
      break;
    }
    case COUNT_STAR: {
      target = new CountStar;
      break;
    }
    case LENGTH: {
      SimpleFunction* func = new Length;
      RC rc = func->setSubExpressions(paras);
      if(rc!=SUCCESS){return rc;}
      target = func;
      break;
    }
    case ROUND:{
      SimpleFunction* func = new Round;
      RC rc = func->setSubExpressions(paras);
      if(rc!=SUCCESS){return rc;}
      target = func;
      break;
    }
    case DATE_FORMAT:{
      SimpleFunction* func = new DateFormat;
      RC rc = func->setSubExpressions(paras);
      if(rc!=SUCCESS){return rc;}
      target = func;
      break;
    }
    default:
      return RC::NO_SUCH_FUNC;
  }
  return RC::SUCCESS;
}
