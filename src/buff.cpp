#include "buff.hpp"
#include "ability.hpp"
#include "cat.hpp"
#include "character.hpp"
#include "draw.hpp"
#include "elona.hpp"
#include "fov.hpp"
#include "i18n.hpp"
#include "random.hpp"
#include "variables.hpp"



using namespace elona;


namespace
{



constexpr int buff_find_slot_no_effect = -1;

int buff_find_slot(const character& cc, int id, int turns)
{
    for (int i = 0; i < cc.buffs.size(); ++i)
    {
        if (cc.buffs[i].id == id)
        {
            if (cc.buffs[i].turns < turns)
            {
                return i;
            }
            else
            {
                return buff_find_slot_no_effect;
            }
        }
        if (cc.buffs[i].id == 0)
        {
            return i;
        }
    }

    return rnd(static_cast<int>(cc.buffs.size()));
}



} // namespace



namespace elona
{


buff_db the_buff_db;


buff_db::buff_db()
{
    storage.emplace(
        0,
        buff_data{0,
                  LUA_REFNIL,
                  buff_data::type_t::buff,
                  LUA_REFNIL,
                  LUA_REFNIL}); // dummy
}


void buff_db::define(lua_State* L)
{
    const char* id = luaL_checkstring(L, -2);
    if (!id)
        throw std::runtime_error(u8"Error: fail to load buff data");

    ELONA_CAT_DB_FIELD_INTEGER(type_, 0);
    ELONA_CAT_DB_FIELD_REF(duration);
    ELONA_CAT_DB_FIELD_REF(on_refresh);

    cat::ref self = luaL_ref(L, LUA_REGISTRYINDEX);
    // Dummy; after calling this function, the caller pop one value from the Lua
    // stack.
    lua_pushnil(L);

    storage.emplace(
        std::stoi(id), // TODO
        buff_data{
            std::stoi(id),
            self,
            buff_data::type_t(type_),
            duration,
            on_refresh,
        });
}



bool buff_has(const character& cc, int id)
{
    return std::any_of(
        std::begin(cc.buffs), std::end(cc.buffs), [&](const auto& buff) {
            return buff.id == id;
        });
}



optional_ref<const buff_t> buff_find(const character& cc, int id)
{
    const auto itr = std::find_if(
        std::begin(cc.buffs), std::end(cc.buffs), [&](const auto& buff) {
            return buff.id == id;
        });
    if (itr == std::end(cc.buffs))
    {
        return none;
    }
    else
    {
        return *itr;
    }
}



void buff_add(
    character& cc,
    int id,
    int power,
    int turns,
    optional_ref<const character> doer)
{
    if (turns <= 0)
        return;

    const auto slot = buff_find_slot(cc, id, turns);
    if (slot == buff_find_slot_no_effect)
    {
        if (is_in_fov(cc))
        {
            txt(i18n::s.get("core.locale.magic.buff.no_effect"));
            return;
        }
    }

    if (the_buff_db[id]->type == buff_data::type_t::hex)
    {
        bool resists{};
        if (sdata(60, cc.index) / 2 > rnd(power * 2 + 100))
        {
            resists = true;
        }
        if (power * 3 < sdata(60, cc.index))
        {
            resists = true;
        }
        if (power / 3 > sdata(60, cc.index))
        {
            resists = false;
        }
        if (cc.quality > 3)
        {
            if (rnd(4))
            {
                resists = true;
            }
            else
            {
                turns = turns / 5 + 1;
            }
        }
        if (cc.quality >= 4 && id == 16)
        {
            resists = true;
        }
        if (const auto& holy_veil = buff_find(cc, 10))
        {
            if (holy_veil->power + 50 > power * 5 / 2
                || rnd(holy_veil->power + 50) > rnd(power + 1))
            {
                txt(i18n::s.get("core.locale.magic.buff.holy_veil_repels"));
                return;
            }
        }
        if (resists)
        {
            if (is_in_fov(cc))
            {
                txt(i18n::s.get("core.locale.magic.buff.resists", cc));
            }
            return;
        }
        if (doer && doer->index == 0)
        {
            hostileaction(0, cc.index);
        }
    }

    if (is_in_fov(cc))
    {
        // Messages of fodd buff are shown elsewhere.
        if (the_buff_db[id]->type != buff_data::type_t::food)
        {
            txt(lang(
                name(cc.index)
                    + i18n::_(u8"buff", std::to_string(id), u8"message_0"),
                name(cc.index) + u8" "s
                    + i18n::_(u8"buff", std::to_string(id), u8"message_0")
                    + _s(cc.index)
                    + i18n::_(u8"buff", std::to_string(id), u8"message_1")));
        }

        add_damage_popup(
            i18n::_(u8"buff", std::to_string(id), u8"name"),
            cc.index,
            {255, 255, 255});
    }

    cc.buffs[slot] = {id, power, turns};

    chara_refresh(cc.index);
}



void buff_delete(character& cc, int slot)
{
    if (cc.index == 0)
    {
        txtef(8);
        txt(i18n::s.get(
            "core.locale.magic.buff.ends",
            i18n::_(u8"buff", std::to_string(cc.buffs[slot].id), u8"name")));
    }
    if (is_in_fov(cc))
    {
        add_damage_popup(
            i18n::_(u8"buff", std::to_string(cc.buffs[slot].id), u8"name"),
            cc.index,
            {191, 191, 191});
    }
    if (cc.buffs[slot].id == 15)
    {
        if (cc.index == 0)
        {
            incognitoend();
        }
    }
    if (cc.buffs[slot].id == 16)
    {
        cc.is_sentenced_daeth() = false;
    }
    if (cc.buffs[slot].id == 18)
    {
        cc.is_contracting_with_reaper() = false;
    }
    cc.buffs[slot].id = 0;
    for (int cnt = slot, cnt_end = cnt + (16 - slot - 1); cnt < cnt_end; ++cnt)
    {
        if (cc.buffs[cnt].id == 0)
        {
            if (cc.buffs[cnt + 1].id != 0)
            {
                cc.buffs[cnt] = cc.buffs[cnt + 1];
                cc.buffs[cnt + 1] = buff_t{};
            }
            else
            {
                break;
            }
        }
    }

    chara_refresh(cc.index);
}



} // namespace elona
