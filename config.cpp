#include "config.hpp"
#include <fstream>
#include <functional>
#include <stdexcept>
#include "elona.hpp"
#include "json.hpp"
#include "range.hpp"
#include "variables.hpp"



namespace
{


struct config_loading_error : public std::runtime_error
{
    config_loading_error(const std::string& message)
        : std::runtime_error(message)
    {
    }
};



template <typename Iterator, typename Function>
void for_each_with_index(Iterator first, Iterator last, Function f)
{
    for (size_t index = 0; first != last; ++first, ++index)
    {
        (void)f(index, *first);
    }
}



struct config_base
{
    virtual void set(const picojson::object& options) = 0;
};



// TODO: rename
template <typename T>
struct config_value : public config_base
{
    config_value(
        const std::string& name,
        std::function<void(const T&)> callback)
        : name(name)
        , callback(callback)
    {
    }


    virtual ~config_value() = default;


    virtual void set(const picojson::object& options) override
    {
        callback(options.at(name).template get<T>());
    }


private:
    std::string name;
    std::function<void(const T&)> callback;
};


using config_integer = config_value<int64_t>;
using config_string = config_value<std::string>;



struct config_key : public config_base
{
    config_key(
        const std::string& name,
        std::function<void(const std::string&, int)> callback)
        : name(name)
        , callback(callback)
    {
    }


    virtual ~config_key() = default;


    virtual void set(const picojson::object& options) override
    {
        callback(options.at(name).get<std::string>(), 0 /* TODO */);
    }


private:
    std::string name;
    std::function<void(const std::string&, int)> callback;
};



struct config_key_sequence : public config_base
{
    config_key_sequence(
        const std::string& name,
        std::function<void(const std::vector<std::string>&)> callback)
        : name(name)
        , callback(callback)
    {
    }


    virtual ~config_key_sequence() = default;


    virtual void set(const picojson::object& options) override
    {
        const auto keys = options.at(name).get<picojson::array>();
        std::vector<std::string> keys_;
        range::transform(
            keys, std::back_inserter(keys_), [](const picojson::value& key) {
                return key.get<std::string>();
            });
        callback(keys_);
    }


private:
    std::string name;
    std::function<void(const std::vector<std::string>&)> callback;
};



} // namespace



