#include <skye/mock_function.hpp>

#include <stdexcept>
#include <string>

namespace skye {
namespace examples {
/**
 * A calculator based on strings.  Uses a separate class to convert
 * between strings and integers.
 */
template<typename convert>
class calculator {
public:
  /**
   * Implement addition for numbers as strings.
   *
   * @param xs a number represented as a string
   * @param ys a number represented as a string
   * @return The addition of @a xs and @a ys as numbers
   */
  std::string add(
      convert & c, std::string const & xs, std::string const & ys) {
    int x = c.parse(xs);
    int y = c.parse(ys);
    return c.format(x + y);
  }

  /**
   * Implement addition for numbers as strings.
   *
   * @param xs a number represented as a string
   * @param ys a number represented as a string
   * @return The addition of @a xs and @a ys as numbers
   */
  std::string mul(
      convert & c, std::string const & xs, std::string const & ys) {
    int x = c.parse(xs);
    int y = c.parse(ys);
    return c.format(x * y);
  }
};

/**
 * A mock converter
 */
struct mocked_convert {
  skye::mock_function<std::string(int)> format;
  skye::mock_function<int(std::string const &)> parse;
};

/**
 * Compare two strings and raise an informative exception if they are different.
 *
 * This is a very naive test, one would use a testing framework
 * normally, but we want to keep the external dependencies minimal in
 * our examples.
 */
void check_equal(std::string const & actual, std::string const & expected) {
  if (actual == expected) {
    return;
  }
  std::string msg("Unexpected result, expected=");
  msg += expected;
  msg += ", actual=";
  msg += actual;
  throw std::runtime_error(msg);
}

/// A very basic test
void test_calculator() {
  typedef calculator<mocked_convert> tested_unit;

  tested_unit calc;

  mocked_convert c;
  c.parse.returns( 2 );
  c.format.returns( std::string("4") );

  std::string actual = calc.add(c, "2", "2");
  check_equal(actual, "4");

  c.parse.check_called().with( "2" ).exactly(2);
  c.format.check_called().with( 4 ).once();

  c.parse.returns( 3 );
  c.format.returns( std::string("9") );

  actual = calc.mul(c, "3", "3");
  check_equal(actual, "9");

  c.parse.check_called().with( "3" ).exactly(2);
  c.format.check_called().with( 9 ).once();
}

} // namespace examples
} // namespace skye

int main() {
  try {
    // Run the tests
    skye::examples::test_calculator();
  } catch(std::exception const & ex) {
    std::cerr << "Standard Exception raised: " << ex.what() << std::endl;
    return 1;
  } catch(...) {
    std::cerr << "Unknown exception raised" << std::endl;
    return 1;
  }
  return 0;
}

