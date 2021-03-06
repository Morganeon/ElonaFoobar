#pragma once

#include <array>
#include <unordered_map>
#include "enums.hpp"
#include "i18n.hpp"
#include "lion.hpp"
#include "optional.hpp"



namespace elona
{


struct item_data
{
    int id;
    int image;
    int value;
    int weight;
    int dice_x;
    int dice_y;
    int hit_bonus;
    int damage_bonus;
    int pv;
    int dv;
    int material;
    int chargelevel;
    bool is_readable;
    bool is_zappable;
    bool is_drinkable;
    bool is_cargo;
    bool is_usable;
    int appearance;
    int expiration_date;
    int level;
    int fltselect;
    int category;
    int subcategory;
    int rarity;
    int coefficient;
    int light;
    std::string originalnameref2;
    bool has_random_name;
    color_index_t color;
    std::string filter;
    std::string rffilter;
    i18n_key locale_key_prefix;
    optional<std::string> on_use_callback;
};

ELONA_LION_DEFINE_DB(item_db_ex, item_data, int, u8"item")

extern item_db_ex the_item_db;


} // namespace elona
