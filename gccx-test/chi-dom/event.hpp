//
// Created by Chiro on 2021/6/15.
//

#include "event.h"
#include "VNode.hpp"

namespace chidom {

class Event {
public:
  VNode *target{};
  std::string type{};
};

}