namespace elona
{



void load_config()
{
    // FIXME std::string{value} => value
    std::unique_ptr<config_base> config_list[] = {
        std::make_unique<config_integer>(
            u8"alert_wait",
            [&](auto value) { config::instance().alert = value; }),
        std::make_unique<config_integer>(
            u8"anime_wait",
            [&](auto value) { config::instance().animewait = value; }),
        std::make_unique<config_integer>(
            u8"ignoreDislike",
            [&](auto value) { config::instance().ignoredislike = value; }),
        std::make_unique<config_integer>(
            u8"wait1",
            [&](auto value) { config::instance().wait1 = value; }),
        std::make_unique<config_string>(
            u8"font1",
            [&](auto value) { config::instance().font1() = std::string{value}; }),
        std::make_unique<config_string>(
            u8"font2",
            [&](auto value) { config::instance()._font2() = std::string{value}; }),
        std::make_unique<config_integer>(
            u8"fontVfix1",
            [&](auto value) { vfix = value; }),
        std::make_unique<config_integer>(
            u8"fontSfix1",
            [&](auto value) { sizefix = value; }),
        std::make_unique<config_integer>(
            u8"story",
            [&](auto value) { config::instance().story = value; }),
        std::make_unique<config_integer>(
            u8"heartbeat",
            [&](auto value) { config::instance().heart = value; }),
        std::make_unique<config_integer>(
            u8"extraHelp",
            [&](auto value) { config::instance().extrahelp = value; }),
        std::make_unique<config_integer>(
            u8"hpBar",
            [&](auto value) { config::instance().hp_bar = value; }),
        std::make_unique<config_integer>(
            u8"leashIcon",
            [&](auto value) { config::instance().leash_icon = value; }),
        std::make_unique<config_integer>(
            u8"alwaysCenter",
            [&](auto value) { config::instance().alwayscenter = value; }),
        std::make_unique<config_integer>(
            u8"scroll",
            [&](auto value) { config::instance().scroll = value; }),
        std::make_unique<config_integer>(
            u8"startRun",
            [&](auto value) { config::instance().startrun = value; }),
        std::make_unique<config_integer>(
            u8"walkWait",
            [&](auto value) { config::instance().walkwait = value; }),
        std::make_unique<config_integer>(
            u8"runWait",
            [&](auto value) { config::instance().runwait = value; }),
        std::make_unique<config_integer>(
            u8"autoTurnType",
            [&](auto value) { config::instance().autoturn = value; }),
        std::make_unique<config_integer>(
            u8"autoNumlock",
            [&](auto value) { config::instance().autonumlock = value; }),
        std::make_unique<config_integer>(
            u8"attackWait",
            [&](auto value) { config::instance().attackwait = value; }),
        std::make_unique<config_integer>(
            u8"attackAnime",
            [&](auto value) { config::instance().attackanime = value; }),
        std::make_unique<config_integer>(
            u8"envEffect",
            [&](auto value) { config::instance().env = value; }),
        std::make_unique<config_integer>(
            u8"titleEffect",
            [&](auto) { /* Unsupported option */ }),
        std::make_unique<config_integer>(
            u8"net",
            [&](auto value) { config::instance().net = value; }),
        std::make_unique<config_integer>(
            u8"netWish",
            [&](auto value) { config::instance().netwish = value; }),
        std::make_unique<config_integer>(
            u8"netChat",
            [&](auto value) { config::instance().netchat = value; }),
        std::make_unique<config_integer>(
            u8"serverList",
            [&](auto value) { config::instance().serverlist = value; }),
        std::make_unique<config_integer>(
            u8"shadow",
            [&](auto value) { config::instance().shadow = value; }),
        std::make_unique<config_integer>(
            u8"objectShadow",
            [&](auto value) { config::instance().objectshadow = value; }),
        std::make_unique<config_integer>(
            u8"windowAnime",
            [&](auto value) { config::instance().windowanime = value; }),
        std::make_unique<config_integer>(
            u8"hide_autoIdentify",
            [&](auto value) { config::instance().hideautoidentify = value; }),
        std::make_unique<config_integer>(
            u8"hide_shopResult",
            [&](auto value) { config::instance().hideshopresult = value; }),
        std::make_unique<config_integer>(
            u8"msg_trans",
            [&](auto value) { config::instance().msgtrans = value; }),
        std::make_unique<config_integer>(
            u8"msg_addTime",
            [&](auto value) { config::instance().msgaddtime = value; }),
        std::make_unique<config_key>(
            u8"key_cancel",
            [&](auto value, auto jk) {
                key_cancel = std::string{value};
                jkey(jk) = std::string{value};
            }),
        std::make_unique<config_key>(
            u8"key_esc",
            [&](auto value, auto jk) {
                key_esc = std::string{value};
                jkey(jk) = std::string{value};
            }),
        std::make_unique<config_key>(
            u8"key_alter",
            [&](auto value, auto jk) {
                key_alter = std::string{value};
                jkey(jk) = std::string{value};
            }),
        std::make_unique<config_string>(
            u8"key_north",
            [&](auto value) { key_north = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_south",
            [&](auto value) { key_south = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_west",
            [&](auto value) { key_west = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_east",
            [&](auto value) { key_east = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_northwest",
            [&](auto value) { key_northwest = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_northeast",
            [&](auto value) { key_northeast = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_southwest",
            [&](auto value) { key_southwest = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_southeast",
            [&](auto value) { key_southeast = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_wait",
            [&](auto value) { key_wait = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_inventory",
            [&](auto value) { key_inventory = std::string{value}; }),
        std::make_unique<config_key>(
            u8"key_help",
            [&](auto value, auto jk) {
                key_help = std::string{value};
                jkey(jk) = std::string{value};
            }),
        std::make_unique<config_string>(
            u8"key_msglog",
            [&](auto value) { key_msglog = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_pageup",
            [&](auto value) { key_pageup = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_pagedown",
            [&](auto value) { key_pagedown = std::string{value}; }),
        std::make_unique<config_key>(
            u8"key_get",
            [&](auto value, auto jk) {
                key_get = std::string{value};
                jkey(jk) = std::string{value};
            }),
        std::make_unique<config_string>(
            u8"key_get2",
            [&](auto value) { key_get2 = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_drop",
            [&](auto value) { key_drop = std::string{value}; }),
        std::make_unique<config_key>(
            u8"key_charainfo",
            [&](auto value, auto jk) {
                key_charainfo = std::string{value};
                jkey(jk) = std::string{value};
            }),
        std::make_unique<config_key>(
            u8"key_enter",
            [&](auto value, auto jk) {
                key_enter = std::string{value};
                jkey(jk) = std::string{value};
            }),
        std::make_unique<config_string>(
            u8"key_eat",
            [&](auto value) { key_eat = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_wear",
            [&](auto value) { key_wear = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_cast",
            [&](auto value) { key_cast = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_drink",
            [&](auto value) { key_drink = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_read",
            [&](auto value) { key_read = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_zap",
            [&](auto value) { key_zap = std::string{value}; }),
        std::make_unique<config_key>(
            u8"key_fire",
            [&](auto value, auto jk) {
                key_fire = std::string{value};
                jkey(jk) = std::string{value};
            }),
        std::make_unique<config_string>(
            u8"key_goDown",
            [&](auto value) { key_godown = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_goUp",
            [&](auto value) { key_goup = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_save",
            [&](auto value) { key_save = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_search",
            [&](auto value) { key_search = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_interact",
            [&](auto value) { key_interact = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_identify",
            [&](auto value) { key_identify = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_skill",
            [&](auto value) { key_skill = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_close",
            [&](auto value) { key_close = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_rest",
            [&](auto value) { key_rest = std::string{value}; }),
        std::make_unique<config_key>(
            u8"key_target",
            [&](auto value, auto jk) {
                key_target = std::string{value};
                jkey(jk) = std::string{value};
            }),
        std::make_unique<config_string>(
            u8"key_dig",
            [&](auto value) { key_dig = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_use",
            [&](auto value) { key_use = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_bash",
            [&](auto value) { key_bash = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_open",
            [&](auto value) { key_open = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_dip",
            [&](auto value) { key_dip = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_pray",
            [&](auto value) { key_pray = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_offer",
            [&](auto value) { key_offer = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_journal",
            [&](auto value) { key_journal = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_material",
            [&](auto value) { key_material = std::string{value}; }),
        std::make_unique<config_key>(
            u8"key_quick",
            [&](auto value, auto jk) {
                key_quick = std::string{value};
                jkey(jk) = std::string{value};
            }),
        std::make_unique<config_string>(
            u8"key_trait",
            [&](auto value) { key_trait = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_look",
            [&](auto value) { key_look = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_give",
            [&](auto value) { key_give = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_throw",
            [&](auto value) { key_throw = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_mode",
            [&](auto value) { key_mode = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_mode2",
            [&](auto value) { key_mode2 = std::string{value}; }),
        std::make_unique<config_key>(
            u8"key_ammo",
            [&](auto value, auto jk) {
                key_ammo = std::string{value};
                jkey(jk) = std::string{value};
            }),
        std::make_unique<config_key>(
            u8"key_quickinv",
            [&](auto value, auto jk) {
                key_quickinv = std::string{value};
                jkey(jk) = std::string{value};
            }),
        std::make_unique<config_string>(
            u8"key_quicksave",
            [&](auto value) { key_quicksave = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_quickload",
            [&](auto value) { key_quickload = std::string{value}; }),
        std::make_unique<config_string>(
            u8"key_autodig",
            [&](auto value) { key_autodig = std::string{value}; }),
        std::make_unique<config_integer>(
            u8"zkey",
            [&](auto value) { config::instance().zkey = value; }),
        std::make_unique<config_integer>(
            u8"xkey",
            [&](auto value) { config::instance().xkey = value; }),
        std::make_unique<config_integer>(
            u8"scr_sync",
            [&](auto value) { config::instance().scrsync = value; }),
        std::make_unique<config_integer>(
            u8"scroll_run",
            [&](auto value) { config::instance().runscroll = value; }),
        std::make_unique<config_integer>(
            u8"skipRandEvents",
            [&](auto value) { config::instance().skiprandevents = value; }),
        std::make_unique<config_key_sequence>(
            u8"key_set",
            [&](const auto& values) {
                for_each_with_index(
                    std::begin(values),
                    std::end(values),
                    [&](auto index, auto value) { key_select(index) = value; });
            }),
    };

    picojson::value value;

    {
        std::ifstream file{filesystem::path(u8"./config.json").native(),
                           std::ios::binary};
        if (!file)
        {
            throw config_loading_error{
                u8"Failed to open: "s
                + filesystem::make_preferred_path_in_utf8(
                      filesystem::path(u8"./config.json"))};
        }

        file >> value;
    }

    const picojson::object& options = value.get<picojson::object>();
    for (const auto& config : config_list)
    {
        config->set(options);
    }

    key_prev = key_northwest;
    key_next = key_northeast;

    if (config::instance().zkey == 0)
    {
        key_quick = u8"z"s;
        key_zap = u8"Z"s;
    }
    else if (config::instance().zkey == 1)
    {
        key_zap = u8"z"s;
        key_quick = u8"Z"s;
    }
    if (config::instance().xkey == 0)
    {
        key_quickinv = u8"x"s;
        key_inventory = u8"X"s;
    }
    else if (config::instance().xkey == 1)
    {
        key_inventory = u8"x"s;
        key_quickinv = u8"X"s;
    }
    if (config::instance().scrsync == 0)
    {
        config::instance().scrsync = 3;
    }
    if (config::instance().walkwait == 0)
    {
        config::instance().walkwait = 5;
    }
    if (config::instance().runwait < 1)
    {
        config::instance().runwait = 1;
    }
    if (config::instance().attackwait < 1)
    {
        config::instance().attackwait = 1;
    }
    if (config::instance().startrun >= 20)
    {
        config::instance().startrun = 1000;
    }

    if (config::instance().language == -1)
    {
        buffer(4);
        picload(filesystem::path(u8"./graphic/lang.bmp"));
        gsel(0);
        gmode(0);
        p = 0;

        while (1)
        {
            boxf();
            pos(160, 170);
            gcopy(4, 0, 0, 340, 100);
            pos(180, 220 + p * 20);
            gcopy(4, 360, 6, 20, 18);
            redraw();
            await(30);
            if (getkey(snail::key::down))
            {
                p = 1;
            }
            if (getkey(snail::key::keypad_2))
            {
                p = 1;
            }
            if (getkey(snail::key::up))
            {
                p = 0;
            }
            if (getkey(snail::key::keypad_8))
            {
                p = 0;
            }
            if (getkey(snail::key::enter))
            {
                break;
            }
            if (getkey(snail::key::space))
            {
                break;
            }
        }

        config::instance().language = p;
        set_config(u8"language", p);
    }
    if (config::instance().language == 0)
    {
        jp = 1;
        vfix = 0;
        sizefix = 0;
    }
    else
    {
        en = 1;
    }
    if (key_mode == ""s)
    {
        key_mode = u8"z"s;
        set_config("key_mode", key_mode);
    }
    if (key_mode2 == ""s)
    {
        key_mode2 = u8"*"s;
        set_config("key_mode2", key_mode2);
    }
    if (key_ammo == ""s)
    {
        key_ammo = u8"A"s;
        set_config("key_mode2", key_ammo);
    }
}



void set_config(const std::string& key, int value)
{
    picojson::value options;

    {
        std::ifstream file{filesystem::path(u8"./config.json").native(),
                           std::ios::binary};
        if (!file)
        {
            throw config_loading_error{
                u8"Failed to open: "s
                + filesystem::make_preferred_path_in_utf8(
                      filesystem::path(u8"./config.json"))};
        }
        file >> options;
    }

    options.get(key) = picojson::value{int64_t{value}};

    {
        std::ofstream file{filesystem::path(u8"./config.json").native(),
                           std::ios::binary};
        if (!file)
        {
            throw config_loading_error{
                u8"Failed to open: "s
                + filesystem::make_preferred_path_in_utf8(
                      filesystem::path(u8"./config.json"))};
        }
        options.serialize(std::ostream_iterator<char>(file), true);
    }
}



void set_config(const std::string& key, const std::string& value)
{
    picojson::value options;

    {
        std::ifstream file{filesystem::path(u8"./config.json").native(),
                           std::ios::binary};
        if (!file)
        {
            throw config_loading_error{
                u8"Failed to open: "s
                + filesystem::make_preferred_path_in_utf8(
                      filesystem::path(u8"./config.json"))};
        }
        file >> options;
    }

    options.get(key) = picojson::value{value};

    {
        std::ofstream file{filesystem::path(u8"./config.json").native(),
                           std::ios::binary};
        if (!file)
        {
            throw config_loading_error{
                u8"Failed to open: "s
                + filesystem::make_preferred_path_in_utf8(
                      filesystem::path(u8"./config.json"))};
        }
        options.serialize(std::ostream_iterator<char>(file), true);
    }
}



void set_config(const std::string& key, const std::string& value1, int value2)
{
    picojson::value options;

    {
        std::ifstream file{filesystem::path(u8"./config.json").native(),
                           std::ios::binary};
        if (!file)
        {
            throw config_loading_error{
                u8"Failed to open: "s
                + filesystem::make_preferred_path_in_utf8(
                      filesystem::path(u8"./config.json"))};
        }
        file >> options;
    }

    options.get(key) = picojson::value{value1};
    (void)value2; // TODO

    {
        std::ofstream file{filesystem::path(u8"./config.json").native(),
                           std::ios::binary};
        if (!file)
        {
            throw config_loading_error{
                u8"Failed to open: "s
                + filesystem::make_preferred_path_in_utf8(
                      filesystem::path(u8"./config.json"))};
        }
        options.serialize(std::ostream_iterator<char>(file), true);
    }
}



void load_config2()
{
    key_select(0) = u8"a"s;
    key_select(1) = u8"b"s;
    key_select(2) = u8"c"s;
    key_select(3) = u8"d"s;
    key_select(4) = u8"e"s;
    key_select(5) = u8"f"s;
    key_select(6) = u8"g"s;
    key_select(7) = u8"h"s;
    key_select(8) = u8"i"s;
    key_select(9) = u8"j"s;
    key_select(10) = u8"k"s;
    key_select(11) = u8"l"s;
    key_select(12) = u8"m"s;
    key_select(13) = u8"n"s;
    key_select(14) = u8"o"s;
    key_select(15) = u8"p"s;
    key_select(16) = u8"q"s;
    key_select(17) = u8"r"s;
    key_select(18) = u8"s"s;

    std::unique_ptr<config_base> config_list[] = {
        std::make_unique<config_integer>(
            u8"language",
            [&](auto value) { config::instance().language = value; }),
        std::make_unique<config_integer>(
            u8"fullscreen",
            [&](auto value) { config::instance().fullscreen = value; }),
        std::make_unique<config_integer>(
            u8"music", [&](auto value) { config::instance().music = value; }),
        std::make_unique<config_integer>(
            u8"sound", [&](auto value) { config::instance().sound = value; }),
        std::make_unique<config_integer>(
            u8"extraRace",
            [&](auto value) { config::instance().extrarace = value; }),
        std::make_unique<config_integer>(
            u8"extraClass",
            [&](auto value) { config::instance().extraclass = value; }),
        std::make_unique<config_integer>(
            u8"joypad", [&](auto value) { config::instance().joypad = value; }),
        std::make_unique<config_integer>(
            u8"msgLine", [&](auto value) { inf_msgline = value; }),
        std::make_unique<config_integer>(
            u8"tileSize", [&](auto value) { inf_tiles = value; }),
        std::make_unique<config_integer>(
            u8"fontSize", [&](auto value) { inf_mesfont = value; }),
        std::make_unique<config_integer>(
            u8"infVerType", [&](auto value) { inf_vertype = value; }),
        std::make_unique<config_integer>(
            u8"windowX", [&](auto value) { windowx = value; }),
        std::make_unique<config_integer>(
            u8"windowY", [&](auto value) { windowy = value; }),
        std::make_unique<config_integer>(
            u8"windowW", [&](auto value) { windoww = value; }),
        std::make_unique<config_integer>(
            u8"windowH", [&](auto value) { windowh = value; }),
        std::make_unique<config_integer>(
            u8"clockX", [&](auto value) { inf_clockx = value; }),
        std::make_unique<config_integer>(
            u8"clockW", [&](auto value) { inf_clockw = value; }),
        std::make_unique<config_integer>(
            u8"clockH", [&](auto value) { inf_clockh = value; }),
        std::make_unique<config_string>(
            u8"defLoadFolder", [&](auto value) { defload = value; }),
        std::make_unique<config_integer>(
            u8"charamake_wiz",
            [&](auto value) { config::instance().wizard = value; }),
    };

    std::ifstream file{filesystem::path(u8"./config.json").native(),
                       std::ios::binary};
    if (!file)
    {
        throw config_loading_error{u8"Failed to open: "s
                                   + filesystem::make_preferred_path_in_utf8(
                                         filesystem::path(u8"./config.json"))};
    }

    picojson::value value;
    file >> value;

    const picojson::object& options = value.get<picojson::object>();
    for (const auto& config : config_list)
    {
        config->set(options);
    }
}


config& config::instance()
{
    static config the_instance;
    return the_instance;
}



} // namespace elona
