#include "eveldb/status.h"  // 记得换成你实际的头文件路径

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <string>
using namespace eveldb;

TEST_CASE("Status Basic", "[basic]") {
  SECTION("Test ok status") {
    Status s1;
    REQUIRE(s1.ok());
    REQUIRE(s1.ToString() == "OK");

    Status s2 = Status::OK();
    REQUIRE(s2.ok());

    REQUIRE_FALSE(s2.isNotFound());
    REQUIRE_FALSE(s2.isCorruption());
    REQUIRE_FALSE(s2.isIOError());
  }

  SECTION("Test factory method") {
    Status s = Status::NotFound("Key not found", "12345");

    REQUIRE_FALSE(s.ok());
    REQUIRE(s.isNotFound());

    REQUIRE_FALSE(s.isCorruption());

    std::string str = s.ToString();
    CHECK(str.find("NotFound") != std::string::npos);
    CHECK(str.find("Key not found") != std::string::npos);
    CHECK(str.find("12345") != std::string::npos);
  }

  SECTION("Test other error") {
    CHECK(Status::Corruption("xx").isCorruption());
    CHECK(Status::NotSupported("xx").isNotSupportedError());
    CHECK(Status::InvalidArgument("xx").isInvalidArgument());
    CHECK(Status::IOError("xx").isIOError());
  }
}

TEST_CASE("Status Copy/Move", "[basic]") {
  SECTION("Copy") {
    Status s1 = Status::IOError("Disk full");

    Status s2(s1);
    REQUIRE(s2.isIOError());
    REQUIRE(s2.ToString() == s1.ToString());

    Status s3 = Status::OK();
    s3 = s1;
    REQUIRE(s3.isIOError());
    REQUIRE(s3.ToString() == s1.ToString());

    s1 = Status::OK();
    REQUIRE(s1.ok());
    REQUIRE(s3.isIOError());
  }

  SECTION("Move") {
    Status s1 = Status::InvalidArgument("Bad Arg");

    Status s2(std::move(s1));

    REQUIRE(s2.isInvalidArgument());
    REQUIRE(s2.ToString().find("Bad Arg") != std::string::npos);

    REQUIRE(s1.ok());

    Status s3 = Status::OK();
    s3 = std::move(s2);

    REQUIRE(s3.isInvalidArgument());
    REQUIRE(s2.ok());
  }
}

TEST_CASE("Status Copy/Move With String", "[basic]") {
  Status notfound = Status::NotFound("hello is not found", "exit");

  SECTION("Test copy") {
    Status notfound2 = notfound;
    REQUIRE(notfound2.ToString().find("NotFound") != std::string::npos);
    REQUIRE(notfound2.ToString().find("hello is not found") !=
            std::string::npos);
    REQUIRE(notfound2.ToString().find("exit") != std::string::npos);
  }

  SECTION("Test move") {
    Status notfound2;
    REQUIRE(notfound2.ok());
    notfound2 = std::move(notfound);
    REQUIRE(notfound2.ToString().find("NotFound") != std::string::npos);
    REQUIRE(notfound2.ToString().find("hello is not found") !=
            std::string::npos);
    REQUIRE(notfound2.ToString().find("exit") != std::string::npos);
    REQUIRE(notfound.ok());
  }
}