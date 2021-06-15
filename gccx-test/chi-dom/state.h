//
// Created by Chiro on 2021/6/15.
//

#ifndef GCCX_TEST_STATE_H
#define GCCX_TEST_STATE_H

#include <memory>

namespace chidom {

template<typename T>
class StateData {
private:
  T data{};
public:
  // explicit StateData(const T &&data_) : StateData(std::forward<T>(data_)) {}

  explicit StateData(const T &data_) : data(data_) {}

  T &get() { return data; }

  void set(const T &data_) { data = data_; }

  // void set(const T &&data_) { set(std::forward<T>(data_)); }
};

template<typename T>
using State = std::shared_ptr<StateData<T>>;

template<typename T>
State<T> useState(const T &data) {
  return std::make_shared<StateData<T>>(data);
}
}

#endif //GCCX_TEST_STATE_H
