//
// Created by zx on 10/17/22.
//

#include "Item.h"
#include "string.h"

// TextItem::TextItem(const char* value): Item(false) {
//   value_ = strdup(value);
// }

TextItem::TextItem(const char* value, bool isNull): Item(isNull) {
  value_ = strdup(value);
}