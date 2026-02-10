#include <catch2/catch_session.hpp>
#include <catch2/catch_test_case_info.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

int main(int argc, char** argv) {
  int result = Catch::Session().run(argc, argv);
  return result;
}