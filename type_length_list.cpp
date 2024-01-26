#include <iostream>
#include <utility>
#include <type_traits>
#include <tuple>
#include <functional>

template<auto, typename...>
struct linked_list{};

template <typename T>
struct linked_list<0, T>{

  template <typename U>
      requires std::is_same_v<T, std::remove_cvref_t<U>>
  linked_list<1, T> push_back(U && u) && {
	  
    return {std::move(*this), std::forward<U>(u)};
  }

  template <typename U>
    requires std::is_same_v<T, std::remove_cvref_t<U>>
  linked_list<1, T> push_back(U && u) & {
    return {*this, std::forward<U>(u)};
  }
};

template <auto N, typename T>
struct linked_list<N, T>: public linked_list<N-1, T> {
  T val;
  linked_list<N-1, T>& prev()
  {
    return static_cast<linked_list<N-1, T>&>(*this);
  };

  template <typename U>
      requires std::is_same_v<T, std::remove_cvref_t<U>>
  linked_list<N+1, T> push_back(U && u) && {
    return {std::move(*this), std::forward<U>(u)};
  }

  template <typename U>
    requires std::is_same_v<T, std::remove_cvref_t<U>>
  linked_list<N+1, T> push_back(U && u) & {
    return {*this, std::forward<U>(u)};
  }

};

template <typename T> static linked_list<0, T> empty_list{};

template <typename T, typename R>
linked_list<0, R> map(linked_list<0, T> &a, std::function<R(T)> fn) {
  return empty_list<R>;
}

template <auto N, typename T, typename R>
linked_list<N, R> map(linked_list<N, T> &a, std::function<R(T)> fn) {
  return map(a.prev(), fn).push_back(fn(a.val));
}

template <typename T, typename R>
R fold(linked_list<0, T> &a, R init, std::function<R(R, T)> fn) {
  return init;  
}
template <auto N, typename T, typename R>
R fold(linked_list<N, T> &a, R init, std::function<R(R, T)> fn) {
  return fn(fold(a.prev(), init, fn), a.val);
}

template <typename T, typename R>
R fold1(linked_list<1, T> &a, std::function<R(T, T)> fn) {
  return a.val;  
}
template <auto N, typename T, typename R>
R fold1(linked_list<N, T> &a, std::function<R(T, T)> fn) {
  return fn(fold1(a.prev(), fn), a.val);
}

template <auto M, typename T>
linked_list<M, T> append(linked_list<0, T> a, linked_list<M, T> b) {
  return b;  
}
template <auto N, typename T>
linked_list<N, T> append(linked_list<N, T> a, linked_list<0, T> b) {
  return a;
}
template <auto N, auto M, typename T>
linked_list<N + M, T> append(linked_list<N, T> a, linked_list<M, T> b) {  
  return append(a, b.prev()).push_back(b.val);
}

constexpr bool greater(int a, int b) { return a > b; }

template <typename T>
linked_list<0, T> drop(linked_list<0, T> a) {
  return a;  
}

template <auto N, typename T>
linked_list<N, T> drop(linked_list<N, T> a) {
  return a;  
}

template <auto N, typename T, auto M>
  requires(greater(N, M))
linked_list<N - M, T> drop(linked_list<N, T> a) {
  return drop<M - 1>(a.prev);  
}

// template <auto M, typename T1, typename T2>
// linked_list<0, std::tuple<T1, T2>> table(linked_list<0, T1>,
//                                          linked_list<M, T2>) {  
//   return empty_list<std::tuple<T1,T2>>;
// }
// template <auto N, typename T1, typename T2>
// linked_list<0, std::tuple<T1, T2>> table(linked_list<N, T1>,
//                                          linked_list<0, T2>) {
//   return empty_list<std::tuple<T1,T2>>;  
// }
// template <auto N, auto M, typename T1, typename T2>
// linked_list<N * M, std::tuple<T1, T2>> table(linked_list<N, T1> a,
//                                              linked_list<M, T2> b) {
//   auto concat = [&b](T1 va) {
//     auto concat = [&va](T2 vb) -> std::tuple<T1, T2> { return {va, vb}; };
//     auto mapped_b = map(b, static_cast< std::function< std::tuple<T1,T2>(T1, T2)> >(concat));
//   };

//   return fold1(map(a, concat), append);
// }

template <auto N, typename T>
linked_list<2 * N - 6, T> f(linked_list<N, T> a) {
  auto q = append(a, a);
  auto w = drop<2>(q);
  return w;  
}

int main(){
  auto lst = empty_list<int>;

  auto two_elems = lst.push_back(5).push_back(7);
  auto one_elem = two_elems.prev();
  auto three_elems = one_elem.push_back(2).push_back(3);

  auto q = f(three_elems);

  // auto six_elems = table(two_elems, three_elems);
  
  std::cout << three_elems.val << " <- " << three_elems.prev().val << " <- " << three_elems.prev().prev().val << '\n';
  // std::cout << mapped3.val << " <- " << mapped3.prev().val << " <- " << mapped3.prev().prev().val << '\n';

  return 0;
}

template <typename T>
auto filter(linked_list<0, T> &e, std::function<bool(T)> pred) {
  return e;  
}

template <auto N, auto M, typename T>
linked_list<M, T> filter(linked_list<N, T> &a, std::function<bool(T)> pred) {
  if (pred(a.val)) {
    auto filtered_a = filter(a.prev(), pred);
    auto pushed_a = filtered_a.push_back(a.val);
    return pushed_a;    
  } else {
    return filter(a.prev(), pred);    
  }
}
