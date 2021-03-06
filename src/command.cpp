#include "command.hpp"
#include "ability.hpp"
#include "activity.hpp"
#include "animation.hpp"
#include "audio.hpp"
#include "buff.hpp"
#include "building.hpp"
#include "calc.hpp"
#include "card.hpp"
#include "casino.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "config.hpp"
#include "crafting.hpp"
#include "ctrl_file.hpp"
#include "db_item.hpp"
#include "dmgheal.hpp"
#include "draw.hpp"
#include "enchantment.hpp"
#include "food.hpp"
#include "fov.hpp"
#include "i18n.hpp"
#include "input.hpp"
#include "item.hpp"
#include "itemgen.hpp"
#include "macro.hpp"
#include "map.hpp"
#include "map_cell.hpp"
#include "mef.hpp"
#include "menu.hpp"
#include "network.hpp"
#include "quest.hpp"
#include "random.hpp"
#include "shop.hpp"
#include "snail/application.hpp"
#include "ui.hpp"
#include "variables.hpp"

namespace elona
{

// TODO organize by order in pc_turn()

turn_result_t do_give_command()
{
    txt(i18n::s.get("core.locale.action.which_direction.default"));
    update_screen();
    int stat = ask_direction();
    if (stat == 0)
    {
        txt(i18n::_(u8"ui", u8"invalid_target"));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    tc = map(x, y, 1);
    if (tc == 0)
    {
        txt(i18n::_(u8"ui", u8"invalid_target"));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    tc -= 1;
    if (tc == 0)
    {
        if (gdata_mount != 0)
        {
            tc = gdata_mount;
        }
    }
    if (tc != 0)
    {
        if (tc < 16)
        {
            if (!cdata[tc].is_escorted()
                && !cdata[tc].is_escorted_in_sub_quest())
            {
                return try_interact_with_npc();
            }
        }
        update_screen();
        invctrl = 10;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    txt(i18n::_(u8"ui", u8"invalid_target"));
    update_screen();
    return turn_result_t::pc_turn_user_error;
}

turn_result_t do_interact_command()
{
    txt(i18n::s.get("core.locale.action.interact.choose"));
    int stat = ask_direction();
    if (stat == 0)
    {
        txt(i18n::_(u8"ui", u8"invalid_target"));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    tc = map(x, y, 1);
    if (tc == 0)
    {
        txt(i18n::_(u8"ui", u8"invalid_target"));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    tc -= 1;
    txt(i18n::s.get("core.locale.action.interact.prompt", cdata[tc]));
    p = 0;
    if (tc != 0)
    {
        if (cdata.player().confused == 0)
        {
            ELONA_APPEND_PROMPT(
                i18n::s.get("core.locale.action.interact.choices.talk"),
                u8"null"s,
                ""s + 0);
            ELONA_APPEND_PROMPT(
                i18n::s.get("core.locale.action.interact.choices.attack"),
                u8"null"s,
                ""s + 1);
        }
        if (cdata[tc].is_escorted() == 0)
        {
            if (cdata[tc].is_escorted_in_sub_quest() == 0)
            {
                if (tc < 16)
                {
                    ELONA_APPEND_PROMPT(
                        i18n::s.get(
                            "core.locale.action.interact.choices.inventory"),
                        u8"null"s,
                        ""s + 4);
                }
                else
                {
                    ELONA_APPEND_PROMPT(
                        i18n::s.get("core.locale.action.interact.choices.give"),
                        u8"null"s,
                        ""s + 2);
                }
                if (cdata[tc].is_livestock() == 1)
                {
                    ELONA_APPEND_PROMPT(
                        i18n::s.get(
                            "core.locale.action.interact.choices.bring_out"),
                        u8"null"s,
                        ""s + 5);
                }
                if (tc < 16)
                {
                    ELONA_APPEND_PROMPT(
                        i18n::s.get(
                            "core.locale.action.interact.choices.appearance"),
                        u8"null"s,
                        ""s + 8);
                }
            }
        }
        ELONA_APPEND_PROMPT(
            i18n::s.get("core.locale.action.interact.choices.teach_words"),
            u8"null"s,
            ""s + 7);
        ELONA_APPEND_PROMPT(
            i18n::s.get("core.locale.action.interact.choices.change_tone"),
            u8"null"s,
            ""s + 10);
        if (gdata_current_map != mdata_t::map_id_t::show_house)
        {
            if (cdata[tc].is_hung_on_sand_bag())
            {
                ELONA_APPEND_PROMPT(
                    i18n::s.get("core.locale.action.interact.choices.release"),
                    u8"null"s,
                    ""s + 9);
            }
        }
    }
    ELONA_APPEND_PROMPT(
        i18n::s.get("core.locale.action.interact.choices.name"),
        u8"null"s,
        ""s + 3);
    if (0 || gdata_wizard)
    {
        ELONA_APPEND_PROMPT(
            i18n::s.get("core.locale.action.interact.choices.info"),
            u8"null"s,
            ""s + 6);
    }
    {
        int stat = show_prompt(promptx, prompty, 200);
        if (stat == -1)
        {
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        rtval = stat;
    }
    screenupdate = -1;
    p = rtval;
    if (p == 0)
    {
        update_screen();
        talk_to_npc();
        if (chatteleport == 1)
        {
            chatteleport = 0;
            return turn_result_t::exit_map;
        }
        else
        {
            return turn_result_t::turn_end;
        }
    }
    if (p == 1)
    {
        update_screen();
        try_to_melee_attack();
        return turn_result_t::turn_end;
    }
    if (p == 2)
    {
        update_screen();
        invctrl = 10;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    if (p == 3)
    {
        update_screen();
        return call_npc();
    }
    if (p == 4)
    {
        return try_interact_with_npc();
    }
    if (p == 5)
    {
        rc = tc;
        new_ally_joins();
        update_screen();
        return turn_result_t::turn_end;
    }
    if (p == 6)
    {
        csctrl = 4;
        snd(26);
        cc = tc;
        menu_character_sheet();
        cc = 0;
        return turn_result_t::pc_turn_user_error;
    }
    if (p == 7)
    {
        txt(i18n::s.get(
            "core.locale.action.interact.change_tone.prompt", cdata[tc]));
        inputlog = "";
        input_text_dialog((windoww - 360) / 2 + inf_screenx, winposy(90), 20);
        cdata[tc].has_custom_talk() = false;
        if (inputlog == ""s)
        {
            cdatan(4, tc) = "";
        }
        else
        {
            cdatan(4, tc) = inputlog;
            txtef(9);
            txt(""s + cdatan(4, tc));
        }
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    if (p == 8)
    {
        gsel(4);
        pos(0, 0);
        picload(filesystem::dir::graphic() / u8"face1.bmp", 1);
        gsel(0);
        ccbk = cc;
        cc = tc;
        change_appearance();
        cc = ccbk;
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    if (p == 9)
    {
        snd(58);
        cdata[tc].is_hung_on_sand_bag() = false;
        txt(i18n::s.get("core.locale.action.interact.release", cdata[tc]));
        flt();
        itemcreate(-1, 733, cdata[tc].position.x, cdata[tc].position.y, 0);
    }
    if (p == 10)
    {
        change_npc_tone();
    }
    update_screen();
    return turn_result_t::pc_turn_user_error;
}


turn_result_t call_npc()
{
    txt(i18n::s.get("core.locale.action.interact.name.prompt", cdata[tc]));
    inputlog = "";
    input_text_dialog((windoww - 220) / 2 + inf_screenx, winposy(90), 12);
    if (inputlog == ""s)
    {
        txt(i18n::s.get("core.locale.action.interact.name.cancel"));
    }
    else
    {
        cdatan(0, tc) = ""s + inputlog;
        cdata[tc].has_own_name() = true;
        txt(i18n::s.get(
            "core.locale.action.interact.name.you_named", cdata[tc]));
    }
    gmode(2);
    update_screen();
    return turn_result_t::pc_turn_user_error;
}


turn_result_t do_bash_command()
{
    txt(i18n::s.get("core.locale.action.bash.prompt"));
    int stat = ask_direction();
    if (stat == 0)
    {
        txt(i18n::s.get("core.locale.common.it_is_impossible"));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    return do_bash();
}

turn_result_t do_dig_command()
{
    txt(i18n::s.get("core.locale.action.dig.prompt"));
    int stat = ask_direction();
    if (stat == 0)
    {
        txt(i18n::s.get("core.locale.common.it_is_impossible"));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    refx = x;
    refy = y;
    tlocx = x;
    tlocy = y;
    if (tlocx == cdata.player().position.x)
    {
        if (tlocy == cdata.player().position.y)
        {
            rowactre = 0;
            spot_digging();
            return turn_result_t::turn_end;
        }
    }
    if ((chipm(7, map(x, y, 0)) & 4) == 0 || chipm(0, map(x, y, 0)) == 3
        || mdata_map_type == mdata_t::map_type_t::world_map)
    {
        txt(i18n::s.get("core.locale.common.it_is_impossible"));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    screenupdate = -1;
    update_screen();
    return do_dig_after_sp_check();
}

turn_result_t do_search_command()
{
    ++msgdup;
    txt(i18n::s.get("core.locale.action.search.execute"));
    if (gdata_current_map == mdata_t::map_id_t::show_house)
    {
        p = 9999;
        for (const auto& cnt : items(-1))
        {
            if (inv[cnt].number() == 0)
            {
                continue;
            }
            if (inv[cnt].own_state != 5)
            {
                continue;
            }
            if (inv[cnt].id != 748)
            {
                continue;
            }
            if (p > dist(
                        inv[cnt].position.x,
                        inv[cnt].position.y,
                        cdata.player().position.x,
                        cdata.player().position.y))
            {
                p = dist(
                    inv[cnt].position.x,
                    inv[cnt].position.y,
                    cdata.player().position.x,
                    cdata.player().position.y);
            }
        }
        if (p != 9999)
        {
            while (1)
            {
                if (p <= 3)
                {
                    txt(i18n::s.get("core.locale.action.search.crystal.close"));
                }
                if (p <= 9)
                {
                    txt(i18n::s.get(
                        "core.locale.action.search.crystal.normal"));
                    break;
                }
                if (p <= 16)
                {
                    txt(i18n::s.get("core.locale.action.search.crystal.far"));
                    break;
                }
                txt(i18n::s.get("core.locale.action.search.crystal.sense"));
                break;
            }
        }
    }
    for (int cnt = 0; cnt < 11; ++cnt)
    {
        y = cdata[cc].position.y + cnt - 5;
        if (y < 0 || y >= mdata_map_height)
        {
            continue;
        }
        for (int cnt = 0; cnt < 11; ++cnt)
        {
            x = cdata[cc].position.x + cnt - 5;
            if (x < 0 || x >= mdata_map_width)
            {
                continue;
            }
            if (map(x, y, 6) != 0)
            {
                cell_featread(x, y);
                refx = x;
                refy = y;
                if (std::abs(cdata[cc].position.y - y) <= 1
                    && std::abs(cdata[cc].position.x - x) <= 1)
                {
                    if (feat(1) == 14)
                    {
                        if (feat(0) == 0)
                        {
                            int stat = try_to_reveal();
                            if (stat == 1)
                            {
                                discover_trap();
                                txt(i18n::s.get(
                                    "core.locale.action.search.discover.trap"));
                            }
                        }
                    }
                    if (feat(1) == 22)
                    {
                        int stat = try_to_reveal();
                        if (stat == 1 || 0)
                        {
                            discover_hidden_path();
                            txt(
                                i18n::s.get("core.locale.action.search."
                                            "discover.hidden_path"));
                        }
                    }
                }
                if (feat(1) == 32)
                {
                    if (gdata_current_map != mdata_t::map_id_t::show_house)
                    {
                        if (cdata[cc].position.x == x
                            && cdata[cc].position.y == y)
                        {
                            snd(24);
                            txt(i18n::s.get(
                                "core.locale.action.search.small_coin.find"));
                            map(x, y, 6) = 0;
                            flt();
                            itemcreate(-1, 622, x, y, 0);
                        }
                        else
                        {
                            if (dist(
                                    cdata[cc].position.x,
                                    cdata[cc].position.y,
                                    x,
                                    y)
                                > 2)
                            {
                                txt(
                                    i18n::s.get("core.locale.action.search."
                                                "small_coin.far"));
                            }
                            else
                            {
                                txt(
                                    i18n::s.get("core.locale.action.search."
                                                "small_coin.close"));
                            }
                        }
                    }
                }
            }
        }
    }
    cell_featread(cdata[cc].position.x, cdata[cc].position.y);
    if (feat(1) == 14)
    {
        if (feat(0) == tile_trap)
        {
            if (cdata.player().god_id == core_god::mani)
            {
                if (cc == 0)
                {
                    movx = cdata[cc].position.x;
                    movy = cdata[cc].position.y;
                    disarm_trap();
                }
            }
        }
    }
    if (feat(1) >= 24 && feat(1) <= 28)
    {
        rowactre(0) = 1;
        rowactre(1) = cdata[cc].position.x;
        rowactre(2) = cdata[cc].position.y;
        if (feat(1) == 24)
        {
            spot_digging();
        }
        if (feat(1) == 27)
        {
            spot_digging();
        }
        if (feat(1) == 26)
        {
            spot_fishing();
        }
        if (feat(1) == 25)
        {
            spot_mining_or_wall();
        }
        if (feat(1) == 28)
        {
            spot_material();
        }
    }
    return turn_result_t::turn_end;
}

turn_result_t do_pray_command()
{
    int stat = item_find(60002);
    if (stat != -1)
    {
        ci = stat;
        int god_id_int = inv[ci].param1;
        if (core_god::int2godid(god_id_int) != cdata.player().god_id)
        {
            begin_to_believe_god(god_id_int);
            return turn_result_t::turn_end;
        }
    }
    return do_pray();
}

turn_result_t do_throw_command()
{
    int ccthrowpotion = 0;
    if (is_in_fov(cdata[cc]))
    {
        txt(i18n::s.get(
            "core.locale.action.throw.execute", cdata[cc], inv[ci]));
    }
    if (dist(cdata[cc].position.x, cdata[cc].position.y, tlocx, tlocy) * 4
            > rnd(sdata(111, cc) + 10) + sdata(111, cc) / 4
        || rnd(10) == 0)
    {
        x = tlocx + rnd(2) - rnd(2);
        y = tlocy + rnd(2) - rnd(2);
        if (x >= 0)
        {
            if (y >= 0)
            {
                if (x < mdata_map_width)
                {
                    if (y < mdata_map_height)
                    {
                        if ((chipm(7, map(x, y, 0)) & 4) == 0)
                        {
                            tlocx = x;
                            tlocy = y;
                        }
                    }
                }
            }
        }
    }
    throwing_object_animation(
        {tlocx, tlocy}, cdata[cc].position, inv[ci].image, inv[ci].color)
        .play();
    ti = inv_getfreeid(-1);
    inv[ci].modify_number(-1);
    if (inv[ci].id == 685)
    {
        if (ti != -1)
        {
            item_copy(ci, ti);
            inv[ti].set_number(1);
            inv[ti].position.x = tlocx;
            inv[ti].position.y = tlocy;
            ci = ti;
        }
    }
    if (cc == 0)
    {
        refresh_burden_state();
    }
    x = tlocx;
    y = tlocy;
    breaking_animation({x, y}).play();
    if (inv[ci].id == 685 || inv[ci].id == 699)
    {
        snd(91);
        cell_refresh(inv[ci].position.x, inv[ci].position.y);
        if (map(tlocx, tlocy, 1) != 0)
        {
            tc = map(tlocx, tlocy, 1) - 1;
            txt(i18n::s.get("core.locale.action.throw.hits", cdata[tc]));
            if (inv[ci].id == 685)
            {
                if (tc < ELONA_MAX_PARTY_CHARACTERS
                    || cdata[tc].character_role != 0 || cdata[tc].quality == 6
                    || cdata[tc].is_lord_of_dungeon() == 1)
                {
                    txt(
                        i18n::s.get("core.locale.action.throw.monster_ball."
                                    "cannot_be_captured"));
                    return turn_result_t::turn_end;
                }
                if (cdata[tc].level > inv[ci].param2)
                {
                    txt(
                        i18n::s.get("core.locale.action.throw.monster_ball.not_"
                                    "enough_power"));
                    return turn_result_t::turn_end;
                }
                if (cdata[tc].hp > cdata[tc].max_hp / 10)
                {
                    txt(
                        i18n::s.get("core.locale.action.throw.monster_ball.not_"
                                    "weak_enough"));
                    return turn_result_t::turn_end;
                }
                txtef(2);
                txt(i18n::s.get(
                    "core.locale.action.throw.monster_ball.capture",
                    cdata[tc]));
                animeload(8, tc);
                inv[ci].subname = cdata[tc].id;
                inv[ci].param3 = cdata[tc].level;
                inv[ci].weight = clamp(cdata[tc].weight, 10000, 100000);
                inv[ci].value = 1000;
            }
            else
            {
                if (cdata[tc].id != 319 || tc < 16)
                {
                    txt(i18n::s.get("core.locale.common.nothing_happens"));
                    return turn_result_t::turn_end;
                }
                if (gdata_current_map == mdata_t::map_id_t::arena
                    || gdata_current_map == mdata_t::map_id_t::pet_arena
                    || gdata_current_map == mdata_t::map_id_t::show_house)
                {
                    txt(i18n::s.get(
                        "core.locale.action.throw.monster_ball.does_not_work"));
                    return turn_result_t::turn_end;
                }
                rc = tc;
                new_ally_joins();
            }
            chara_vanquish(tc);
            quest_check();
        }
        return turn_result_t::turn_end;
    }
    if (the_item_db[inv[ci].id]->category == 52000 || inv[ci].id == 772)
    {
        if (inv[ci].id != 601)
        {
            if (is_in_fov({tlocx, tlocy}))
            {
                if (inv[ci].id == 587)
                {
                    snd(86);
                }
                else
                {
                    snd(47);
                }
            }
            if (map(tlocx, tlocy, 1) != 0)
            {
                tc = map(tlocx, tlocy, 1) - 1;
                if (is_in_fov(cdata[tc]))
                {
                    txt(i18n::s.get(
                        "core.locale.action.throw.hits", cdata[tc]));
                    wet(tc, 25);
                }
                rowact_check(tc);
                if (inv[ci].id == 587)
                {
                    if (is_in_fov(cdata[tc]))
                    {
                        if (tc != 0)
                        {
                            txtef(9);
                            txt(i18n::s.get_enum(
                                "core.locale.action.throw.snow.dialog",
                                rnd(6)));
                        }
                    }
                    return turn_result_t::turn_end;
                }
                if (inv[ci].id == 772)
                {
                    if (is_in_fov(cdata[tc]))
                    {
                        txtef(4);
                        txt(i18n::s.get("core.locale.action.throw.tomato"));
                    }
                    if (inv[ci].param3 == -1)
                    {
                        if (is_in_fov(cdata[tc]))
                        {
                            txtef(4);
                            txt(i18n::s.get(
                                "core.locale.damage.is_engulfed_in_fury",
                                cdata[tc]));
                        }
                        cdata[tc].furious += rnd(10) + 5;
                    }
                    return turn_result_t::turn_end;
                }
                if (tc >= 16)
                {
                    hostileaction(cc, tc);
                }
                ccthrowpotion = cc;
                potionthrow = 100;
                cc = tc;
                dbid = inv[ci].id;
                access_item_db(15);
                cc = ccthrowpotion;
                return turn_result_t::turn_end;
            }
            if (inv[ci].id == 587)
            {
                if (map(tlocx, tlocy, 4) != 0)
                {
                    cell_itemlist(tlocx, tlocy);
                    f = 0;
                    for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
                    {
                        p = list(0, cnt);
                        if (inv[p].id == 541)
                        {
                            if (is_in_fov({tlocx, tlocy}))
                            {
                                txt(i18n::s.get(
                                    "core.locale.action.throw.snow.hits_"
                                    "snowman",
                                    inv[p(0)]));
                            }
                            inv[p].modify_number(-1);
                            f = 1;
                            break;
                        }
                    }
                    if (f == 1)
                    {
                        cell_refresh(tlocx, tlocy);
                        return turn_result_t::turn_end;
                    }
                }
            }
            if (inv[ci].id == 587)
            {
                if (chipm(0, map(tlocx, tlocy, 0)) == 4)
                {
                    return turn_result_t::turn_end;
                }
                if (is_in_fov({tlocx, tlocy}))
                {
                    txt(i18n::s.get("core.locale.action.throw.snow.melts"));
                }
            }
            else if (is_in_fov({tlocx, tlocy}))
            {
                txt(i18n::s.get("core.locale.action.throw.shatters"));
            }
            if (inv[ci].id == 772)
            {
                if (is_in_fov({tlocx, tlocy}))
                {
                    txtef(4);
                    txt(i18n::s.get("core.locale.action.throw.tomato"));
                }
                return turn_result_t::turn_end;
            }
            efp = 50 + sdata(111, cc) * 10;
            if (inv[ci].id == 392)
            {
                mef_add(tlocx, tlocy, 3, 19, rnd(15) + 5, efp, cc);
                return turn_result_t::turn_end;
            }
            if (inv[ci].id == 577)
            {
                mef_add(tlocx, tlocy, 5, 24, rnd(15) + 25, efp, cc);
                mapitem_fire(tlocx, tlocy);
                return turn_result_t::turn_end;
            }
            mef_add(
                tlocx,
                tlocy,
                6,
                27,
                -1,
                efp,
                cc,
                inv[ci].id,
                static_cast<int>(inv[ci].curse_state), // TODO
                inv[ci].color);
            return turn_result_t::turn_end;
        }
    }
    if (is_in_fov({tlocx, tlocy}))
    {
        txt(i18n::s.get("core.locale.action.throw.shatters"));
        snd(47);
    }
    if (inv[ci].id == 578)
    {
        flt();
        itemcreate(-1, 54, tlocx, tlocy, inv[ci].param1);
    }
    return turn_result_t::turn_end;
}

turn_result_t do_close_command()
{
    int stat = ask_direction_to_close();
    if (stat == 0)
    {
        txt(i18n::s.get("core.locale.common.it_is_impossible"));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    cell_featread(x, y);
    if (feat(1) != 20)
    {
        txt(i18n::s.get("core.locale.action.close.nothing_to_close"));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    if (map(x, y, 1) != 0)
    {
        txt(i18n::s.get("core.locale.action.close.blocked"));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    cell_featset(x, y, tile_doorclosed, 21, -1, -1);
    txt(i18n::s.get("core.locale.action.close.execute", cdata[cc]));
    return turn_result_t::turn_end;
}

turn_result_t do_change_ammo_command()
{
    f = 0;
    for (int cnt = 0; cnt < 30; ++cnt)
    {
        body = 100 + cnt;
        if (cdata[cc].body_parts[cnt] % 10000 == 0)
        {
            continue;
        }
        if (cdata[cc].body_parts[cnt] / 10000 == 11)
        {
            ci = cdata[cc].body_parts[cnt] % 10000 - 1;
            f = 1;
            break;
        }
    }
    if (f == 0)
    {
        txt(i18n::s.get("core.locale.action.ammo.need_to_equip"));
        return turn_result_t::pc_turn_user_error;
    }
    listmax = 0;
    cs = -1;
    for (int cnt = 0; cnt < 15; ++cnt)
    {
        if (inv[ci].enchantments[cnt].id == 0)
        {
            break;
        }
        enc = inv[ci].enchantments[cnt].id;
        i = enc / 10000;
        if (i != 0)
        {
            enc = enc % 10000;
            if (i == 9)
            {
                if (inv[ci].count == cnt)
                {
                    cs = cnt;
                }
                list(0, listmax) = cnt;
                ++listmax;
                continue;
            }
        }
    }
    if (listmax == 0)
    {
        inv[ci].count = -1;
        txt(i18n::s.get("core.locale.action.ammo.is_not_capable", inv[ci]));
        return turn_result_t::pc_turn_user_error;
    }
    snd(90);
    ++cs;
    if (cs >= listmax)
    {
        inv[ci].count = -1;
    }
    else
    {
        inv[ci].count = list(0, cs);
    }
    txt(i18n::s.get("core.locale.action.ammo.current") + ":");
    for (int cnt = 0, cnt_end = (listmax + 1); cnt < cnt_end; ++cnt)
    {
        if (cnt == 0)
        {
            s(0) = i18n::s.get("core.locale.action.ammo.normal");
            s(1) = i18n::s.get("core.locale.action.ammo.unlimited");
        }
        else
        {
            p = list(0, cnt - 1);
            i(0) = inv[ci].enchantments[p].power % 1000;
            i(1) = inv[ci].enchantments[p].power / 1000;
            s(0) = ammoname(inv[ci].enchantments[p].id % 10000);
            s(1) = ""s + i + u8"/"s + i(1);
        }
        s = s + u8":"s + s(1);
        if (inv[ci].count == cnt - 1)
        {
            s = u8"["s + s + u8"]"s;
            txtef(4);
        }
        else
        {
            s = u8" "s + s + u8" "s;
        }
        txt(u8" "s + s);
    }
    return turn_result_t::pc_turn_user_error;
}

turn_result_t do_offer_command()
{
    if (cdata.player().god_id.empty())
    {
        txt(i18n::s.get("core.locale.action.offer.do_not_believe"));
        return turn_result_t::turn_end;
    }
    rowact_item(ci);
    item_separate(ci);
    txt(i18n::s.get(
        "core.locale.action.offer.execute",
        inv[ci],
        i18n::_(u8"god", cdata.player().god_id, u8"name")));
    snd(121);
    const auto tcbk = tc(0);
    tc = 0;
    bright_aura_animation(
        cdata[tc].position, bright_aura_animation::type_t::offering)
        .play();
    tc = tcbk;
    int stat = item_find(60002);
    if (stat != -1)
    {
        ti = stat;
    }
    else
    {
        return turn_result_t::turn_end;
    }
    if (inv[ci].id == 204)
    {
        i = clamp(inv[ci].weight / 200, 1, 50);
        if (inv[ci].param3 < 0)
        {
            i = 1;
        }
    }
    else
    {
        i = 25;
    }
    if (core_god::int2godid(inv[ti].param1) != cdata.player().god_id)
    {
        f = 0;
        if (inv[ti].param1 == 0)
        {
            f = 1;
            txt(i18n::s.get(
                "core.locale.action.offer.claim",
                i18n::_(u8"god", cdata.player().god_id, u8"name")));
        }
        else
        {
            txt(i18n::s.get(
                "core.locale.action.offer.take_over.attempt",
                i18n::_(u8"god", cdata.player().god_id, u8"name"),
                i18n::_(
                    u8"god", core_god::int2godid(inv[ti].param1), u8"name")));
            if (rnd(17) <= i)
            {
                f = 1;
            }
            else
            {
                f = 0;
            }
        }
        if (f == 1)
        {
            modpiety(i * 5);
            cdata.player().praying_point += i * 30;
            animode = 100;
            miracle_animation().play();
            snd(120);
            if (inv[ti].param1 != 0)
            {
                txt(i18n::s.get("core.locale.action.offer.take_over.shadow"));
            }
            txtef(5);
            txt(i18n::s.get(
                "core.locale.action.offer.take_over.succeed",
                i18n::_(u8"god", cdata.player().god_id, u8"name"),
                inv[ti]));
            txtgod(cdata.player().god_id, 2);
            inv[ti].param1 = core_god::godid2int(cdata.player().god_id);
        }
        else
        {
            txt(i18n::s.get(
                "core.locale.action.offer.take_over.fail",
                i18n::_(u8"god", core_god::int2godid(inv[ti].param1))));
            txtgod(core_god::int2godid(inv[ti].param1), 3);
            god_fail_to_take_over_penalty();
        }
    }
    else
    {
        txtef(2);
        for (int cnt = 0; cnt < 1; ++cnt)
        {
            if (i >= 15)
            {
                txt(i18n::s.get(
                    "core.locale.action.offer.result.best", inv[ci]));
                txtgod(cdata.player().god_id, 4);
                break;
            }
            if (i >= 10)
            {
                txt(i18n::s.get(
                    "core.locale.action.offer.result.good", inv[ci]));
                break;
            }
            if (i >= 5)
            {
                txt(i18n::s.get(
                    "core.locale.action.offer.result.okay", inv[ci]));
                break;
            }
            if (i >= 1)
            {
                txt(i18n::s.get(
                    "core.locale.action.offer.result.poor", inv[ci]));
                break;
            }
        }
        modpiety(i);
        cdata.player().praying_point += i * 7;
    }
    inv[ci].modify_number((-inv[ci].number()));
    return turn_result_t::turn_end;
}

turn_result_t do_look_command()
{
    page = 0;
    pagesize = 16;
    cs_bk = -1;
    build_target_list();
    if (listmax == 0)
    {
        ++msgdup;
        txt(i18n::s.get("core.locale.action.look.find_nothing"));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    cs = 0;
    for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
    {
        if (list(0, cnt) == cdata.player().enemy_id)
        {
            cs = cnt % pagesize;
            page = cnt / pagesize;
        }
    }
label_1952_internal:
    cs_bk = -1;
    pagemax = (listmax - 1) / pagesize;
    if (page < 0)
    {
        page = pagemax;
    }
    else if (page > pagemax)
    {
        page = 0;
    }
label_1953_internal:
    if (cs != cs_bk)
    {
        screenupdate = -1;
        update_screen();
        keyrange = 0;
        font(20 - en * 2, snail::font_t::style_t::bold);
        for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
        {
            p = pagesize * page + cnt;
            if (p >= listmax)
            {
                break;
            }
            key_list(cnt) = key_select(cnt);
            ++keyrange;
            x = list(1, p) - scx;
            y = list(2, p) - scy;
            if (cs == cnt)
            {
                i = p;
                get_route(
                    cdata[cc].position.x,
                    cdata[cc].position.y,
                    cdata[list(0, p)].position.x,
                    cdata[list(0, p)].position.y);
                dx = (tlocx - scx) * inf_tiles + inf_screenx;
                dy = (tlocy - scy) * inf_tiles + inf_screeny;
                if (maxroute != 0)
                {
                    dx = cdata[cc].position.x;
                    dy = cdata[cc].position.y;
                    for (int cnt = 0; cnt < 100; ++cnt)
                    {
                        int stat = route_info(dx, dy, cnt);
                        if (stat == 0)
                        {
                            break;
                        }
                        else if (stat == -1)
                        {
                            continue;
                        }
                        sx = (dx - scx) * inf_tiles + inf_screenx;
                        sy = (dy - scy) * inf_tiles + inf_screeny;
                        if (sy + inf_tiles <= windowh - inf_verh)
                        {
                            pos(sx, sy * (sy > 0));
                            snail::application::instance()
                                .get_renderer()
                                .set_blend_mode(snail::blend_mode_t::blend);
                            snail::application::instance()
                                .get_renderer()
                                .set_draw_color({255, 255, 255, 25});
                            snail::application::instance()
                                .get_renderer()
                                .fill_rect(
                                    sx,
                                    sy * (sy > 0),
                                    inf_tiles
                                        - (sx + inf_tiles > windoww)
                                            * (sx + inf_tiles - windoww),
                                    inf_tiles + (sy < 0) * inf_screeny
                                        - (sy + inf_tiles > windowh - inf_verh)
                                            * (sy + inf_tiles - windowh
                                               + inf_verh));
                        }
                    }
                }
                sx = x * inf_tiles + inf_screenx;
                sy = y * inf_tiles + inf_screeny;
                if (sy + inf_tiles <= windowh - inf_verh)
                {
                    pos(sx, sy * (sy > 0));
                    snail::application::instance()
                        .get_renderer()
                        .set_blend_mode(snail::blend_mode_t::blend);
                    snail::application::instance()
                        .get_renderer()
                        .set_draw_color({127, 127, 255, 50});
                    snail::application::instance().get_renderer().fill_rect(
                        sx,
                        sy * (sy > 0),
                        inf_tiles,
                        inf_tiles + (sy < 0) * inf_screeny
                            - (sy + inf_tiles > windowh - inf_verh)
                                * (sy + inf_tiles - windowh + inf_verh));
                }
            }
            display_key(
                x * inf_tiles + inf_screenx - 12,
                y * inf_tiles + inf_screeny - 12,
                cnt);
        }
        color(0, 0, 0);
        txttargetnpc(
            cdata[list(0, i)].position.x, cdata[list(0, i)].position.y);
        cs_bk = cs;
        render_hud();
        redraw();
    }
    await(config::instance().wait1);
    key_check();
    cursor_check();
    if (key == key_target)
    {
        key = key_list(cs);
    }
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (p != -1)
    {
        cdata.player().enemy_id = p;
        snd(20);
        txt(i18n::s.get("core.locale.action.look.target", cdata[p(0)]));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_1952_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_1952_internal;
        }
    }
    if (key == key_cancel)
    {
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    goto label_1953_internal;
}

turn_result_t do_dip_command()
{
    if (inv[cidip].id == 617)
    {
        item_separate(ci);
        inv[cidip].modify_number(-1);
        snd(13);
        txt(i18n::s.get(
            "core.locale.action.dip.result.bait_attachment",
            inv[ci],
            inv[cidip]));
        if (inv[ci].param4 == inv[cidip].param1)
        {
            inv[ci].count += rnd(10) + 15;
        }
        else
        {
            inv[ci].count = rnd(10) + 15;
            inv[ci].param4 = inv[cidip].param1;
        }
        return turn_result_t::turn_end;
    }
    snd(17);
    if (the_item_db[inv[cidip].id]->category == 52000)
    {
        if (the_item_db[inv[ci].id]->subcategory == 60001)
        {
            item_separate(ci);
            inv[cidip].modify_number(-1);
            if (inv[cidip].id != 601)
            {
                txt(i18n::s.get(
                    "core.locale.action.dip.execute", inv[ci], inv[cidip]));
                if (inv[ci].id == 602)
                {
                    txt(i18n::s.get(
                        "core.locale.action.dip.result.holy_well_polluted"));
                    return turn_result_t::turn_end;
                }
                if (inv[ci].param3 >= 20)
                {
                    txt(i18n::s.get(
                        "core.locale.action.dip.result.well_dry", inv[ci]));
                    return turn_result_t::turn_end;
                }
                txt(i18n::s.get(
                    "core.locale.action.dip.result.well_refilled", inv[ci]));
                if (inv[cidip].id == 587)
                {
                    i18n::s.get("core.locale.action.dip.result.snow_melts.dip");
                }
                else
                {
                    inv[ci].param1 += rnd(3);
                }
                return turn_result_t::turn_end;
            }
            else
            {
                if (inv[ci].param1 < -5 || inv[ci].param3 >= 20
                    || (inv[ci].id == 602 && gdata_holy_well_count <= 0))
                {
                    txt(i18n::s.get(
                        "core.locale.action.dip.result.natural_potion_dry",
                        inv[ci]));
                    txt(i18n::s.get(
                        "core.locale.action.dip.result.natural_potion_drop"));
                    return turn_result_t::turn_end;
                }
                if (inv_getfreeid(0) == -1)
                {
                    txt(i18n::s.get(
                        "core.locale.ui.inv.common.inventory_is_full"));
                    return turn_result_t::turn_end;
                }
                if (inv[ci].id == 602)
                {
                    --gdata_holy_well_count;
                    flt();
                    int stat = itemcreate(0, 516, -1, -1, 0);
                    if (stat != 0)
                    {
                        inv[ci].curse_state = curse_state_t::blessed;
                    }
                }
                else
                {
                    inv[ci].param1 -= 3;
                    flt(20);
                    flttypemajor = 52000;
                    itemcreate(0, 0, -1, -1, 0);
                }
                txt(i18n::s.get(
                    "core.locale.action.dip.result.natural_potion"));
                txt(i18n::s.get("core.locale.action.dip.you_get", inv[ci]));
                item_stack(0, ci, 1);
                return turn_result_t::turn_end;
            }
        }
    }
    if (inv[cidip].id == 262)
    {
        if (the_item_db[inv[ci].id]->category == 57000)
        {
            inv[cidip].modify_number(-1);
            item_separate(ci);
            txt(i18n::s.get(
                    "core.locale.action.dip.result.love_food.made",
                    inv[ci],
                    inv[cidip])
                + i18n::s.get("core.locale.action.dip.result.love_food.grin"));
            if (is_cursed(inv[cidip].curse_state))
            {
                dipcursed(ci);
            }
            ibitmod(14, ci, 1);
            return turn_result_t::turn_end;
        }
    }
    if (inv[cidip].id == 620)
    {
        if (the_item_db[inv[ci].id]->category == 57000)
        {
            inv[cidip].modify_number(-1);
            item_separate(ci);
            txt(i18n::s.get(
                    "core.locale.action.dip.result.love_food.made",
                    inv[ci],
                    inv[cidip])
                + i18n::s.get(
                      "core.locale.action.dip.result.love_food.guilty"));
            if (is_cursed(inv[cidip].curse_state))
            {
                dipcursed(ci);
            }
            ibitmod(6, ci, 1);
            return turn_result_t::turn_end;
        }
    }
    if (inv[cidip].id == 519)
    {
        if (inv[cidip].curse_state == curse_state_t::blessed)
        {
            in = inv[cidip].number();
        }
        else
        {
            in = 1;
            item_separate(ci);
        }
        inv[cidip].modify_number(-1);
        inv[ci].color = inv[cidip].color;
        txt(i18n::s.get("core.locale.action.dip.result.dyeing", inv[ci]));
        if (inv_getowner(ci) == -1)
        {
            cell_refresh(inv[ci].position.x, inv[ci].position.y);
        }
        if (inv[ci].body_part != 0)
        {
            create_pcpic(cc, true);
        }
        return turn_result_t::turn_end;
    }
    if (inv[cidip].id == 566)
    {
        if (inv[cidip].curse_state == curse_state_t::blessed)
        {
            in = inv[cidip].number();
        }
        else
        {
            in = 1;
            item_separate(ci);
        }
        txt(i18n::s.get(
            "core.locale.action.dip.result.put_on", inv[ci], inv[cidip]));
        if (is_cursed(inv[cidip].curse_state))
        {
            dipcursed(ci);
        }
        else
        {
            ibitmod(1, ci, 1);
            txt(i18n::s.get(
                "core.locale.action.dip.result.gains_acidproof", inv[ci]));
        }
        inv[cidip].modify_number(-1);
        return turn_result_t::turn_end;
    }
    if (inv[cidip].id == 736)
    {
        if (inv[cidip].curse_state == curse_state_t::blessed)
        {
            in = inv[cidip].number();
        }
        else
        {
            in = 1;
            item_separate(ci);
        }
        txt(i18n::s.get(
            "core.locale.action.dip.result.put_on", inv[ci], inv[cidip]));
        if (is_cursed(inv[cidip].curse_state))
        {
            dipcursed(ci);
        }
        else if (inv[ci].id == 567)
        {
            txt(i18n::s.get("core.locale.action.dip.result.good_idea_but"));
        }
        else
        {
            ibitmod(2, ci, 1);
            txt(i18n::s.get(
                "core.locale.action.dip.result.gains_fireproof", inv[ci]));
        }
        inv[cidip].modify_number(-1);
        return turn_result_t::turn_end;
    }
    if (inv[cidip].id == 516)
    {
        inv[cidip].modify_number(-1);
        if (inv[cidip].curse_state == curse_state_t::blessed)
        {
            txtef(2);
            txt(i18n::s.get(
                "core.locale.action.dip.result.becomes_blessed", inv[ci]));
            inv[ci].curse_state = curse_state_t::blessed;
            chara_refresh(cc);
            return turn_result_t::turn_end;
        }
        if (is_cursed(inv[cidip].curse_state))
        {
            txtef(8);
            txt(i18n::s.get(
                "core.locale.action.dip.result.becomes_cursed", inv[ci]));
            inv[ci].curse_state = curse_state_t::cursed;
            chara_refresh(cc);
            return turn_result_t::turn_end;
        }
    }
    txt(i18n::s.get("core.locale.common.nothing_happens"));
    return turn_result_t::turn_end;
}

turn_result_t do_use_command()
{
    screenupdate = -1;
    update_screen();
    tc = cc;
    tlocx = cdata[cc].position.x;
    tlocy = cdata[cc].position.y;
    auto item_data = the_item_db[inv[ci].id];

    if (item_data->on_use_callback)
    {
        auto item_handle = lua::lua->get_handle_manager().get_handle(inv[ci]);
        auto user_handle = lua::lua->get_handle_manager().get_handle(cdata[cc]);

        assert(item_handle != sol::lua_nil);
        assert(user_handle != sol::lua_nil);

        bool success = lua::lua->get_export_manager().call_with_result(
            *item_data->on_use_callback, false, item_handle, user_handle);

        if (success)
        {
            return turn_result_t::turn_end;
        }
        else
        {
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
    }

    if (ibit(7, ci) == 1)
    {
        if (gdata_hour + gdata_day * 24 + gdata_month * 24 * 30
                + gdata_year * 24 * 30 * 12
            < inv[ci].count)
        {
            txt(i18n::s.get(
                "core.locale.action.use.useable_again_at",
                cnvdate(inv[ci].count)));
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        item_separate(ci);
        inv[ci].count = gdata_hour + gdata_day * 24 + gdata_month * 24 * 30
            + gdata_year * 24 * 30 * 12 + inv[ci].param3;
    }
    if (ibit(4, ci) == 1)
    {
        if (inv[ci].count <= 0)
        {
            txt(i18n::s.get("core.locale.action.use.out_of_charge"));
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        item_separate(ci);
        --inv[ci].count;
    }
    if (item_data->subcategory == 58500)
    {
        return do_plant();
    }
    if (item_data->subcategory == 59500)
    {
        return blending_menu();
    }
    if (item_data->subcategory == 60004)
    {
        if (gdata_continuous_active_hours < 15)
        {
            txt(i18n::s.get("core.locale.action.use.not_sleepy"));
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        gdata(91) = 100;
        continuous_action_others();
        return turn_result_t::turn_end;
    }
    if (inv[ci].id == 413 || inv[ci].id == 414)
    {
        return do_gatcha();
    }
    if (inv[ci].id == 312 || inv[ci].id == 313 || inv[ci].id == 314
        || inv[ci].id == 315)
    {
        atxid = 1;
        casino_dealer();
        return turn_result_t::turn_end;
    }
    if (inv[ci].function == 1 || inv[ci].function == 2 || inv[ci].function == 3
        || inv[ci].function == 4)
    {
        prodtype = inv[ci].function;
        snd(26);
        invctrl = 0;
        crafting_menu();
        return turn_result_t::turn_end;
    }
    if (ibit(10, ci))
    {
        if (inv[ci].param2 < calcexpalive(inv[ci].param1))
        {
            txt(i18n::s.get("core.locale.action.use.living.needs_more_blood"));
        }
        else
        {
            txtnew();
            txt(i18n::s.get(
                "core.locale.action.use.living.ready_to_grow", inv[ci]));
            randomize(inv[ci].subname);
            if (inv[ci].param1 >= 4 + rnd(12))
            {
                txt(i18n::s.get("core.locale.action.use.living.weird"));
            }
            txt(i18n::s.get("core.locale.action.use.living.it"));
            reftype = item_data->category;
            listmax = 0;
            for (int cnt = 0; cnt < 3; ++cnt)
            {
                randomize(inv[ci].subname + inv[ci].param1 * 10 + cnt);
                if (enchantment_add(
                        ci,
                        enchantment_generate(enchantment_gen_level(4)),
                        enchantment_gen_p(),
                        0,
                        0,
                        1))
                {
                    if (rtval == 34)
                    {
                        if (rnd(3))
                        {
                            continue;
                        }
                    }
                    list(0, listmax) = rtval;
                    list(1, listmax) = rtval(1);
                    get_enchantment_description(
                        list(0, listmax), list(1, listmax), 0);
                    ELONA_APPEND_PROMPT(s, u8"null"s, ""s + promptmax);
                    ++listmax;
                }
            }
            list(0, listmax) = -1;
            ++listmax;
            s = i18n::s.get("core.locale.action.use.living.bonus");
            ELONA_APPEND_PROMPT(s, u8"null"s, ""s + promptmax);
            rtval = show_prompt(promptx, prompty, 400);
            txtnew();
            if (rtval == -1)
            {
                txt(i18n::s.get(
                    "core.locale.action.use.living.displeased", inv[ci]));
            }
            else
            {
                if (list(0, rtval) == -1)
                {
                    ++inv[ci].enhancement;
                }
                else
                {
                    enchantment_add(ci, list(0, rtval), list(1, rtval), 0, 1);
                }
                txtef(2);
                txt(i18n::s.get(
                    "core.locale.action.use.living.pleased", inv[ci]));
                randomize(inv[ci].subname);
                if (inv[ci].param1 >= 4 + rnd(12))
                {
                    txt(i18n::s.get(
                        "core.locale.action.use.living.becoming_a_threat"));
                    if (enchantment_add(ci, 45, 50))
                    {
                        inv[ci].enchantments[14].id = 0;
                        txt(i18n::s.get(
                            "core.locale.action.use.living.removes_enchantment",
                            inv[ci]));
                    }
                }
                inv[ci].param2 = 0;
                ++inv[ci].param1;
            }
            randomize();
        }
        chara_refresh(cc);
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    switch (inv[ci].function)
    {
    case 24:
        x = cdata[cc].position.x;
        y = cdata[cc].position.y;
        if (mdata_map_type == mdata_t::map_type_t::world_map)
        {
            txt(i18n::s.get("core.locale.action.use.mine.cannot_use_here"));
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        if (map(x, y, 6) != 0)
        {
            txt(i18n::s.get("core.locale.action.use.mine.cannot_place_here"));
            return turn_result_t::pc_turn_user_error;
        }
        inv[ci].modify_number(-1);
        cell_featset(x, y, 0, 14, 7, cc);
        txt(i18n::s.get("core.locale.action.use.mine.you_set_up"));
        snd(58);
        goto label_2229_internal;
    case 44:
        if (inv_getowner(ci) != -1)
        {
            txt(i18n::s.get(
                "core.locale.action.use.chair.needs_place_on_ground"));
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        txt(i18n::s.get("core.locale.action.use.chair.you_sit_on", inv[ci]));
        ELONA_APPEND_PROMPT(
            i18n::s.get("core.locale.action.use.chair.choices.relax"),
            u8"null"s,
            ""s + 0);
        if (inv[ci].param1 != 1)
        {
            ELONA_APPEND_PROMPT(
                i18n::s.get("core.locale.action.use.chair.choices.my_chair"),
                u8"null"s,
                ""s + 1);
        }
        if (inv[ci].param1 != 2)
        {
            ELONA_APPEND_PROMPT(
                i18n::s.get("core.locale.action.use.chair.choices.guest_chair"),
                u8"null"s,
                ""s + 2);
        }
        if (inv[ci].param1 != 0)
        {
            ELONA_APPEND_PROMPT(
                i18n::s.get("core.locale.action.use.chair.choices.free_chair"),
                u8"null"s,
                ""s + 3);
        }
        {
            int stat = show_prompt(promptx, prompty, 260);
            if (stat == -1)
            {
                goto label_2229_internal;
            }
            rtval = stat;
        }
        if (rtval == 0)
        {
            i18n::s.get("core.locale.action.use.chair.relax");
            goto label_2229_internal;
        }
        if (rtval == 1)
        {
            i18n::s.get("core.locale.action.use.chair.my_chair", inv[ci]);
            inv[ci].param1 = 1;
            goto label_2229_internal;
        }
        if (rtval == 2)
        {
            i18n::s.get("core.locale.action.use.chair.guest_chair", inv[ci]);
            inv[ci].param1 = 2;
            goto label_2229_internal;
        }
        if (rtval == 3)
        {
            i18n::s.get("core.locale.action.use.chair.free_chair", inv[ci]);
            inv[ci].param1 = 0;
            goto label_2229_internal;
        }
        goto label_2229_internal;
    case 8:
        if (mdata_map_type != mdata_t::map_type_t::player_owned)
        {
            txt(i18n::s.get(
                "core.locale.action.use.house_board.cannot_use_it_here"));
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        screenupdate = -1;
        update_screen();
        return turn_result_t::show_house_board;
    case 19:
        txt(i18n::s.get("core.locale.action.use.dresser.prompt"));
        {
            int stat = ask_direction();
            f = 0;
            if (stat != 0)
            {
                if (map(x, y, 1) > 0)
                {
                    tc = map(x, y, 1) - 1;
                    if (tc < 16)
                    {
                        screenupdate = -1;
                        update_screen();
                        ccbk = cc;
                        cc = tc;
                        change_appearance_equipment();
                        cc = ccbk;
                        f = 1;
                    }
                }
            }
        }
        if (f == 0)
        {
            txt(i18n::s.get("core.locale.common.it_is_impossible"));
        }
        update_screen();
        return turn_result_t::pc_turn_user_error;
        break;
    case 15:
        efid = 184;
        magic();
        goto label_2229_internal;
    case 16:
        efid = 185;
        magic();
        goto label_2229_internal;
    case 17:
        efid = 183;
        magic();
        goto label_2229_internal;
    case 14:
        if (cc == 0)
        {
            if (inv[ci].number() < 5)
            {
                txt(i18n::s.get("core.locale.action.use.snow.need_more"));
                update_screen();
                return turn_result_t::pc_turn_user_error;
            }
            inv[ci].modify_number(-5);
        }
        flt();
        itemcreate(
            -1, 541, cdata.player().position.x, cdata.player().position.y, 0);
        if (is_in_fov(cdata[cc]))
        {
            snd(86);
            txt(i18n::s.get(
                "core.locale.action.use.snow.make_snowman", cdata[cc]));
        }
        goto label_2229_internal;
    case 13:
        snd(6);
        if (gdata_torch == 0)
        {
            gdata_torch = 1;
            txt(i18n::s.get("core.locale.action.use.torch.light"));
        }
        else
        {
            gdata_torch = 0;
            txt(i18n::s.get("core.locale.action.use.torch.put_out"));
        }
        chara_refresh(0);
        goto label_2229_internal;
    case 9:
    {
        int stat = read_textbook();
        if (stat == 1)
        {
            return turn_result_t::turn_end;
        }
        else
        {
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
    }
        goto label_2229_internal;
    case 5:
        txt(i18n::s.get("core.locale.action.use.stethoscope.prompt"));
        update_screen();
        {
            int stat = ask_direction();
            if (stat == 0)
            {
                txt(i18n::s.get("core.locale.common.it_is_impossible"));
                update_screen();
                return turn_result_t::pc_turn_user_error;
            }
        }
        tc = map(x, y, 1) - 1;
        if (tc == 0)
        {
            txt(i18n::s.get("core.locale.action.use.stethoscope.self"));
            gdata(94) = 0;
            return turn_result_t::turn_end;
        }
        if (tc > 0 && tc < 16)
        {
            if (cdata[tc].state() == character::state_t::alive)
            {
                gdata(94) = 0;
                if (cdata[tc].has_been_used_stethoscope() == 1)
                {
                    cdata[tc].has_been_used_stethoscope() = false;
                    txt(i18n::s.get(
                        "core.locale.action.use.stethoscope.other.stop",
                        cdata[tc]));
                    return turn_result_t::turn_end;
                }
                txt(i18n::s.get(
                    "core.locale.action.use.stethoscope.other.start.text",
                    cdata[tc]));
                if (cdata[tc].sex == 1)
                {
                    txt(i18n::s.get(
                        "core.locale.action.use.stethoscope.other.start.female."
                        "text",
                        cdata[tc]));
                    txtef(4);
                    txt(i18n::s.get(
                        "core.locale.action.use.stethoscope.other.start.female."
                        "dialog",
                        cdata[tc]));
                }
                cdata[tc].has_been_used_stethoscope() = true;
                return turn_result_t::turn_end;
            }
        }
        txt(i18n::s.get("core.locale.common.it_is_impossible"));
        update_screen();
        return turn_result_t::pc_turn_user_error;
        break;
    case 23:
        txt(i18n::s.get("core.locale.action.use.leash.prompt"));
        update_screen();
        {
            int stat = ask_direction();
            f = 0;
            if (stat != 0)
            {
                if (map(x, y, 1) > 0)
                {
                    tc = map(x, y, 1) - 1;
                    if (tc == 0)
                    {
                        txt(i18n::s.get("core.locale.action.use.leash.self"));
                    }
                    else if (cdata[tc].is_leashed() == 0)
                    {
                        if (tc >= 16)
                        {
                            if (rnd(5) == 0)
                            {
                                txt(i18n::s.get(
                                    "core.locale.action.use.leash.other.start."
                                    "resists",
                                    cdata[tc]));
                                inv[ci].modify_number(-1);
                                cell_refresh(
                                    inv[ci].position.x, inv[ci].position.y);
                                refresh_burden_state();
                                goto label_2229_internal;
                            }
                        }
                        cdata[tc].is_leashed() = true;
                        txt(i18n::s.get(
                            "core.locale.action.use.leash.other.start.text",
                            cdata[tc]));
                        txtef(9);
                        txt(i18n::s.get(
                            "core.locale.action.use.leash.other.start.dialog",
                            cdata[tc]));
                    }
                    else
                    {
                        cdata[tc].is_leashed() = false;
                        txt(i18n::s.get(
                            "core.locale.action.use.leash.other.stop.text",
                            cdata[tc]));
                        txtef(9);
                        txt(i18n::s.get(
                            "core.locale.action.use.leash.other.stop.dialog",
                            cdata[tc]));
                    }
                    animeload(8, tc);
                    f = 1;
                }
            }
        }
        if (f == 0)
        {
            txt(i18n::s.get("core.locale.common.it_is_impossible"));
        }
        goto label_2229_internal;
    case 45:
        if (gdata_current_map == mdata_t::map_id_t::show_house)
        {
            txt(i18n::s.get("core.locale.action.use.sandbag.cannot_use_here"));
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        txt(i18n::s.get("core.locale.action.use.sandbag.prompt"));
        update_screen();
        {
            int stat = ask_direction();
            f = 0;
            if (stat != 0)
            {
                if (map(x, y, 1) > 0)
                {
                    tc = map(x, y, 1) - 1;
                    if (cdata[tc].hp >= cdata[tc].max_hp / 5)
                    {
                        txt(i18n::s.get(
                            "core.locale.action.use.sandbag.not_weak_enough"));
                        return turn_result_t::pc_turn_user_error;
                    }
                    if (tc != 0)
                    {
                        if (tc < 16)
                        {
                            txt(i18n::s.get(
                                "core.locale.action.use.sandbag.ally"));
                            return turn_result_t::pc_turn_user_error;
                        }
                    }
                    if (cdata[tc].is_hung_on_sand_bag())
                    {
                        txt(i18n::s.get(
                            "core.locale.action.use.sandbag.already"));
                        return turn_result_t::pc_turn_user_error;
                    }
                    if (tc == 0)
                    {
                        txt(i18n::s.get("core.locale.action.use.sandbag.self"));
                    }
                    else
                    {
                        snd(58);
                        cdata[tc].is_hung_on_sand_bag() = true;
                        txt(i18n::s.get(
                            "core.locale.action.use.sandbag.start", cdata[tc]));
                        txt(i18n::s.get(
                            "core.locale.action.use.leash.other.start.dialog",
                            cdata[tc]));
                        animeload(8, tc);
                        inv[ci].modify_number(-1);
                        cell_refresh(inv[ci].position.x, inv[ci].position.y);
                        refresh_burden_state();
                    }
                    f = 1;
                }
            }
        }
        if (f == 0)
        {
            txt(i18n::s.get("core.locale.common.it_is_impossible"));
        }
        goto label_2229_internal;
    case 6:
    {
        txt(i18n::s.get("core.locale.action.use.music_disc.play", inv[ci]));
        auto music = inv[ci].param1 + 50 + 1;
        if (music > 97)
        {
            music = 97;
        }
        mdata_map_bgm = music;

        auto music_id = *the_music_db.get_id_from_legacy(music);
        assert(music_id);
        if (!music_id)
        {
            goto label_2229_internal;
        }
        play_music(*music_id);
    }
        goto label_2229_internal;
    case 10:
        screenupdate = -1;
        update_screen();
        return turn_result_t::play_scene;
    case 7:
        if (inv[ci].own_state != 3)
        {
            if (mdata_map_refresh_type == 0
                || gdata_current_map == mdata_t::map_id_t::quest
                || gdata_current_map == mdata_t::map_id_t::shelter_)
            {
                if (gdata_current_map == mdata_t::map_id_t::fields)
                {
                    txt(i18n::s.get(
                        "core.locale.action.use.shelter.only_in_world_map"));
                }
                else
                {
                    txt(i18n::s.get(
                        "core.locale.action.use.shelter.cannot_build_it_here"));
                }
                update_screen();
                return turn_result_t::pc_turn_user_error;
            }
            gdata(91) = 101;
            continuous_action_others();
            return turn_result_t::turn_end;
        }
        if (adata(16, gdata_current_map) == mdata_t::map_id_t::random_dungeon)
        {
            if (gdata_current_dungeon_level == adata(10, gdata_current_map))
            {
                if (adata(20, gdata_current_map) != -1)
                {
                    txt(i18n::s.get(
                        "core.locale.action.use.shelter.during_quest"));
                    ELONA_YES_NO_PROMPT();
                    rtval = show_prompt(promptx, prompty, 160);
                    if (rtval != 0)
                    {
                        update_screen();
                        return turn_result_t::pc_turn_user_error;
                    }
                }
            }
        }
        gdata(91) = 102;
        continuous_action_others();
        goto label_2229_internal;
    case 11:
        if (moneybox(inv[ci].param2) > cdata.player().gold)
        {
            txt(i18n::s.get(
                "core.locale.action.use.money_box.not_enough_gold"));
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        if (inv[ci].param1 >= 1000000000)
        {
            txt(i18n::s.get("core.locale.action.use.money_box.full"));
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        item_separate(ci);
        snd(12);
        cdata.player().gold -= moneybox(inv[ci].param2);
        inv[ci].param1 += moneybox(inv[ci].param2);
        inv[ci].weight += 100;
        goto label_2229_internal;
    case 20:
        efid = 458;
        efp = 400;
        magic();
        goto label_2229_internal;
    case 47:
        txt(i18n::s.get("core.locale.action.use.summoning_crystal.use"));
        goto label_2229_internal;
    case 22:
        snd(118);
        if (mdata_map_type != mdata_t::map_type_t::town
            && mdata_map_type != mdata_t::map_type_t::guild)
        {
            txt(i18n::s.get("core.locale.action.use.rune.only_in_town"));
            goto label_2229_internal;
        }
        inv[ci].modify_number(-1);
        cell_refresh(inv[ci].position.x, inv[ci].position.y);
        txt(i18n::s.get("core.locale.action.use.rune.use"));
        comctrl = 2;
        {
            int stat = query_for_showroom_to_visit();
            if (stat == 1)
            {
                return do_enter_strange_gate();
            }
        }
        goto label_2229_internal;
    case 49:
        txt(i18n::s.get("core.locale.action.use.hammer.use", inv[ci]));
        snd(58);
        efid = 49;
        efp = 100;
        magic();
        goto label_2229_internal;
    case 21:
        txt(i18n::s.get("core.locale.action.use.hammer.use", inv[ci]));
        snd(58);
        inv[ci].modify_number(-1);
        fixmaterial = inv[ci].material;
        efid = 21;
        efp = 500;
        magic();
        goto label_2229_internal;
    case 25:
        txt(i18n::s.get("core.locale.action.use.unicorn_horn.use", inv[ci]));
        inv[ci].modify_number(-1);
        efid = 637;
        efp = 500;
        magic();
        goto label_2229_internal;
    case 26:
        txt(i18n::s.get("core.locale.action.use.statue.activate", inv[ci]));
        gdata_diastrophism_flag = 1;
        snd(64);
        txtef(5);
        txt(i18n::s.get("core.locale.action.use.statue.opatos"));
        goto label_2229_internal;
    case 34:
        txt(i18n::s.get("core.locale.action.use.statue.activate", inv[ci]));
        txtef(5);
        txt(i18n::s.get("core.locale.action.use.statue.jure"));
        efid = 637;
        efp = 5000;
        magic();
        goto label_2229_internal;
    case 43:
        txt(i18n::s.get("core.locale.action.use.statue.activate", inv[ci]));
        snd(64);
        txtef(5);
        txt(i18n::s.get("core.locale.action.use.statue.ehekatl"));
        buff_add(cdata[tc], 19, 77, 2500);
        goto label_2229_internal;
    case 27:
        txt(i18n::s.get("core.locale.action.use.statue.activate", inv[ci]));
        snd(64);
        txtef(5);
        if (gdata_weather == 1)
        {
            txt(i18n::s.get(
                "core.locale.action.use.statue.lulwy.during_etherwind"));
            goto label_2229_internal;
        }
        p = gdata_weather;
        while (1)
        {
            if (rnd(10) == 0)
            {
                gdata_weather = 0;
            }
            if (rnd(10) == 0)
            {
                gdata_weather = 3;
            }
            if (rnd(15) == 0)
            {
                gdata_weather = 4;
            }
            if (rnd(20) == 0)
            {
                gdata_weather = 2;
            }
            if (gdata_weather != p)
            {
                break;
            }
        }
        txt(i18n::s.get("core.locale.action.use.statue.lulwy.normal"));
        txt(i18n::s.get("core.locale.action.weather.changes"));
        sound_play_environmental();
        goto label_2229_internal;
    case 28:
        if (mdata_map_type == mdata_t::map_type_t::world_map)
        {
            txt(i18n::s.get("core.locale.action.use.nuke.cannot_place_here"));
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        if (cdata.player().position.x != 33 || cdata.player().position.y != 16)
        {
            if (gdata_red_blossom_in_palmia == 1)
            {
                txt(i18n::s.get("core.locale.action.use.nuke.not_quest_goal"));
                ELONA_YES_NO_PROMPT();
                rtval = show_prompt(promptx, prompty, 160);
                if (rtval != 0)
                {
                    update_screen();
                    return turn_result_t::pc_turn_user_error;
                }
            }
        }
        inv[ci].modify_number(-1);
        cell_refresh(inv[ci].position.x, inv[ci].position.y);
        txt(i18n::s.get("core.locale.action.use.nuke.set_up"));
        snd(58);
        mef_add(
            cdata[cc].position.x, cdata[cc].position.y, 7, 632, 10, 100, cc);
        goto label_2229_internal;
    case 48:
        if (gdata_current_map != mdata_t::map_id_t::show_house
            || usermapid == 0)
        {
            txt(i18n::s.get("core.locale.action.use.statue.creator.normal"));
            goto label_2229_internal;
        }
        txt(i18n::s.get("core.locale.action.use.statue.creator.in_usermap"));
        goto label_2229_internal;
    case 29:
        trait(inv[ci].param1) = 1;
        if (inv[ci].param1 == 169)
        {
            trait(162) = 0;
        }
        if (inv[ci].param1 == 162)
        {
            trait(169) = 0;
        }
        inv[ci].modify_number(-1);
        txt(i18n::s.get("core.locale.action.use.secret_treasure.use"));
        animeload(10, 0);
        chara_refresh(cc);
        goto label_2229_internal;
    case 30:
        txt(i18n::s.get("core.locale.action.use.statue.activate", inv[ci]));
        efid = inv[ci].param1;
        efp = inv[ci].param2;
        tc = cc;
        efstatus = curse_state_t::none;
        magic();
        goto label_2229_internal;
    case 41:
        if (gdata_next_level_minus_one_kumiromis_experience_becomes_available
            > cdata.player().level)
        {
            txt(
                i18n::s.get("core.locale.action.use.secret_experience.kumiromi."
                            "not_enough_exp"));
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        snd(64);
        gdata_next_level_minus_one_kumiromis_experience_becomes_available += 10;
        inv[ci].modify_number(-1);
        ++gdata_acquirable_feat_count;
        txt(i18n::s.get(
            "core.locale.action.use.secret_experience.kumiromi.use.dialog"));
        txtef(5);
        txt(i18n::s.get(
            "core.locale.action.use.secret_experience.kumiromi.use.text"));
        goto label_2229_internal;
    case 42:
        snd(38);
        txtef(8);
        txt(i18n::s.get("core.locale.action.use.secret_experience.lomias"));
        goto label_2229_internal;
    case 46:
        txtnew();
        txt(i18n::s.get("core.locale.action.use.rope.prompt"));
        ELONA_YES_NO_PROMPT();
        rtval = show_prompt(promptx, prompty, 160);
        if (rtval != 0)
        {
            return turn_result_t::turn_end;
        }
        damage_hp(cdata.player(), 99999, -20);
        goto label_2229_internal;
    case 33:
        if (inv[ci].subname == 0)
        {
            txt(i18n::s.get("core.locale.action.use.monster_ball.empty"));
            goto label_2229_internal;
        }
        if (chara_get_free_slot_ally() == 0)
        {
            txt(i18n::s.get(
                "core.locale.action.use.monster_ball.party_is_full"));
            goto label_2229_internal;
        }
        txt(i18n::s.get("core.locale.action.use.monster_ball.use", inv[ci]));
        inv[ci].modify_number(-1);
        flt();
        novoidlv = 1;
        chara_create(56, inv[ci].subname, -3, 0);
        rc = 56;
        new_ally_joins();
        goto label_2229_internal;
    case 31:
        x = cdata[cc].position.x;
        y = cdata[cc].position.y;
        cell_featread(x, y);
        if (feat(1) != 29)
        {
            txt(i18n::s.get(
                "core.locale.action.use.gem_stone.kumiromi.no_plant"));
            goto label_2229_internal;
        }
        if (feat == tile_plant + 2)
        {
            txt(i18n::s.get(
                "core.locale.action.use.gem_stone.kumiromi.already_grown"));
            goto label_2229_internal;
        }
        if (feat == tile_plant + 3)
        {
            feat = tile_plant + 1;
            try_to_grow_plant();
            txt(i18n::s.get(
                "core.locale.action.use.gem_stone.kumiromi.revives"));
        }
        else
        {
            ++feat;
            txt(i18n::s.get("core.locale.action.use.gem_stone.kumiromi.grows"));
        }
        cell_featset(x, y, feat, feat(1), feat(2), feat(3));
        animeload(8, 0);
        goto label_2229_internal;
    case 32:
        txtnew();
        txt(i18n::s.get("core.locale.action.use.gene_machine.choose_original"));
        rc = 0;
        {
            int stat = ctrl_ally(ctrl_ally_operation::gene_engineer);
            if (stat == -1)
            {
                return turn_result_t::turn_end;
            }
            rc = stat;
        }
        txtnew();
        txt(i18n::s.get("core.locale.action.use.gene_machine.choose_subject"));
        {
            int stat = ctrl_ally(ctrl_ally_operation::gene_engineer);
            if (stat == -1)
            {
                return turn_result_t::turn_end;
            }
            tc = stat;
        }
        update_screen();
        txtnew();
        txt(i18n::s.get(
            "core.locale.action.use.gene_machine.prompt",
            cdata[tc],
            cdata[rc]));
        ELONA_YES_NO_PROMPT();
        rtval = show_prompt(promptx, prompty, 160);
        if (rtval != 0)
        {
            return turn_result_t::turn_end;
        }
        txtnew();
        txtef(5);
        txt(i18n::s.get(
            "core.locale.action.use.gene_machine.has_inherited",
            cdata[rc],
            cdata[tc]));
        gene_engineering_animation(cdata[rc].position).play();
        {
            int stat = transplant_body_parts();
            if (stat != -1)
            {
                cdata[rc].body_parts[stat - 100] = rtval * 10000;
                txtef(2);
                txt(i18n::s.get(
                    "core.locale.action.use.gene_machine.gains.body_part",
                    cdata[rc],
                    i18n::_(u8"ui", u8"body_part", u8"_"s + rtval)));
                refresh_speed_correction_value(cdata[rc]);
            }
        }
        {
            int stat = gain_skills_by_geen_engineering();
            if (stat != 0)
            {
                for (int cnt = 0; cnt < 2; ++cnt)
                {
                    if (rtval(cnt) == -1)
                    {
                        break;
                    }
                    chara_gain_skill(cdata[rc], rtval(cnt), 1);
                    txtef(2);
                    txt(i18n::s.get(
                        "core.locale.action.use.gene_machine.gains.ability",
                        cdata[rc],
                        i18n::_(
                            u8"ability",
                            std::to_string(rtval(cnt)),
                            u8"name")));
                }
            }
        }
        if (cdata[tc].level > cdata[rc].level)
        {
            lv = (cdata[tc].level - cdata[rc].level) / 2 + 1;
            for (int cnt = 0, cnt_end = (lv); cnt < cnt_end; ++cnt)
            {
                r2 = 1;
                gain_level(cdata[rc]);
            }
            txtef(2);
            txt(i18n::s.get(
                "core.locale.action.use.gene_machine.gains.level",
                cdata[rc],
                cdata[rc].level));
            listmax = 0;
            for (int cnt = 10; cnt < 18; ++cnt)
            {
                list(0, listmax) = cnt;
                list(1, listmax) = sdata.get(cnt, tc).original_level;
                ++listmax;
            }
            sort_list_by_column1();
            for (int cnt = 0; cnt < 3; ++cnt)
            {
                p = listmax - cnt - 1;
                i = list(0, p);
                if (list(1, p) > sdata.get(i, rc).original_level)
                {
                    p = (list(1, p) - sdata.get(i, rc).original_level) * 500;
                    p = clamp(p * 10 / clamp(lv, 2, 10), 1000, 10000);
                    chara_gain_fixed_skill_exp(cdata[rc], i, p);
                }
            }
        }
        chara_vanquish(tc);
        autosave = 1 * (gdata_current_map != mdata_t::map_id_t::show_house);
        chara_gain_skill_exp(cdata.player(), 151, 1200);
        randomize();
        screenupdate = -1;
        update_screen();
        cc = rc;
        csctrl = 4;
        snd(26);
        menu_character_sheet();
        cc = 0;
        goto label_2229_internal;
    case 35:
        txt(i18n::s.get("core.locale.action.use.iron_maiden.use"));
        txtef(9);
        txt(i18n::s.get("core.locale.action.use.iron_maiden.interesting"));
        txt(i18n::s.get(
            "core.locale.action.use.iron_maiden.someone_activates"));
        txtef(9);
        txt(i18n::s.get("core.locale.action.use.iron_maiden.grin"));
        damage_hp(cdata.player(), 9999, -18);
        goto label_2229_internal;
    case 36:
        txt(i18n::s.get("core.locale.action.use.guillotine.use"));
        txtef(9);
        txt(i18n::s.get("core.locale.action.use.iron_maiden.interesting"));
        txt(i18n::s.get("core.locale.action.use.guillotine.someone_activates"));
        txtef(9);
        txt(i18n::s.get("core.locale.action.use.iron_maiden.grin"));
        damage_hp(cdata.player(), 9999, -19);
        goto label_2229_internal;
    case 39:
        txtef(9);
        txt(i18n::s.get("core.locale.action.use.whistle.use"));
        make_sound(cdata[cc].position.x, cdata[cc].position.y, 10, 1, 1, cc);
        goto label_2229_internal;
    case 37: show_card_collection(); goto label_2229_internal;
    }
label_2229_internal:
    refresh_burden_state();
    return turn_result_t::turn_end;
}

turn_result_t do_open_command()
{
    int refweight = 0;
    if (inv[ci].id == 361)
    {
        modify_karma(cdata.player(), -10);
        invctrl(0) = 22;
        invctrl(1) = 0;
        invfile = inv[ci].param1;
        snd(23);
        shop_sell_item();
        screenupdate = -1;
        update_screen();
        return turn_result_t::turn_end;
    }
    if (inv[ci].id == 560)
    {
        invctrl(0) = 24;
        invctrl(1) = 0;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    if (inv[ci].id == 616)
    {
        invctrl(0) = 24;
        invctrl(1) = 2;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    if (inv[ci].id == 701)
    {
        invctrl(0) = 24;
        invctrl(1) = 8;
        snd(100);
        ctrl_inventory();
        return turn_result_t::turn_end;
    }
    if (inv[ci].id == 600)
    {
        snd(22);
        txt(i18n::s.get("core.locale.action.open.shackle.text"));
        if (gdata_current_map == mdata_t::map_id_t::noyel)
        {
            if (gdata_current_dungeon_level == 1)
            {
                if (gdata_released_fire_giant == 0)
                {
                    if (cdata[gdata_fire_giant].state()
                        == character::state_t::alive)
                    {
                        tc = chara_find(203);
                        if (tc != 0)
                        {
                            txtef(9);
                            txt(i18n::s.get(
                                "core.locale.action.open.shackle.dialog"));
                            cdata[gdata_fire_giant].enemy_id = tc;
                            cdata[gdata_fire_giant].hate = 1000;
                        }
                        gdata_released_fire_giant = 1;
                    }
                }
            }
        }
        return turn_result_t::turn_end;
    }
    if (inv[ci].count != 0)
    {
        invfile = inv[ci].count;
        invcontainer(1) = inv[ci].id;
        const auto container_ci = ci;
        if (inv[ci].id == 641)
        {
            refweight = -1;
        }
        else
        {
            refweight = 0;
        }
        if (inv[ci].count == 3 || inv[ci].count == 4 || inv[ci].count == 6)
        {
            if (gdata_current_map != mdata_t::map_id_t::your_home)
            {
                txt(i18n::s.get("core.locale.action.open.only_in_home"));
                update_screen();
                return turn_result_t::pc_turn_user_error;
            }
        }
        if (inv[ci].count == 5)
        {
            if (adata(16, gdata_current_map) != mdata_t::map_id_t::shop)
            {
                txt(i18n::s.get("core.locale.action.open.only_in_shop"));
                update_screen();
                return turn_result_t::pc_turn_user_error;
            }
        }
        ctrl_file(file_operation2_t::map_items_write, u8"shoptmp.s2");
        tmpload(filesystem::u8path(u8"shop"s + invfile + u8".s2"));
        if (fs::exists(
                filesystem::dir::tmp() / (u8"shop"s + invfile + u8".s2")))
        {
            ctrl_file(
                file_operation2_t::map_items_read,
                u8"shop"s + invfile + u8".s2");
        }
        else
        {
            for (const auto& cnt : items(-1))
            {
                inv[cnt].remove();
            }
        }
        shoptrade = 0;
        invsubroutine = 1;
        invctrl(0) = 22;
        invctrl(1) = 2;
        if (invfile == 3)
        {
            txt(i18n::s.get(
                "core.locale.ui.inv.take.can_claim_more",
                gdata_rights_to_succeed_to));
            invctrl(1) = 1;
        }
        if (invfile == 6 || invcontainer(1) == 641)
        {
            if (invfile == 6)
            {
                invcontainer = 15;
            }
            else
            {
                invcontainer = 4;
            }
            menucycle = 1;
            invctrl(1) = 3;
        }
        mode = 6;
        snd(100);
        ctrl_inventory();
        invcontainer = 0;
        if (refweight == -1)
        {
            refweight = inv_weight(-1) + 2500;
        }
        ctrl_file(
            file_operation2_t::map_items_write, u8"shop"s + invfile + u8".s2");
        ctrl_file(file_operation2_t::map_items_read, u8"shoptmp.s2");
        if (refweight != 0)
        {
            inv[container_ci].weight = refweight;
            refresh_burden_state();
        }
        update_screen();
        mode = 0;
        return turn_result_t::turn_end;
    }
    item_separate(ci);
    if (inv[ci].param1 != 0)
    {
        if (inv[ci].param2 != 0)
        {
            int stat = unlock_box(inv[ci].param2);
            if (stat == 0)
            {
                screenupdate = -1;
                update_screen();
                return turn_result_t::turn_end;
            }
        }
    }
    if (inv[ci].param1 == 0)
    {
        txt(i18n::s.get("core.locale.action.open.empty"));
    }
    else
    {
        if (inv[ci].id == 752)
        {
            open_new_year_gift();
        }
        else
        {
            open_box();
        }
        item_stack(cc, ri);
    }
    screenupdate = -1;
    update_screen();
    return turn_result_t::turn_end;
}

turn_result_t do_use_stairs_command(int val0)
{
    int movelevelbystairs = 0;
    if (dbg_freemove)
    {
        txt(i18n::s.get("core.locale.action.use_stairs.cannot_during_debug"));
        return turn_result_t::pc_turn_user_error;
    }
    int stat = item_find(631, 3, -1);
    if (stat != -1)
    {
        if (mdata_map_type == mdata_t::map_type_t::town
            || mdata_map_type == mdata_t::map_type_t::guild)
        {
            ci = stat;
            return step_into_gate();
        }
    }
    cell_featread(cdata[cc].position.x, cdata[cc].position.y);
    movelevelbystairs = 0;
    if (val0 == 1)
    {
        if (mapitemfind(cdata[cc].position.x, cdata[cc].position.y, 753) != -1)
        {
            txt(i18n::s.get("core.locale.action.use_stairs.kotatsu.prompt"));
            ELONA_YES_NO_PROMPT();
            rtval = show_prompt(promptx, prompty, 160);
            if (rtval != 0)
            {
                update_screen();
                return turn_result_t::pc_turn_user_error;
            }
            txt(i18n::s.get("core.locale.action.use_stairs.kotatsu.use"));
            cdata.player().blind += 2;
            return turn_result_t::turn_end;
        }
    }
    if (gdata_current_map == mdata_t::map_id_t::your_home)
    {
        if (val0 == 1)
        {
            if (mapitemfind(cdata[cc].position.x, cdata[cc].position.y, 751)
                != -1)
            {
                if (gdata_current_dungeon_level >= adata(10, gdata_current_map))
                {
                    txt(i18n::s.get(
                        "core.locale.action.use_stairs.cannot_go.down"));
                    return turn_result_t::pc_turn_user_error;
                }
                else
                {
                    movelevelbystairs = 1;
                }
            }
        }
        if (val0 == 2)
        {
            if (mapitemfind(cdata[cc].position.x, cdata[cc].position.y, 750)
                != -1)
            {
                if (gdata_current_dungeon_level <= adata(17, gdata_current_map))
                {
                    txt(i18n::s.get(
                        "core.locale.action.use_stairs.cannot_go.up"));
                    return turn_result_t::pc_turn_user_error;
                }
                else
                {
                    movelevelbystairs = 1;
                }
            }
        }
    }
    if (movelevelbystairs == 0)
    {
        if (mdata_map_type != mdata_t::map_type_t::world_map)
        {
            if (val0 == 1)
            {
                if (feat(1) != 11)
                {
                    txt(i18n::s.get(
                        "core.locale.action.use_stairs.no.downstairs"));
                    update_screen();
                    return turn_result_t::pc_turn_user_error;
                }
                else
                {
                    movelevelbystairs = 1;
                    if (gdata_current_map == mdata_t::map_id_t::the_void
                        && gdata_current_dungeon_level >= gdata(186))
                    {
                        txt(
                            i18n::s.get("core.locale.action.use_stairs.blocked_"
                                        "by_barrier"));
                        return turn_result_t::pc_turn_user_error;
                    }
                }
            }
            if (val0 == 2)
            {
                if (feat(1) != 10)
                {
                    txt(i18n::s.get(
                        "core.locale.action.use_stairs.no.upstairs"));
                    update_screen();
                    return turn_result_t::pc_turn_user_error;
                }
                else
                {
                    movelevelbystairs = 1;
                }
            }
        }
    }
    if (feat == tile_downlocked)
    {
        f = 0;
        if (gdata_current_dungeon_level == 3)
        {
            if (gdata_main_quest_flag >= 65)
            {
                f = 1;
            }
        }
        if (gdata_current_dungeon_level == 17)
        {
            if (gdata_main_quest_flag >= 115)
            {
                f = 1;
            }
        }
        if (gdata_current_dungeon_level == 25)
        {
            if (gdata_main_quest_flag >= 125)
            {
                f = 1;
            }
        }
        if (gdata_current_dungeon_level == 44)
        {
            if (gdata_main_quest_flag >= 125)
            {
                f = 1;
            }
        }
        if (f == 1)
        {
            if (gdata_current_dungeon_level == 44)
            {
                txt(i18n::s.get("core.locale.action.use_stairs.unlock.stones"));
            }
            else
            {
                txt(i18n::s.get("core.locale.action.use_stairs.unlock.normal"));
            }
            snd(23);
            cell_featset(
                cdata[cc].position.x,
                cdata[cc].position.y,
                tile_downstairs,
                11);
            return turn_result_t::turn_end;
        }
        snd(22);
        txt(i18n::s.get("core.locale.action.use_stairs.locked"));
        return turn_result_t::turn_end;
    }
    if (adata(16, gdata_current_map) == mdata_t::map_id_t::random_dungeon)
    {
        if (gdata_current_dungeon_level == adata(10, gdata_current_map))
        {
            if (adata(20, gdata_current_map) != -1)
            {
                txt(i18n::s.get(
                    "core.locale.action.use_stairs.prompt_give_up_quest"));
                ELONA_YES_NO_PROMPT();
                rtval = show_prompt(promptx, prompty, 160);
                if (rtval != 0)
                {
                    update_screen();
                    return turn_result_t::pc_turn_user_error;
                }
            }
        }
    }
    if (movelevelbystairs == 1)
    {
        bool ok = action_sp(cdata.player(), 15);
        if (!ok || cdata.player().inventory_weight_type >= 3)
        {
            if (!ok || rnd(5 - cdata.player().inventory_weight_type) == 0)
            {
                txt(i18n::s.get("core.locale.action.use_stairs.lost_balance"));
                damage_hp(
                    cdata[cc],
                    cdata[cc].max_hp
                            * (cdata.player().inventory_weight * 10
                                   / cdata.player().max_inventory_weight
                               + 10)
                            / 100
                        + 1,
                    -7);
                msg_halt();
            }
            if (cdata.player().state() == character::state_t::empty)
            {
                return turn_result_t::turn_begin;
            }
        }
    }
    if (feat(1) == 15)
    {
        if (feat(2) + feat(3) * 100 == 35)
        {
            comctrl = 0;
            int stat = query_for_showroom_to_visit();
            if (stat == 1)
            {
                return do_enter_strange_gate();
            }
            else
            {
                return turn_result_t::pc_turn_user_error;
            }
        }
    }
    snd(49);
    levelexitby = 4;
    return turn_result_t::exit_map;
}

turn_result_t do_movement_command()
{
    f = 0;
    if (cdata[cc].dimmed != 0)
    {
        if (cdata[cc].dimmed + 10 > rnd(60))
        {
            f = 1;
        }
    }
    if (cdata[cc].drunk != 0)
    {
        if (rnd(5) == 0)
        {
            txtef(9);
            txt(i18n::s.get("core.locale.action.move.drunk"));
            f = 1;
        }
    }
    if (cdata[cc].confused != 0 || f == 1)
    {
        cdata[cc].next_position.x = cdata[cc].position.x + rnd(3) - 1;
        cdata[cc].next_position.y = cdata[cc].position.y + rnd(3) - 1;
    }
    if (gdata_mount != 0)
    {
        if (cdata[gdata_mount].continuous_action_id != 0)
        {
            if (cdata[gdata_mount].continuous_action_turn > 0)
            {
                txt(i18n::s.get(
                    "core.locale.action.move.interrupt", cdata[gdata_mount]));
                cdata[gdata_mount].continuous_action_id = 0;
                cdata[gdata_mount].continuous_action_turn = 0;
            }
        }
    }
    cell_check(cdata[cc].next_position.x, cdata[cc].next_position.y);
    if (cdata.player().inventory_weight_type >= 4)
    {
        ++msgdup;
        txt(i18n::s.get("core.locale.action.move.carry_too_much"));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    if (cellchara != -1 && cellchara != 0)
    {
        tc = cellchara;
        if (cdata[tc].relationship >= 10
            || (cdata[tc].relationship == -1
                && !config::instance().attack_neutral_npcs)
            || (cdata[tc].relationship == 0
                && (adata(16, gdata_current_map) == mdata_t::map_id_t::museum
                    || adata(16, gdata_current_map) == mdata_t::map_id_t::shop
                    || key_shift)))
        {
            if (cdata[tc].is_hung_on_sand_bag() == 0)
            {
                if (mdata_map_type == mdata_t::map_type_t::world_map)
                {
                    goto label_2204_internal;
                }
                if (config::instance().scroll)
                {
                    cdata.player().next_position.x = cdata[tc].position.x;
                    cdata.player().next_position.y = cdata[tc].position.y;
                    ui_scroll_screen();
                }
                cell_swap(cc, tc);
                txt(i18n::s.get(
                    "core.locale.action.move.displace.text", cdata[tc]));
                if (cdata[tc].id == 271)
                {
                    if (rnd(5) == 0)
                    {
                        if (cdata[tc].sleep == 0)
                        {
                            p = rnd(clamp(cdata[cc].gold, 0, 20) + 1);
                            if (cdata[cc].is_protected_from_thieves())
                            {
                                p = 0;
                            }
                            if (p != 0)
                            {
                                snd(11);
                                cdata[cc].gold -= p;
                                earn_gold(cdata[tc], p);
                                txt(i18n::s.get_enum(
                                    "core.locale.action.move.displace.dialog",
                                    rnd(2)));
                            }
                        }
                    }
                }
                if (cdata[tc].continuous_action_id == 1)
                {
                    if (cdata[tc].continuous_action_turn > 0)
                    {
                        txt(i18n::s.get(
                            "core.locale.action.move.interrupt", cdata[tc]));
                        cdata[tc].continuous_action_id = 0;
                        cdata[tc].continuous_action_turn = 0;
                    }
                }
                sense_map_feats_on_move();
                return turn_result_t::turn_end;
            }
        }
        if (running)
        {
            if (cdata[tc].relationship >= -2 || keybd_wait >= 40)
            {
                return turn_result_t::pc_turn_user_error;
            }
        }
        if (cdata[tc].relationship <= -1)
        {
            cdata.player().enemy_id = tc;
            if (cdata[tc].is_invisible() == 1)
            {
                if (cdata.player().can_see_invisible() == 0)
                {
                    if (cdata[tc].wet == 0)
                    {
                        cdata.player().enemy_id = 0;
                    }
                }
            }
            if (keybd_attacking == 0)
            {
                keybd_wait = 1;
                keybd_attacking = 1;
            }
            try_to_melee_attack();
            return turn_result_t::turn_end;
        }
        talk_to_npc();
        if (chatteleport == 1)
        {
            chatteleport = 0;
            return turn_result_t::exit_map;
        }
        return turn_result_t::turn_end;
    }
    else
    {
        keybd_attacking = 0;
    }
    if (mdata_map_type == mdata_t::map_type_t::world_map)
    {
        if (dbg_freemove)
        {
            x = cdata.player().next_position.x;
            y = cdata.player().next_position.y;
            if (x >= 0 && x < mdata_map_width && y >= 0 && y < mdata_map_height)
            {
                if (map(x, y, 1) == 0)
                {
                    cellaccess = 1;
                }
            }
        }
    }
    if (cellaccess == 1)
    {
    label_2204_internal:
        if (mdata_map_type == mdata_t::map_type_t::world_map)
        {
            if (264 <= map(cdata[cc].next_position.x,
                           cdata[cc].next_position.y,
                           0)
                && map(cdata[cc].next_position.x, cdata[cc].next_position.y, 0)
                    < 363)
            {
                return turn_result_t::pc_turn_user_error;
            }
        }
        return proc_movement_event();
    }
    if (mdata_map_type == mdata_t::map_type_t::shelter
        || (gdata_current_dungeon_level == 1
            && mdata_map_type != mdata_t::map_type_t::world_map
            && (mdata_map_type < mdata_t::map_type_t::dungeon
                || mdata_t::map_type_t::dungeon_castle < mdata_map_type)))
    {
        if (cdata[cc].next_position.x < 0
            || cdata[cc].next_position.x > mdata_map_width - 1
            || cdata[cc].next_position.y < 0
            || cdata[cc].next_position.y > mdata_map_height - 1)
        {
            txt(i18n::s.get("core.locale.action.move.leave.prompt", mdatan(0)));
            if (mdata_map_type == mdata_t::map_type_t::temporary)
            {
                if (gdata(73) != 3)
                {
                    txt(i18n::s.get(
                        "core.locale.action.move.leave.abandoning_quest"));
                }
            }
            ELONA_YES_NO_PROMPT();
            rtval = show_prompt(promptx, prompty, 160);
            update_screen();
            if (rtval == 0)
            {
                gdata(60) = cdata.player().position.x;
                gdata(61) = cdata.player().position.y;
                snd(49);
                --gdata_current_dungeon_level;
                levelexitby = 4;
                return turn_result_t::exit_map;
            }
            return turn_result_t::pc_turn_user_error;
        }
    }
    if (cellfeat != -1)
    {
        if (cellfeat == 21)
        {
            return proc_movement_event();
        }
        keyhalt = 1;
        if (cellfeat == 23)
        {
            snd(99);
            return turn_result_t::show_quest_board;
        }
        if (cellfeat == 31)
        {
            snd(99);
            voting_box();
            return turn_result_t::turn_end;
        }
        if (cellfeat == 33)
        {
            menucycle = 1;
            show_city_chart();
            return turn_result_t::pc_turn_user_error;
        }
    }
    if (cdata.player().confused != 0)
    {
        ++msgdup;
        txt(i18n::s.get("core.locale.action.move.confused"));
        update_screen();
    }
    return turn_result_t::pc_turn_user_error;
}

turn_result_t do_read_command()
{
    if (inv[ci].id == 783)
    {
        if (inv[ci].subname == 0)
        {
            txt(i18n::s.get("core.locale.action.read.recipe.info"));
            return turn_result_t::turn_end;
        }
    }
    efid = 0;
    dbid = inv[ci].id;
    access_item_db(13);
    if (efid == 1115)
    {
        return build_new_building();
    }
    return turn_result_t::turn_end;
}

turn_result_t do_eat_command()
{
    if (cc == 0)
    {
        int stat = cargocheck();
        if (stat == 0)
        {
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        if (itemusingfind(ci) > 0)
        {
            txt(i18n::s.get("core.locale.action.someone_else_is_using"));
            return turn_result_t::pc_turn_user_error;
        }
    }
    else if (itemusingfind(ci) != -1)
    {
        tc = itemusingfind(ci);
        if (tc != cc)
        {
            rowactend(tc);
            if (is_in_fov(cdata[cc]))
            {
                txt(i18n::s.get(
                    "core.locale.action.eat.snatches", cdata[cc], cdata[tc]));
            }
        }
    }
    cdata[cc].emotion_icon = 116;
    continuous_action_eating();
    return turn_result_t::turn_end;
}

turn_result_t do_drink_command()
{
    dbid = inv[ci].id;
    access_item_db(15);
    return turn_result_t::turn_end;
}

turn_result_t do_zap_command()
{
    dbid = inv[ci].id;
    access_item_db(14);
    int stat = do_zap();
    if (stat == 0)
    {
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    return turn_result_t::turn_end;
}

turn_result_t do_rest_command()
{
    do_rest();
    return turn_result_t::turn_end;
}

turn_result_t do_fire_command()
{
    cc = 0;
    int stat = find_enemy_target();
    if (stat == 0)
    {
        return turn_result_t::pc_turn_user_error;
    }
    tc = cdata.player().enemy_id;
    if (cdata[tc].relationship >= 0)
    {
        int stat = prompt_really_attack();
        if (stat == 0)
        {
            return turn_result_t::pc_turn_user_error;
        }
    }
    {
        int stat = can_do_ranged_attack();
        if (stat == -1)
        {
            ++msgdup;
            txt(i18n::s.get("core.locale.action.ranged.equip.need_weapon"));
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        if (stat == -2)
        {
            ++msgdup;
            txt(i18n::s.get("core.locale.action.ranged.equip.need_ammo"));
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        if (stat == -3)
        {
            ++msgdup;
            txt(i18n::s.get("core.locale.action.ranged.equip.wrong_ammo"));
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
    }
    do_ranged_attack();
    return turn_result_t::turn_end;
}

turn_result_t do_get_command()
{
    const auto item_info = cell_itemoncell(cdata.player().position);
    const auto number = item_info.first;
    const auto item = item_info.second;

    if (map(cdata.player().position.x, cdata.player().position.y, 6) != 0
        && gdata_current_map != mdata_t::map_id_t::show_house && number == 0)
    {
        cell_featread(cdata.player().position.x, cdata.player().position.y);
        if (feat(1) == 29)
        {
            if (feat < tile_plant + 2)
            {
                txt(i18n::s.get("core.locale.action.get.plant.young"));
                map(cdata.player().position.x, cdata.player().position.y, 6) =
                    0;
                return turn_result_t::turn_end;
            }
            if (feat == tile_plant + 3)
            {
                txt(i18n::s.get("core.locale.action.get.plant.dead"));
                map(cdata.player().position.x, cdata.player().position.y, 6) =
                    0;
                return turn_result_t::turn_end;
            }
            if (!inv_getspace(0))
            {
                txt(i18n::s.get("core.locale.ui.inv.common.inventory_is_full"));
                update_screen();
                return turn_result_t::pc_turn_user_error;
            }
            create_harvested_item();
            harvest_plant(
                chipm(
                    0,
                    map(cdata.player().position.x,
                        cdata.player().position.y,
                        0))
                        == 2
                    ? 1
                    : 0);
            if (feat(2) == 40)
            {
                autosave =
                    1 * (gdata_current_map != mdata_t::map_id_t::show_house);
            }
            refresh_burden_state();
            return turn_result_t::turn_end;
        }
        if (mdata_map_type == mdata_t::map_type_t::world_map && feat(1) == 15
            && feat(2) + feat(3) * 100 >= 300 && feat(2) + feat(3) * 100 < 450)
        {
            txt(i18n::s.get("core.locale.action.get.building.prompt"));
            ELONA_YES_NO_PROMPT();
            rtval = show_prompt(promptx, prompty, 160);
            if (rtval != 0)
            {
                update_screen();
                return turn_result_t::pc_turn_user_error;
            }
            area = feat(2) + feat(3) * 100;
            map(cdata.player().position.x, cdata.player().position.y, 6) = 0;
            adata(16, area) = mdata_t::map_id_t::none;
            removeworker(area);
            map_global_prepare();
            ctrl_file(file_operation_t::temp_dir_delete_area);
            snd(58);
            txt(i18n::s.get("core.locale.action.get.building.remove"));
            return turn_result_t::turn_end;
        }
    }

    if (number == 0)
    {
        if ((mdata_map_type == mdata_t::map_type_t::town
             || mdata_map_type == mdata_t::map_type_t::guild)
            && chipm(
                   0,
                   map(cdata.player().position.x, cdata.player().position.y, 0))
                == 4)
        {
            snd(83);
            txt(i18n::s.get("core.locale.action.get.snow"));
            if (!action_sp(cdata.player(), 10))
            {
                txt(i18n::s.get("core.locale.magic.common.too_exhausted"));
                return turn_result_t::turn_end;
            }
            flt();
            {
                int stat = itemcreate(0, 587, -1, -1, 0);
                if (stat != 0)
                {
                    inv[ci].curse_state = curse_state_t::none;
                    inv[ci].identification_state =
                        identification_state_t::completely_identified;
                    item_stack(0, ci, 1);
                }
            }
            return turn_result_t::turn_end;
        }
        ++msgdup;
        txt(i18n::s.get("core.locale.action.get.air"));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }

    ci = item;
    if (number > 1)
    {
        invctrl = 3;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    if ((inv[ci].own_state > 0 && inv[ci].own_state < 3)
        || inv[ci].own_state == 5)
    {
        snd(27);
        ++msgdup;
        if (inv[ci].own_state == 2)
        {
            txt(i18n::s.get("core.locale.action.get.cannot_carry"));
        }
        if (inv[ci].own_state == 1 || inv[ci].own_state == 5)
        {
            txt(i18n::s.get_enum("core.locale.action.get.not_owned", rnd(3)));
        }
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    in = inv[ci].number();

    int stat = pick_up_item();
    if (stat == 1 || stat == -1)
    {
        return turn_result_t::turn_end;
    }
    else
    {
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
}

turn_result_t do_cast_command()
{
    tc = cc;
    int stat = do_cast_magic();
    if (stat == 0)
    {
        return turn_result_t::pc_turn_user_error;
    }
    return turn_result_t::turn_end;
}

turn_result_t do_short_cut_command()
{
    menucycle = 0;
    if (gdata(40 + sc) == 0)
    {
        ++msgdup;
        txt(i18n::s.get("core.locale.action.shortcut.unassigned"));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    if (gdata(40 + sc) >= 10000)
    {
        invsc = gdata((40 + sc)) % 10000;
        invctrl(0) = gdata((40 + sc)) / 10000;
        invctrl(1) = 0;
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    efid = gdata(40 + sc);
    if (efid >= 300 && efid < 400)
    {
        return do_use_magic();
    }
    if (efid >= 600)
    {
        if (mdata_map_type == mdata_t::map_type_t::world_map)
        {
            txtnew();
            txt(i18n::s.get("core.locale.action.cannot_do_in_global"));
            display_msg();
            redraw();
            return turn_result_t::pc_turn_user_error;
        }
        if (efid < 661)
        {
            if (spact(efid - 600) == 0)
            {
                txt(i18n::s.get(
                    "core.locale.action.shortcut.cannot_use_anymore"));
                update_screen();
                return turn_result_t::pc_turn_user_error;
            }
        }
        return do_use_magic();
    }
    if (efid >= 400)
    {
        if (mdata_map_type == mdata_t::map_type_t::world_map)
        {
            txtnew();
            txt(i18n::s.get("core.locale.action.cannot_do_in_global"));
            display_msg();
            redraw();
            return turn_result_t::pc_turn_user_error;
        }
        if (spell(efid - 400) <= 0)
        {
            ++msgdup;
            txt(i18n::s.get(
                "core.locale.action.shortcut.cannot_use_spell_anymore"));
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        return do_cast_command();
    }
    return turn_result_t::pc_turn_user_error;
}

turn_result_t do_exit_command()
{
    txtnew();
    if (gdata_current_map == mdata_t::map_id_t::show_house)
    {
        txtef(3);
        txt(i18n::s.get("core.locale.action.exit.cannot_save_in_usermap"));
    }
    else
    {
        txt(i18n::s.get("core.locale.action.exit.prompt"));
    }
    ELONA_APPEND_PROMPT(
        i18n::s.get("core.locale.action.exit.choices.exit"),
        u8"a"s,
        ""s + promptmax);
    ELONA_APPEND_PROMPT(
        i18n::s.get("core.locale.action.exit.choices.cancel"),
        u8"b"s,
        ""s + promptmax);
    ELONA_APPEND_PROMPT(
        i18n::s.get("core.locale.action.exit.choices.game_setting"),
        u8"c"s,
        ""s + promptmax);
    rtval = show_prompt(promptx, prompty, 190);
    if (rtval == 0)
    {
        if (gdata_current_map != mdata_t::map_id_t::show_house)
        {
            snd(44);
            save_game();
            txt(i18n::s.get("core.locale.action.exit.saved"));
            txt(i18n::s.get(
                "core.locale.action.exit.you_close_your_eyes", cdata[cc]));
            msg_halt();
            update_screen();
        }
        return turn_result_t::finish_elona;
    }
    if (rtval == 2)
    {
        snd(20);
        set_option();
    }
    update_screen();
    return turn_result_t::pc_turn_user_error;
}

int ask_direction_to_close()
{
    int number_of_doors{};
    position_t pos;
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (dy == 0 && dx == 0)
                continue;

            int x = cdata.player().position.x + dx;
            int y = cdata.player().position.y + dy;
            cell_featread(x, y);
            if (feat(1) == 20 && map(x, y, 1) == 0)
            {
                ++number_of_doors;
                pos = {x, y};
            }
        }
    }
    if (number_of_doors == 1)
    {
        x = pos.x;
        y = pos.y;
        return 1;
    }

    txt(i18n::s.get("core.locale.action.which_direction.door"));
    update_screen();
    return ask_direction();
}

} // namespace elona
