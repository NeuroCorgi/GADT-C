#include <iostream>
#include <functional>

template <typename R>
struct Exp {
  virtual R eval() const;
  virtual ~Exp(){};
};

template <typename R>
class Lit : public Exp<R> {
  const R _val;

public:
  R eval() const { return _val; }
  Lit(R val) : _val{val} {}
};

template <typename R, std::function<R(R, R)> Fn>
class BinOp : public Exp<R> {
  Exp<R> &_lhs, &_rhs;  

public:
  R eval() const {
    return F(_lhs.eval(), _rhs.eval());    
  }
};

template <typename R> using Add = BinOp<R, std::plus<R>{}>;
template <typename R> using Sub = BinOp<R, std::minus<R>{}>;

template <typename R>
class Cond : public Exp<R> {
  Exp<bool> &_cond;
  Exp<R> &_then, _else;  

public:
  R eval() const {
	  return _cond.eval() ? _then.eval() : _else.eval();
  }
};

int main() {
  std::cout << "Hello World\n" << std::endl;

  Lit{10};
  
  
  return 0;
}
