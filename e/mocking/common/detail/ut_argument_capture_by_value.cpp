#include <e/mocking/common/detail/argument_wrapper.hpp>
#include <e/mocking/common/detail/tuple_streaming.hpp>

#include <boost/test/unit_test.hpp>

using namespace e::mocking::common::detail;

template<typename signature>
class async_functor_holder;
template<typename tuple_type, typename signature>
class async_functor_holder_generic;

template<typename return_type, typename... functor_args>
class async_functor_holder<return_type(functor_args...)>
{
 public:
  typedef std::shared_ptr<async_functor_holder> pointer;
  virtual ~async_functor_holder() {}

  virtual bool equals(pointer const & other) const = 0;
  virtual std::size_t argument_count() const = 0;
  virtual void stream(std::ostream & os) const = 0;
  virtual return_type call_functor(functor_args... args) const = 0;
};

template<typename tuple_type, typename return_type, typename... functor_args>
class async_functor_holder_generic<tuple_type, return_type(functor_args...)> 
    : public async_functor_holder<return_type(functor_args...)>
{
 public:
  typedef async_functor_holder<return_type(functor_args...)> base;
  typedef typename base::pointer pointer;

  async_functor_holder_generic() = default;
  async_functor_holder_generic(
      async_functor_holder_generic const &) = default;

  static pointer create(tuple_type && t) {
    return pointer(new async_functor_holder_generic(
        std::forward<tuple_type>(t)));
  }

  virtual bool equals(pointer const & other) const override {
    auto rhs = dynamic_cast<
      async_functor_holder_generic const*>(other.get());
    if (rhs == nullptr) {
      return false;
    }
    return tuple_ == rhs->tuple_;
  }

  virtual std::size_t argument_count() const override {
    return std::tuple_size<tuple_type>::value;
  }

  virtual void stream(std::ostream & os) const override {
    os << tuple_;
  }

  virtual return_type call_functor(functor_args... args) const override {
    std::size_t const N = std::tuple_size<tuple_type>::value;
    return std::get<N-1>(tuple_).value(args...);
  }

 private:
  async_functor_holder_generic(tuple_type && tuple)
      : tuple_(tuple)
  {}


 private:
  tuple_type tuple_;
};


template<typename... arg_types>
class known_arguments_capture_by_value {
 public:
  typedef decltype(wrap_args_as_tuple(std::declval<arg_types>()...)) value_type;

  static value_type capture(arg_types&&... args) {
    return wrap_args_as_tuple(args...);
  }
};

BOOST_AUTO_TEST_CASE( test_known_arguments_capture_by_value ) {
  typedef known_arguments_capture_by_value<
    int,std::string,std::string &> capture;

  std::string ref("foo");

  auto c1 = capture::capture(1, std::string("bar"), ref);
  BOOST_CHECK_EQUAL(std::get<2>(c1), "foo");
}


template<class signature>
class async_functor_capture_by_value;

template<class return_type, typename... arg_types>
class async_functor_capture_by_value<return_type(arg_types...)>
{
 public:
  typedef typename async_functor_holder<
   return_type(arg_types...)>::pointer value_type;

  template<typename... call_types>
  static value_type capture(call_types&&... args) {
    auto t = wrap_args_as_tuple(args...);
    return async_functor_holder_generic<
      decltype(t),return_type(arg_types...)>::create(std::move(t));
  }
};

void do_stuff(int x, int y) {}

BOOST_AUTO_TEST_CASE( test_async_functor_holder ) {
  typedef async_functor_capture_by_value<void(int,int)> capture;

  std::string a("a"), b("b"), c("c"), d("d");
  auto c1 = capture::capture(
      a, b, [](int const & x, int const & y) { do_stuff(x, y); });
  BOOST_CHECK_EQUAL(c1->argument_count(), 3);

  int x = 0;
  auto c2 = capture::capture(a, b, c, [&x](int a, int b) { x = a + b; });
  BOOST_CHECK_EQUAL(c2->argument_count(), 4);

  c2->call_functor(1, 2);
  BOOST_CHECK_EQUAL(x, 3);
}
