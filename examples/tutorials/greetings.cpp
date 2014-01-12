#include <skye/mock_function.hpp>

#include <string>

namespace skye {
namespace examples {

/**
 * The class we are going to test.
 *
 * This is a very simplified example of a class that uses an external database.
 */
template<typename database_handle>
class greeter {
public:
  /// Constructor
  greeter(database_handle & handle, std::string const & query_restriction)
    : msg(build_message(handle, query_restriction))
  {}

  /// Return the current greetings.
  std::string const greetings() const { return msg; }

private:
  std::string build_message(
      database_handle & handle, std::string const & query_restriction) {
    // Create the query string
    std::string q = "SELECT portion FROM greetings WHERE ";
    q += query_restriction;
    q += " ORDER BY position";

    // make the query
    auto results = handle.query(q);

    // in real code one would use boost::algorithm::join or something
    // similar ...
    std::string msg;
    char const * sep = "";
    for (auto i : results) {
      msg += sep;
      msg += i;
      sep = " ";
    }
    if (msg != "") {
      msg += "!";
    }
    // return the completed message
    return msg;
  }

private:
  std::string msg;
};

/**
 * A mock version of the database handle class.
 */
struct mock_db {
  typedef std::vector<std::string> results;

  skye::mock_function<results(std::string const&)> query;
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

/**
 * A simplified test for the greeter class.
 */
void test_greetings_classic() {
  typedef greeter<mock_db> tested_unit;

  mock_db db;
  db.query.returns( mock_db::results{ "Hello", "World" } );

  tested_unit t(db, "name = 'classic'");

  db.query.check_called()
    .with("SELECT portion FROM greetings WHERE "
          "name = 'classic' ORDER BY position")
    .once();

  check_equal(t.greetings(), "Hello World!");
}

} // namespace examples
} // namespace skye

int main() {
  try {
    // Run the tests
    skye::examples::test_greetings_classic();
  } catch(std::exception const & ex) {
    std::cerr << "Standard Exception raised: " << ex.what() << std::endl;
    return 1;
  } catch(...) {
    std::cerr << "Unknown exception raised" << std::endl;
    return 1;
  }
  return 0;
}

