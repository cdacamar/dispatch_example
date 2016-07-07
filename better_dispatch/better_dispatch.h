#pragma once
#include <better_dispatch/better_dispatch_impl.h>

namespace better_dispatch {

#include <common/shape_intersector.h>

template <typename S>
struct rhs_visitor : shape_t::visitor_t {
  bool* const result;
  const S&    lhs;
  rhs_visitor(bool* result, const S& lhs) : result{ result }, lhs { lhs } { }

  void operator()(const circle_t&   c) const override { *result = shape_intersector_t{}(lhs, c); }
  void operator()(const triangle_t& t) const override { *result = shape_intersector_t{}(lhs, t); }
  void operator()(const square_t&   s) const override { *result = shape_intersector_t{}(lhs, s); }
};

struct lhs_visitor : shape_t::visitor_t {
  bool* const    result;
  const shape_t& rhs;
  lhs_visitor(bool* result, const shape_t& rhs) : result{ result }, rhs { rhs } { }

  void operator()(const circle_t&   c) const override { rhs.accept(rhs_visitor<circle_t>{result, c }); }
  void operator()(const triangle_t& t) const override { rhs.accept(rhs_visitor<triangle_t>{result, t }); }
  void operator()(const square_t&   s) const override { rhs.accept(rhs_visitor<square_t>{result, s }); }
};

bool intersect(const shape_t& lhs, const shape_t& rhs) {
  bool result = false;
  lhs.accept(lhs_visitor(&result, rhs));
  return result;
}

}