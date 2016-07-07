struct shape_intersector_t {
  bool operator()(const circle_t&,   const circle_t&)   const { return true; }
  bool operator()(const circle_t&,   const triangle_t&) const { return false; }
  bool operator()(const circle_t&,   const square_t&)   const { return true; }
  bool operator()(const triangle_t&, const triangle_t&) const { return false; }
  bool operator()(const triangle_t&, const circle_t&)   const { return true; }
  bool operator()(const triangle_t&, const square_t&)   const { return false; }
  bool operator()(const square_t&,   const square_t&)   const { return true; }
  bool operator()(const square_t&,   const circle_t&)   const { return false; }
  bool operator()(const square_t&,   const triangle_t&) const { return true; }
};

struct shape_intersector_good_t {
  bool operator()(const circle_t&,   const circle_t&)   const { return true; }
  bool operator()(const circle_t&,   const triangle_t&) const { return true; }
  bool operator()(const circle_t&,   const square_t&)   const { return true; }
  bool operator()(const triangle_t&, const triangle_t&) const { return true; }
  bool operator()(const triangle_t&, const square_t&)   const { return true; }
  bool operator()(const square_t&,   const square_t&)   const { return true; }
  template <typename T, typename U>
  bool operator()(const T& lhs, const U& rhs) const { return (*this)(rhs, lhs); }
};