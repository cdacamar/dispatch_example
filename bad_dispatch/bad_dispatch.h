#pragma once
#include <bad_dispatch/bad_dispatch_impl.h>

#include <cassert>

namespace bad_dispatch {

#include <common/shape_intersector.h>
bool intersect(const shape_t& lhs, const shape_t& rhs) {
  shape_intersector_t intersect;
  if (auto l = dynamic_cast<const circle_t*>(&lhs)) {
    if (auto r = dynamic_cast<const circle_t*>(&rhs)) {
      return intersect(*l, *r);
    }
    else if (auto r = dynamic_cast<const triangle_t*>(&rhs)) {
      return intersect(*l, *r);
    }
    else if (auto r = dynamic_cast<const square_t*>(&rhs)) {
      return intersect(*l, *r);
    }
  }
  else if (auto l = dynamic_cast<const triangle_t*>(&lhs)) {
    if (auto r = dynamic_cast<const circle_t*>(&rhs)) {
      return intersect(*l, *r);
    }
    else if (auto r = dynamic_cast<const triangle_t*>(&rhs)) {
      return intersect(*l, *r);
    }
    else if (auto r = dynamic_cast<const square_t*>(&rhs)) {
      return intersect(*l, *r);
    }
  }
  else if (auto l = dynamic_cast<const square_t*>(&lhs)) {
    if (auto r = dynamic_cast<const circle_t*>(&rhs)) {
      return intersect(*l, *r);
    }
    else if (auto r = dynamic_cast<const triangle_t*>(&rhs)) {
      return intersect(*l, *r);
    }
    else if (auto r = dynamic_cast<const square_t*>(&rhs)) {
      return intersect(*l, *r);
    }
  }
  assert(false && "oh god why...");
  return false;
}

}