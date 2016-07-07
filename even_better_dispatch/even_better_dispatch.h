#pragma once
#include <even_better_dispatch/even_better_dispatch_impl.h>

namespace even_better_dispatch {

#include <common/shape_intersector.h>

bool intersect(const shape_t& lhs, const shape_t& rhs) {
  return vdsp::match<bool>(lhs, rhs, shape_intersector_t{});
}

void get_circle(const shape_t& s, const circle_t*& circle) {
  vdsp::match<void>(s,
    [&circle](const circle_t& c) { circle = &c; },
    [](const auto&) {}
  );
}

}