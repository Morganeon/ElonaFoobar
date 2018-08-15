#include "../thirdparty/catch2/catch.hpp"

#include <sstream>
#include "../character.hpp"
#include "../i18n.hpp"
#include "../item.hpp"
#include "../itemgen.hpp"
#include "../testing.hpp"
#include "../ui.hpp"
#include "../variables.hpp"
#include "tests.hpp"

using namespace std::literals::string_literals;
using namespace elona;

namespace
{
i18n::store load(const std::string& str)
{
    i18n::store store;

    std::stringstream ss(str);

    REQUIRE_NOTHROW(store.load(ss, "test.hcl", "test"));
    return store;
}
} // namespace

TEST_CASE("test formats", "[I18N: Formatting]")
{
    REQUIRE(i18n::fmt_hil("${_1}", 1) == u8"1"s);
    REQUIRE(i18n::fmt_hil("${_1}", u8"foo"s) == u8"foo"s);
    REQUIRE(i18n::fmt_hil("${_1} ${_2}", u8"foo"s, 2) == u8"foo 2"s);
    REQUIRE(i18n::fmt_hil("${_1} ${_1}", u8"foo"s, 2) == u8"foo foo"s);
    REQUIRE(i18n::fmt_hil("${_1} ${_2}", u8"foo"s) == u8"foo <missing>"s);
    REQUIRE(
        i18n::fmt_hil("You see ${_1}.", u8"Palmia") == u8"You see Palmia."s);
    REQUIRE(
        i18n::fmt_hil("You see ${_1} the ${_2}.", u8"Adam", u8"rock thrower")
        == u8"You see Adam the rock thrower."s);
}

TEST_CASE("test format chara", "[I18N: Formatting]")
{
    testing::start_in_debug_map();
    REQUIRE(chara_create(-1, PUTIT_PROTO_ID, 4, 8));
    character& chara = elona::cdata[elona::rc];

    REQUIRE(
        i18n::fmt_hil("${_1}", chara)
        == u8"<character: "s + std::to_string(chara.index) + u8">"s);
}

TEST_CASE("test format item", "[I18N: Formatting]")
{
    testing::start_in_debug_map();
    REQUIRE(itemcreate(-1, PUTITORO_PROTO_ID, 4, 8, 3));
    item& i = elona::inv[elona::ci];

    REQUIRE(
        i18n::fmt_hil("${_1}", i)
        == u8"<item: "s + std::to_string(i.index) + u8">"s);
}

TEST_CASE("test format character by function", "[I18N: Formatting]")
{
    testing::start_in_debug_map();
    character& chara = testing::create_chara(PUTIT_PROTO_ID, 4, 8);

    REQUIRE(i18n::fmt_hil("${name(_1)}", chara) == u8"何か"s);
    REQUIRE(i18n::fmt_hil("${basename(_1)}", chara) == u8"プチ"s);
}

TEST_CASE("test format item by function", "[I18N: Formatting]")
{
    testing::start_in_debug_map();
    item& i = testing::create_item(PUTITORO_PROTO_ID, 3);

    REQUIRE(i18n::fmt_hil("${itemname(_1)}", i) == u8"3個のプチトロ"s);
    REQUIRE(i18n::fmt_hil("${itembasename(_1)}", i) == u8"プチトロ"s);
}


TEST_CASE("test i18n store literal", "[I18N: Store]")
{
    i18n::store store = load(R"(
locale {
    foo = "bar"
}
)");

    REQUIRE(store.get(u8"test.locale.foo") == u8"bar");
    REQUIRE(
        store.get(u8"test.locale.baz") == u8"<Unknown ID: test.locale.baz>");
}

TEST_CASE("test i18n store nested literal", "[I18N: Store]")
{
    i18n::store store = load(R"(
locale {
    foo {
        bar = "baz"
    }
}
)");

    REQUIRE(store.get(u8"test.locale.foo.bar") == u8"baz");
}

