#include <iostream>
#include <bad_dispatch/bad_dispatch.h>
#include <better_dispatch/better_dispatch.h>
#include <even_better_dispatch/even_better_dispatch.h>

#define test(lhs, rhs) { \
                         auto& l = (lhs); auto& r = (rhs); \
                         std::cout<<"intersect("<<#lhs<<','<<#rhs<<") = "<< intersect(l, r)<<'\n'; \
                       }

int main() {
  {
    using namespace bad_dispatch;
    circle_t   circle;
    triangle_t triangle;
    square_t   square;

    test(circle, circle);
    test(circle, triangle);
    test(circle, square);
    test(triangle, triangle);
    test(triangle, circle);
    test(triangle, square);
    test(square, square);
    test(square, circle);
    test(square, triangle);
  }
  {
    using namespace better_dispatch;
    circle_t   circle;
    triangle_t triangle;
    square_t   square;

    test(circle, circle);
    test(circle, triangle);
    test(circle, square);
    test(triangle, triangle);
    test(triangle, circle);
    test(triangle, square);
    test(square, square);
    test(square, circle);
    test(square, triangle);
  }
  {
    using namespace even_better_dispatch;
    circle_t   circle;
    triangle_t triangle;
    square_t   square;

    test(circle, circle);
    test(circle, triangle);
    test(circle, square);
    test(triangle, triangle);
    test(triangle, circle);
    test(triangle, square);
    test(square, square);
    test(square, circle);
    test(square, triangle);
  }
  std::cin.get();
}