#pragma once
#include <visitor_dispatch/visitor_dispatch.h>

namespace even_better_dispatch {

struct circle_t;
struct triangle_t;
struct square_t;

struct shape_t {
  using visitor_t = vdsp::visitor_t<circle_t, triangle_t, square_t>;
  virtual void accept(const visitor_t&) const = 0;
  virtual void do_thing()               const = 0;
};

struct circle_t : shape_t {
  void accept(const visitor_t& visitor) const override { visitor(*this); }
  void do_thing()                       const override { }
};

struct triangle_t : shape_t {
  void accept(const visitor_t& visitor) const override { visitor(*this); }
  void do_thing()                       const override { }
};

struct square_t : shape_t {
  void accept(const visitor_t& visitor) const override { visitor(*this); }
  void do_thing()                       const override { }
};

}