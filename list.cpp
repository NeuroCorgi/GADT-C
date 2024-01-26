#include <cstddef>

template <typename T, size_t N>
class LinkedList {
  T _val;
  LinkedList<T, N - 1> *_next;

  LinkedList(T v) : _val{v} {};
	
public:
  LinkedList(T val, LinkedList<T, N - 1> next) : _val{val}, _next{next} {};

  static LinkedList<T, 0> Empty();
  
};

template <typename T, int N>
LinkedList<T, 0> LinkedList<T, N>::Empty() {
	return LinkedList{T{}};
}

int main() {
  // auto ll = std::make_unique<LinkedList<int, 0>>();
  // auto empty = LinkedList::Empty<int>;
  
  // auto ll = Node<int>{1, empty};

  // ll.head();
  
  
}
