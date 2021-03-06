#pragma once
#include "draw.hpp"
#include "lion.hpp"
#include "pic_loader/extent.hpp"

namespace elona
{

struct chara_chip_data
{
    int id;
    extent rect;
    chara_chip_t chip;
    optional<fs::path> filepath;
};

// Used only as an intermediary between registry and initialize_chara_chips().
ELONA_LION_DEFINE_DB(chara_chip_db, chara_chip_data, int, u8"chara_chip")

} // namespace elona
