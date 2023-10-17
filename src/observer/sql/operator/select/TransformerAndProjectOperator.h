//
// Created by zx on 10/20/22.
//

#ifndef MINIDB_TRANSFORMERANDPROJECTOPERATOR_H
#define MINIDB_TRANSFORMERANDPROJECTOPERATOR_H

#include "sql/operator/QueryOperator.h"

class TransformerAndProjectOperator: public QueryOperator{
public:
  TransformerAndProjectOperator(QueryOperator* subOperator, std::vector<BaseExpression*> exprs)
      : subOperator(subOperator), expressions(exprs){}

  RC evaluate(Relation * &) override;

  RC close() override;

  void setSchemaOfFather(std::vector<Schema> schemaOfFather) override{
    subOperator->setSchemaOfFather(schemaOfFather);
    schemaOfFather_ = schemaOfFather;
  }

  void setRowFromFather(Row rowFromFather) override{
    subOperator->setRowFromFather(rowFromFather);
    rowFromFather_ = rowFromFather;
  }

private:
  QueryOperator* subOperator;
  std::vector<BaseExpression*> expressions;

  RC constructRow(std::vector<Row>::iterator & it, Row& row);
};

#endif  // MINIDB_TRANSFORMERANDPROJECTOPERATOR_H
