#pragma once

namespace better_dispatch {

struct shape_t {
  struct visitor_t;
  virtual void accept(const visitor_t&) const = 0;
  virtual void do_thing()               const = 0;
};

struct circle_t;
struct triangle_t;
struct square_t;
struct shape_t::visitor_t {
  virtual void operator()(const circle_t&)   const = 0;
  virtual void operator()(const triangle_t&) const = 0;
  virtual void operator()(const square_t&)   const = 0;
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