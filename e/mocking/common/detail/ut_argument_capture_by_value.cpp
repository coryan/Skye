#include <e/mocking/common/detail/argument_capture_by_value.hpp>
#include <e/mocking/common/detail/argument_wrapper.hpp>
#include <e/mocking/common/detail/generic_arglist_capture.hpp>
#include <e/mocking/common/detail/tuple_streaming.hpp>

#include <boost/test/unit_test.hpp>

using namespace e::mocking::common::detail;

/**
 * Define a strategy to capture arguments in mock functions.
 *
 * Mock functions (and particularly template mock functions) require
 * us to capture argument lists of unknown types.  Moreover, depending
 * on the intended usage, the tester may want to operate on the
 * captured arguments, such as counting how many arguments match some
 * values, or types, or even may need to examine the values of the
 * capture.
 *
 * In more dynamic languages than C++ this can be perfomed by holding
 * on to the arguments, or to objects that wrap the arguments if
 * needed.  Then using duck typing one can call on the objects.  But
 * just expressing the type "a vector of heterogenous things" is not
 * possible in C++.
 *
 * It is not too difficult to design a class that captures
 * heteronegous tuples.  Just wrap the tuples in class template all
 * with a common base, as in:
 *
 * @code
 * class base { .... };
 *
 * template<typename tuple> tuple_holder<tuple> : public base { ... };
 * @endcode
 *
 * This works in that the values are captured (except for non-copyable
 * objects, more or this later), but does not provide a good way to
 * operate on the values.  Using some dynamic polymorphism, combined
 * with RTTI one can at least get comparisons working for to implement
 * things like with() filters:
 *
 * @code
 * bool tuple_holder<tuple>::compare(base const * other) const override {
 *   auto rhs = dynamic_cast<tuple_holder<tuple> const*>(other);
 *   if (rhs == nullptr) { return false; }
 *   // compare tuples as usual!
 * }
 * @endcode
 *
 * Boost.ASIO provides another interesting motivation.  In this
 * framework one often receives callbacks when an operation completes:
 *
 * @code
 * socket s;
 * my_object foo;
 *
 * boost::asio::async_read_until(s, some_buffer, '\n',
 *   [foo](boost::system::error_code const & ec) {
 *     foo.my_callback(ec);
 *   });
 * @endcode
 *
 * writing tests for @em my_object requires mocking the socket class,
 * as in:
 *
 * @code
 * class mock_socket {
 *   mock_function_type_to_be_defined async_write_some;
 * };
 * // .. more stuff ...
 * mock_socket s;
 * my_object foo;
 *
 * boost::asio::async_read_until(s, some_buffer, '\n',
 *   [foo](boost::system::error_code const & ec) {
 *     foo.my_callback(ec);
 *   });
 * @endcode
 *
 * Boost.ASIO will call s.async_write_some more or less like this:
 * 
 * @code
 * s.async_write_some(some arguments ..., functor);
 * @endcode
 * 
 * One must call the functor provided to exercise the code in the
 * my_object class.
 *
 * In this case one would want to capture the functors in a separate
 * sequence of object.  The functors have signatures known at compile
 * time, and it would be desirable to make them available wrapped in
 * classes with nice virtual functions.
 *
 * The argument_argument_capture_strategy defines how a
 * mock_template_function captures its arguments.  The default
 * template parameter value simple captures the arguments by value,
 * replacing non-copyable arguments with a placeholder.
 */
class arguments_by_value_holder {
 public:
  typedef std::shared_ptr<arguments_by_value_holder> pointer;
  virtual ~arguments_by_value_holder() {}

  virtual bool equals(pointer const & other) const = 0;
  virtual std::size_t argument_count() const = 0;
  virtual void stream(std::ostream & os) const = 0;
};

template<typename tuple_type>
class arguments_by_value_holder_generic : public arguments_by_value_holder {
 public:
  typedef arguments_by_value_holder base;
  typedef typename base::pointer pointer;

  arguments_by_value_holder_generic() = default;
  arguments_by_value_holder_generic(
      arguments_by_value_holder_generic const &) = default;

  static pointer create(tuple_type && t) {
    return pointer(new arguments_by_value_holder_generic(
        std::forward<tuple_type>(t)));
  }

  virtual bool equals(pointer const & other) const override {
    auto rhs = dynamic_cast<
      arguments_by_value_holder_generic const*>(other.get());
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

 private:
  arguments_by_value_holder_generic(tuple_type && tuple)
      : tuple_(tuple)
  {}


 private:
  tuple_type tuple_;
};

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
    return std::get<N-1>(tuple_)(args...);
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

class unknown_arguments_capture_by_value {
 public:
  typedef arguments_by_value_holder::pointer value_type;

  template<typename... arg_types>
  static value_type capture(arg_types&&... args) {
    auto t = wrap_args_as_tuple(args...);
    return arguments_by_value_holder_generic<decltype(t)>::create(std::move(t));
  }
};

BOOST_AUTO_TEST_CASE( test_known_arguments_capture_by_value ) {
  typedef known_arguments_capture_by_value<
    int,std::string,std::string &> capture;

  std::string ref("foo");

  auto c1 = capture::capture(1, std::string("bar"), ref);
  BOOST_CHECK_EQUAL(std::get<2>(c1), "foo");
}

BOOST_AUTO_TEST_CASE( test_unknown_arguments_capture_by_value ) {
  std::string a("a"), b("b"), c("c"), d("d");

  auto c1 = unknown_arguments_capture_by_value::capture(1, 2, a, b);
  BOOST_CHECK_EQUAL(c1->argument_count(), 4);
  std::ostringstream os;
  c1->stream(os);
  BOOST_CHECK_EQUAL(os.str(), "<1,2,a,b>");

  auto c3 = unknown_arguments_capture_by_value::capture(a, b, c);
  BOOST_CHECK_EQUAL(c3->argument_count(), 3);
  os.str("");
  os.clear();
  c3->stream(os);
  BOOST_CHECK_EQUAL(os.str(), "<a,b,c>");
  auto c4 = unknown_arguments_capture_by_value::capture(a, b, c);
  BOOST_CHECK(c3->equals(c4));
  auto c5 = unknown_arguments_capture_by_value::capture(b, c, d);
  BOOST_CHECK(not c3->equals(c5));

  c4 = unknown_arguments_capture_by_value::capture(a, b, c, 1, 2, 3, 4);
  BOOST_CHECK_EQUAL(c4->argument_count(), 7);
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

#if 0
  typedef async_functor_capture_by_value<void(int,int)> capture;

  std::string a("a"), b("b"), c("c"), d("d");
  auto c1 = capture::capture(a, b, [](int x, int y) { do_stuff(x, y); });
  BOOST_CHECK_EQUAL(c1->argument_count(), 3);

  int x = 0;
  auto c2 = capture::capture(a, b, c, [&x](int a, int b) { x = a + b; });
  BOOST_CHECK_EQUAL(c2->argument_count(), 4);

  c2->call_functor(1, 2);
  BOOST_CHECK_EQUAL(x, 3);
#endif
}
