#pragma once

namespace bad_dispatch {

struct shape_t {
  virtual void do_thing() const = 0;
};

struct circle_t : shape_t {
  void do_thing() const override { }
};

struct triangle_t : shape_t {
  void do_thing() const override { }
};

struct square_t : shape_t {
  void do_thing() const override { }
};

}