TEST_CASE("test i18n store multiple nested literals", "[I18N: Store]")
{
    i18n::store store = load(R"(
locale {
    foo {
        bar = "baz"
    }
    hoge {
        fuga = "piyo"
    }
}
)");

    REQUIRE(store.get(u8"test.locale.foo.bar") == u8"baz");
    REQUIRE(store.get(u8"test.locale.hoge.fuga") == u8"piyo");
}

TEST_CASE("test i18n store enum", "[I18N: Store]")
{
    i18n::store store = load(R"(
locale {
    foo {
        _1 = "bar"
        _2 = "baz"
    }
}
)");

    REQUIRE(store.get_enum(u8"test.locale.foo", 1) == u8"bar");
    REQUIRE(store.get_enum(u8"test.locale.foo", 2) == u8"baz");
}


TEST_CASE("test i18n store complex enum", "[I18N: Store]")
{
    i18n::store store = load(R"(
locale {
    foo {
        _1 {
            name = "bar"
        }
        _2 {
            name = "baz"
        }
    }
}
)");

    REQUIRE(store.get_enum_property(u8"test.locale.foo", "name", 1) == u8"bar");
    REQUIRE(store.get_enum_property(u8"test.locale.foo", "name", 2) == u8"baz");
}

TEST_CASE("test i18n store interpolation", "[I18N: Store]")
{
    i18n::store store = load(R"(
locale {
    foo = "bar: ${_1}"
}
)");

    REQUIRE(store.get(u8"test.locale.foo") == u8"bar: <missing>");
    REQUIRE(store.get(u8"test.locale.foo", 12) == u8"bar: 12");
    REQUIRE(store.get(u8"test.locale.foo", u8"baz") == u8"bar: baz");
    REQUIRE(store.get(u8"test.locale.foo", u8"baz", "hoge") == u8"bar: baz");
}

TEST_CASE("test i18n store multiple interpolation", "[I18N: Store]")
{
    i18n::store store = load(R"(
locale {
    foo = "${_2}: ${_1}"
}
)");

    REQUIRE(store.get(u8"test.locale.foo") == u8"<missing>: <missing>");
    REQUIRE(store.get(u8"test.locale.foo", 42) == u8"<missing>: 42");
    REQUIRE(store.get(u8"test.locale.foo", 12, u8"bar") == u8"bar: 12");
    REQUIRE(store.get(u8"test.locale.foo", u8"bar", u8"baz") == u8"baz: bar");
    REQUIRE(
        store.get(u8"test.locale.foo", u8"bar", u8"baz", "hoge")
        == u8"baz: bar");
}


TEST_CASE("test i18n store enum interpolation", "[I18N: Store]")
{
    i18n::store store = load(R"(
locale {
    foo {
        _1 {
            name = "bar: ${_1}"
        }
        _2 {
            name = "baz: ${_1}"
        }
    }
}
)");

    REQUIRE(
        store.get_enum_property(u8"test.locale.foo", "name", 1, "dood")
        == u8"bar: dood");
    REQUIRE(
        store.get_enum_property(u8"test.locale.foo", "name", 2, "dood")
        == u8"baz: dood");
}

TEST_CASE("test i18n halfwidth katakana", "[I18N: Store]")
{
    i18n::store store = load(
        u8"\
locale {\
    ether_disease = \"ｴｰﾃﾙ病\"\
}\
"s);

    REQUIRE(store.get(u8"test.locale.ether_disease") == u8"ｴｰﾃﾙ病"s);
}

TEST_CASE("test loading i18n data from multiple sources", "[I18N: Store]")
{
    i18n::store store;
    store.init({{testing::get_mods_path() / "test_i18n_a" / "locale" / "jp",
                 "test_i18n_a"},
                {testing::get_mods_path() / "test_i18n_b" / "locale" / "jp",
                 "test_i18n_b"}});


    REQUIRE(store.get(u8"test_i18n_a.locale.test") == u8"こんばんは"s);
    REQUIRE(store.get(u8"test_i18n_b.locale.test") == u8"こんにちは"s);
}
