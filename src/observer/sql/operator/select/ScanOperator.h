//
// Created by zx on 10/19/22.
//

#ifndef MINIDB_SCANOPERATOR_H
#define MINIDB_SCANOPERATOR_H

#include "sql/operator/QueryOperator.h"
#include "unordered_map"

class ScanOperator: public QueryOperator{
public :
  ScanOperator(Operator* scan, std::vector<Schema> schema, std::unordered_map<const Table*, Relation*>* relations)
    : scanOperator_(scan), schema_(schema), relations_(relations){}

  RC evaluate(Relation *&) override;

  RC close() override;
  void setSchemaOfFather(std::vector<Schema> schemaOfFather) override{
    return;
  }

  void setRowFromFather(Row rowFromFather) {
    return;
  }
private:
  // table_scan_operator 或者 index_scan_operator
  Operator* scanOperator_;
  std::vector<Schema> schema_;
  std::unordered_map<const Table*, Relation*> *relations_;
  Relation* relation_ = nullptr;
};

#endif  // MINIDB_SCANOPERATOR_H
