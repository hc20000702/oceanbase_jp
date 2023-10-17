#include "OrderOperator.h"
#include "algorithm"

typedef struct {
  int idx;
  AttrType type;
  bool is_asc;
} OrderMeta;
const double epsilon = 1E-6;
static std::vector<OrderMeta> orderMeta_;

bool sortRow(const Row &r1, const Row &r2)
{
  for (auto it : orderMeta_) {
    Item *a = r1.at(it.idx);
    Item *b = r2.at(it.idx);
    if (a->isNull() || b->isNull()) {
      if (!a->isNull()) {
        return it.is_asc ? false : true;
      } else if (!b->isNull()) {
        return it.is_asc ? true : false;
      } else {
        continue;
      }
    } else if (it.type == INTS || it.type == DATES) {
      int aInt = a->getInt();
      int bInt = b->getInt();
      if (aInt == bInt) {
        continue;
      }
      return it.is_asc ? aInt < bInt : aInt > bInt;
    } else if (it.type == FLOATS) {
      float aFloat = a->getFloat();
      float bFloat = b->getFloat();
      if (aFloat - bFloat >= -epsilon && aFloat - bFloat <= epsilon) {
        continue;
      }
      return it.is_asc ? aFloat - bFloat < -epsilon : aFloat - bFloat > epsilon;
    } else {  // CHARS
      int cmp = strcmp(a->getText(), b->getText());
      if (cmp == 0) {
        continue;
      }
      return it.is_asc ? cmp < 0 : cmp > 0;
    }
  }
  return true;
}

RC OrderOperator::evaluate(Relation *&relation)
{
  RC rc;
  Relation *r;
  rc = subOperator->evaluate(r);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  std::vector<Row> &rows = r->getRows();
  orderMeta_.clear();
  for (auto it : orderBy_) {
    for (int i = 0; i < r->getWidth(); i++) {
      if (r->getSchema(i).getExpression()->toString() == it.first->toString()) {
        OrderMeta orderMeta;
        orderMeta.idx = i;
        orderMeta.type = r->getSchema(i).attrType();
        orderMeta.is_asc = it.second;
        orderMeta_.push_back(orderMeta);
        break;
      }
    }
  }
  std::sort(rows.begin(), rows.end(), sortRow);

  relation = r;
  relation->setMultiTable(isMultiTable());
  return rc;
}

RC OrderOperator::close()
{
  return subOperator->close();
};
