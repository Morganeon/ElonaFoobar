#pragma once

#include <cassert>
#include <memory>
#include <unordered_map>
#include <vector>
#include "cat.hpp"


namespace elona
{



struct ability_data
{
    int id;
    int related_basic_attribute;
    int sdataref1;
    int cost;
    int sdataref3;
    int sdataref4;
};


class ability_db;


namespace cat
{

template <>
struct cat_db_traits<ability_db>
{
    using id_type = int;
    using data_type = ability_data;
    static constexpr const char* filename = u8"ability.lua";
    static constexpr const char* table_name = u8"ability";
};

} // namespace cat



class ability_db : public cat::cat_db<ability_db>
{
public:
    ability_db() = default;

    void define(lua_State* L);
};


extern ability_db the_ability_db;



struct ability
{
    // NOTE: Don't add new fields unless you add them to serialization, which
    // will break save compatibility.
    int current_level = 0;
    int original_level = 0;
    int experience = 0;
    int potential = 0;


    template <typename Archive>
    void serialize(Archive& ar)
    {
        ar(current_level);
        ar(original_level);
        ar(experience);
        ar(potential);
    }
};



class skill_data
{
public:
    skill_data();


    int& operator()(int id, int cc)
    {
        return get(id, cc).current_level;
    }


    ability& get(int id, int cc)
    {
        assert(id < 600);
        return storage[cc][id];
    }


    void clear(int cc);

    void copy(int tc, int cc);


private:
    std::vector<std::vector<ability>> storage;
};


extern skill_data sdata;


struct character;

void chara_init_skill(character& cc, int id, int initial_level);
void chara_init_common_skills(character& cc);

void chara_gain_skill(
    character& cc,
    int id,
    int initial_level = 0,
    int stock = 0);
void gain_special_action();

void chara_gain_fixed_skill_exp(character& cc, int id, int experience);
void chara_gain_skill_exp(
    character& cc,
    int id,
    int experience,
    int experience_divisor_of_related_basic_attribute = 0,
    int experience_divisor_of_character_level = 0);

void gain_digging_experience();
void gain_literacy_experience();
void gain_negotiation_experience(int cc);
void gain_lock_picking_experience(int cc);
void gain_detection_experience(int cc);
void gain_casting_experience(int cc);
void gain_mana_capacity_experience(int cc);
void gain_healing_and_meditation_experience(int cc);
void gain_stealth_experience(int cc);
void gain_investing_experience(int cc);
void gain_weight_lifting_experience(int cc);
void gain_magic_device_experience(int cc);
void gain_fishing_experience(int cc);
void gain_memorization_experience(int cc);
void gain_crafting_experience(int skill, int mat_amount);
void gain_disarm_trap_experience();


} // namespace elona
