//
// Created by zx on 10/17/22.
//

#ifndef MINIDB_ITEM_H
#define MINIDB_ITEM_H

#include "iostream"
#include "float.h"
#include "math.h"
#include "string.h"

class not_support:public std::logic_error    //从logic_error派生自定义异常类
{
public:
    not_support():std::logic_error("method not supported"){}
    virtual ~not_support() throw(){}
};


std::string date2string(int v);
class Item {
public:
  Item(bool isNull): isNull_(isNull) {}
  virtual int getInt() { throw not_support(); }
  virtual ~Item(){}
  virtual float getFloat() { throw not_support(); }
  virtual bool getBool() { throw not_support(); }
  virtual char* getText() { throw not_support(); }
  virtual char getChar() { throw not_support(); }

  virtual void to_string(std::ostream& os){
    os<<toString();
  }

  virtual std::string toString() = 0;
  bool isNull() { return isNull_; };
  virtual Item* copyItem() const = 0;
  // 释放内存， 实际上只有textItem需要
  virtual void release(){};
protected:
  bool isNull_;
};

class NullItem: public Item{
public:
    NullItem(): Item(true) {}
    std::string toString() override {
      return "NULL";
    }

    Item* copyItem() const override{
      return new NullItem();
    }
};

class IntItem : public Item {
private:
  int value;

public:
  // IntItem(int value): Item(false), value(value) {}
  IntItem(int value, bool isNull) : Item(isNull), value(value) {}

  std::string toString() override {
    if(!isNull()){
      return std::to_string(value);
    }else{
      return "NULL";
    }
  }

  int getInt() override {
    return value;
  }

  Item* copyItem() const override{
    return new IntItem(value, isNull_);
  }
};

class DateItem : public Item {
private:
  int value;

public:
  // DateItem(int value): Item(false), value(value) {}
  DateItem(int value, bool isNull) : Item(isNull), value(value) {}

  std::string toString() override{
    if(!isNull()){
      return date2string(value);
    }else{
      return "NULL";
    }
  }

  int getInt() override {
    return value;
  }

  Item* copyItem() const override{
    return new DateItem(value, isNull_);
  }
};

class FloatItem : public Item {
private:
  float value;

public:

  // FloatItem(float value): Item(false), value(value) {}
  FloatItem(float value, bool isNull) : Item(isNull), value(value) {}

  static float round(float v){
    return std::round(v*100) / 100;
  }

  std::string toString() {
    if(!isNull()){
      char buffer[30];
      std::sprintf(buffer, "%.2f", round(value));
      int len = strlen(buffer);
      for(int i = 0; i < 3; ++i){
        if(buffer[len - 1 - i] == '0' || buffer[len - 1 - i] == '.'){
          buffer[len - 1 - i] = 0;
        }else{
          break;
        }
      }
      return buffer;
    }else{
      return "NULL";
    }
  }

  float getFloat() override {
    return value;
  }

  Item* copyItem() const override{
    return new FloatItem(value, isNull_);
  }
};

class BoolItem : public Item {
private:
  bool value;

public:
  // BoolItem(bool value): Item(false), value(value) {}
  BoolItem(bool value, bool isNull): Item(isNull), value(value) {}

  bool getBool() override {
    return value;
  }

  Item* copyItem() const override{
    return new BoolItem(value, isNull_);
  }

  std::string toString() override {
    if(!isNull()){
      return (value?"true":"false");
    }else{
      return "NULL";
    }
  }
};

class CharItem: public Item{
private:
  char value;

public:
  // CharItem(char value): Item(false), value(value) {}
  CharItem(char value, bool isNull): Item(isNull), value(value) {}

  std::string toString() override {
    if(!isNull()){
      return std::to_string(value);
    }else{
      return "NULL";
    }
  }

  char getChar() override{
    return value;
  }

  Item* copyItem() const override{
    return new CharItem(value, isNull_);
  }
};

class TextItem: public Item{
private:
  char* value_;

public:
  // TextItem(const char* value);
  TextItem(const char* value, bool isNull);

  std::string toString() override{
    if(!isNull()){
      return value_;
    }else{
      return "NULL";
    };
  }

  char* getText() override{
    return value_;
  }

  void release() override{
    free(value_);
  }

  Item* copyItem() const override{
    return new TextItem(value_, isNull_);
  }
};

#endif  // MINIDB_ITEM_H
