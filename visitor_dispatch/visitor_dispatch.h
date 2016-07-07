#pragma once
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace vdsp {

namespace helpers {

template <typename tuple_t, std::size_t... I>
auto tuple_pop_front_impl(const tuple_t& t, std::index_sequence<I...>) {
  return std::forward_as_tuple(std::get<I + 1>(t)...);
}

template <typename... Ts>
auto tuple_pop_front(const std::tuple<Ts...>& t) {
  return tuple_pop_front_impl(t, std::make_index_sequence<sizeof...(Ts)-1>{});
}

template <typename F, typename tuple_t, std::size_t... I>
auto apply_impl(F&& f, tuple_t&& t, std::index_sequence<I...>) {
  return std::invoke(f, std::get<I>(t)...);
}

template <typename F, typename... Ts>
auto apply(F&& f, std::tuple<Ts...>&& t) {
  return apply_impl(f, t, std::make_index_sequence<sizeof...(Ts)>{});
}

}

template <typename...>
struct visitor_t;

template <typename T>
struct visitor_t<T> {
  virtual void operator()(const T&) const = 0;
};
template <typename T, typename... Ts>
struct visitor_t<T, Ts...> : visitor_t<Ts...> {
  using super_t = visitor_t<Ts...>;

  using super_t::operator();
  virtual void operator()(const T&) const = 0;
};

template <typename...>
struct overload_t;

template <typename lambda_t>
struct overload_t<lambda_t> : lambda_t {
  using this_t = lambda_t;
  overload_t(lambda_t&& l) : this_t{ std::forward<lambda_t>(l) } { }

  using this_t::operator();
};

template <typename lambda_t, typename... lambda_ts>
struct overload_t<lambda_t, lambda_ts...> : lambda_t, overload_t<lambda_ts...> {
  using this_t  = lambda_t;
  using super_t = overload_t<lambda_ts...>;
  overload_t(lambda_t&& l, lambda_ts&&... ls) : this_t{ std::forward<lambda_t>(l) }, super_t{ std::forward<lambda_ts>(ls)... } { }

  using super_t::operator();
  using this_t::operator();
};

template <typename... lambda_ts>
auto make_overload(lambda_ts&&... ls) {
  return overload_t<std::decay_t<lambda_ts>...>{ std::forward<lambda_ts>(ls)... };
}

template <typename... elm_ts>
struct visitor_tag_t { using visitor_t = visitor_t<elm_ts...>; };

template <typename>
struct make_visitor_tag;

template <typename... elm_ts>
struct make_visitor_tag<visitor_t<elm_ts...>> { using type = visitor_tag_t<elm_ts...>; };

template <typename visitor_t>
using make_visitor_tag_t = typename make_visitor_tag<visitor_t>::type;

template <typename...>
struct applier_t;

template <typename... elm_ts,
          typename    F,
          typename    R,
          typename... more_super_ts,
          typename... arg_elm_ts>
  struct applier_t<
          visitor_t<elm_ts...>,
          F,
          R,
          std::tuple<const more_super_ts&...>,
          std::tuple<const arg_elm_ts&...>
  > : visitor_t<elm_ts...> {
  F                                   f;
  R*                                  ret;
  std::tuple<const arg_elm_ts&...>    args;
  std::tuple<const more_super_ts&...> parents;
  applier_t(F                                   f,
            R*                                  ret,
            std::tuple<const more_super_ts&...> parents,
            std::tuple<const arg_elm_ts&...>    args) :
    f{ std::move(f) },
    ret{ ret },
    args{ std::move(args) },
    parents{ std::move(parents) } { }
};

template <typename... elm_ts,
          typename    F,
          typename    R,
          typename... more_super_ts,
          typename... arg_elm_ts,
          typename    this_elm_t,
          typename... more_elm_ts>
  struct applier_t<
          visitor_t<elm_ts...>,
          F,
          R,
          std::tuple<more_super_ts...>,
          std::tuple<arg_elm_ts...>,
          this_elm_t,
          more_elm_ts...
  > : applier_t<visitor_t<elm_ts...>,
                F,
                R,
                std::tuple<more_super_ts...>,
                std::tuple<arg_elm_ts...>,
                more_elm_ts...> {
  using super_t = applier_t<visitor_t<elm_ts...>,
                            F,
                            R,
                            std::tuple<more_super_ts...>,
                            std::tuple<arg_elm_ts...>,
                            more_elm_ts...>;
  applier_t(F                                   f,
            R*                                  ret,
            std::tuple<const more_super_ts&...> parents,
            std::tuple<const arg_elm_ts&...>    args) :
    super_t{ std::move(f),
    ret,
    std::move(parents),
    std::move(args) } { }

  using super_t::operator();
  void operator()(const this_elm_t& e) const override {
    dispatch(this->f, e, this->parents, this->args);
  }

  private:
    template <typename F_, typename elm_t_, typename... arg_elm_ts_>
    void dispatch(      F_&&                               f,
                  const elm_t_&                            e,
                  const std::tuple<>&,
                  const std::tuple<const arg_elm_ts_&...>& args) const {
      make_overload(
        [&](void*)     {        helpers::apply(f, std::tuple_cat(args, std::forward_as_tuple(e))); },
        [&](auto* ret) { *ret = helpers::apply(f, std::tuple_cat(args, std::forward_as_tuple(e))); }
      )(this->ret);
    }

    template <typename    F_,
              typename    elm_t_,
              typename    super_t_,
              typename... more_super_ts_,
              typename... arg_elm_ts_,
              typename... elm_ts_>
      void apply(      F_&&                                                   f,
                 const elm_t_&                                                e,
                 const std::tuple<const super_t_&, const more_super_ts_&...>& parents,
                 const std::tuple<const arg_elm_ts_&...>&                     args,
                       visitor_tag_t<elm_ts_...>) const {
      std::get<0>(parents).accept(
        applier_t<
          visitor_t<elm_ts_...>,
          std::decay_t<F_>,
          R,
          std::tuple<const more_super_ts_&...>,
          std::tuple<const arg_elm_ts_&..., const elm_t_&>,
          elm_ts_...>{ std::forward<F_>(f), this->ret, helpers::tuple_pop_front(parents), std::tuple_cat(args, std::forward_as_tuple(e)) }
      );
    }

    template <typename F_,
              typename elm_t_,
              typename super_t_,
              typename... more_super_ts_,
              typename... arg_elm_ts_>
    void dispatch(      F_&&                                                   f,
                  const elm_t_&                                                e,
                  const std::tuple<const super_t_&, const more_super_ts_&...>& parents,
                  const std::tuple<const arg_elm_ts_&...>&                     args) const {
      apply(std::forward<F_>(f), e, parents, args, make_visitor_tag_t<typename super_t_::visitor_t>{});
    }
};

template <typename T>
struct ret_container {
  T t;

  T* get() { return &t; }

  T&& pull() { return std::move(t); }
};

template <>
struct ret_container<void> {
  void* get() { return nullptr; }

  void pull() { return void(); }
};

template <typename R, typename super_t, typename... super_ts, typename F, typename... elm_ts>
R apply_impl(std::tuple<const super_t&, const super_ts&...>&& parents, F&& f, visitor_tag_t<elm_ts...>) {
  using this_applier_t = applier_t<
                            visitor_t<elm_ts...>,
                            std::decay_t<F>,
                            R,
                            std::tuple<const super_ts&...>,
                            std::tuple<>,
                            elm_ts...>;
  ret_container<R> ret;
  std::get<0>(parents).accept(this_applier_t{ std::forward<F>(f), ret.get(), helpers::tuple_pop_front(parents), std::forward_as_tuple() });
  return ret.pull();
}

template <typename R, typename super_t, typename... super_ts, typename F>
R apply(std::tuple<const super_t&, const super_ts&...>&& parents, F&& f) {
  return apply_impl<R>(std::move(parents), std::forward<F>(f), make_visitor_tag_t<typename super_t::visitor_t>{});
}

template <typename R, typename super_t, typename... lambda_ts>
R match(const super_t& super, lambda_ts&&... fs) {
  return apply<R>(std::forward_as_tuple(super), make_overload(std::forward<lambda_ts>(fs)...));
}

template <typename R, typename super_t, typename... lambda_ts>
R match(const super_t& lhs, const super_t& rhs, lambda_ts&&... fs) {
  return apply<R>(std::forward_as_tuple(lhs, rhs), make_overload(std::forward<lambda_ts>(fs)...));
}

}