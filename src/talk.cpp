#include "talk.hpp"
#include "audio.hpp"
#include "calc.hpp"
#include "character.hpp"
#include "config.hpp"
#include "draw.hpp"
#include "enums.hpp"
#include "event.hpp"
#include "i18n.hpp"
#include "input.hpp"
#include "item.hpp"
#include "itemgen.hpp"
#include "menu.hpp"
#include "quest.hpp"
#include "random.hpp"
#include "shop.hpp"
#include "ui.hpp"
#include "variables.hpp"

namespace elona
{

int chatpicloaded = 0;
int j_at_m193 = 0;
int p_at_m193 = 0;

void talk_start()
{
    gsel(4);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"face1.bmp", 1);
    gsel(7);
    picload(filesystem::dir::graphic() / u8"ie_chat.bmp");
    gsel(0);
    return;
}

void talk_to_npc()
{
    keyhalt = 1;
    if (cdata[tc].character_role == 1005)
    {
        if (config::instance().extrahelp)
        {
            if (gdata(207) == 0)
            {
                if (mode == 0)
                {
                    if (cdata.player().continuous_action_turn == 0)
                    {
                        gdata(207) = 1;
                        ghelp = 7;
                        show_ex_help();
                        screenupdate = -1;
                        update_screen();
                    }
                }
            }
        }
    }
    if (cdata[tc].character_role == 7)
    {
        if (config::instance().extrahelp)
        {
            if (gdata(208) == 0)
            {
                if (mode == 0)
                {
                    if (cdata.player().continuous_action_turn == 0)
                    {
                        gdata(208) = 1;
                        ghelp = 8;
                        show_ex_help();
                        screenupdate = -1;
                        update_screen();
                    }
                }
            }
        }
    }
    set_npc_religion();
    if (scenemode == 0)
    {
        snd(99);
        map_proc_regen_and_update();
    }
    cs = 0;
    chatflag = 0;
    chatesc = 1;
    if (cdata[tc].relationship <= -1)
    {
        if (evnum == 0)
        {
            txt(i18n::s.get("core.locale.talk.will_not_listen", cdata[tc]));
            quest_teleport = false;
            update_screen();
            return;
        }
    }
    if (gdata_hour + gdata_day * 24 + gdata_month * 24 * 30
            + gdata_year * 24 * 30 * 12
        >= cdata[tc].time_interest_revive)
    {
        cdata[tc].interest = 100;
    }
    if ((cdata[tc].character_role >= 1000 && cdata[tc].character_role < 2000)
        || cdata[tc].character_role == 2003)
    {
        invfile = cdata[tc].shop_store_id;
        shop_refresh_on_talk();
    }
    talk_start();
    if (scenemode == 1)
    {
        talk_wrapper(talk_result_t::talk_more);
        return;
    }
    chatval_unique_chara_id = none;
    chatval_show_impress = true;
    if (cdata[tc].quality == 6 && tc >= 16)
    {
        chatval_unique_chara_id = cdata[tc].id;
        chatval_show_impress = false;
    }
    if (event_id() == 2)
    {
        talk_wrapper(talk_result_t::talk_game_begin);
        return;
    }
    if (event_id() == 16)
    {
        talk_wrapper(talk_result_t::talk_finish_escort);
        return;
    }
    if (cdata[tc].sleep != 0)
    {
        talk_wrapper(talk_result_t::talk_sleeping);
        return;
    }
    if (cdata[tc].continuous_action_id)
    {
        talk_wrapper(talk_result_t::talk_busy);
        return;
    }
    if (tc == 0)
    {
        talk_wrapper(talk_result_t::talk_end);
        return;
    }
    if (cdata[tc].visited_just_now())
    {
        cdata[tc].visited_just_now() = false;
        talk_wrapper(talk_result_t::talk_house_visitor);
    }
    if (chatval_unique_chara_id)
    {
        if (gdata_current_map != mdata_t::map_id_t::show_house)
        {
            if (tc >= 16)
            {
                talk_wrapper(talk_result_t::talk_unique);
                return;
            }
        }
    }
    if (quest_teleport)
    {
        quest_teleport = false;
        talk_wrapper(talk_result_t::talk_quest_giver);
        return;
    }
    buff = "";
    talk_wrapper(talk_result_t::talk_npc);
}



talk_result_t talk_more()
{
    listmax = 0;
    buff = buff;
    tc = tc * 1 + 0;
    list(0, listmax) = 0;
    listn(0, listmax) = i18n::_(u8"ui", u8"more");
    ++listmax;
    chatesc = 1;
    talk_window_query();
    if (scenemode)
    {
        if (scene_cut == 1)
        {
            return talk_result_t::talk_end;
        }
    }
    return talk_result_t::talk_end;
}


talk_result_t talk_sleeping()
{
    listmax = 0;
    buff = u8"("s + i18n::s.get("core.locale.talk.is_sleeping", cdata[tc])
        + u8")"s;
    tc = tc * 1 + 0;
    list(0, listmax) = 0;
    listn(0, listmax) = i18n::_(u8"ui", u8"bye");
    ++listmax;
    chatesc = 1;
    talk_window_query();
    if (scenemode)
    {
        if (scene_cut == 1)
        {
            return talk_result_t::talk_end;
        }
    }
    return talk_result_t::talk_end;
}

talk_result_t talk_busy()
{
    listmax = 0;
    buff = u8"("s + i18n::s.get("core.locale.talk.is_busy", cdata[tc]) + u8")"s;
    tc = tc * 1 + 0;
    list(0, listmax) = 0;
    listn(0, listmax) = i18n::_(u8"ui", u8"bye");
    ++listmax;
    chatesc = 1;
    talk_window_query();
    if (scenemode)
    {
        if (scene_cut == 1)
        {
            return talk_result_t::talk_end;
        }
    }
    return talk_result_t::talk_end;
}

talk_result_t talk_ignored()
{
    listmax = 0;
    buff = i18n::s.get("core.locale.talk.ignores_you", cdata[tc]);
    tc = tc * 1 + 0;
    ELONA_APPEND_RESPONSE(0, i18n::_(u8"ui", u8"bye"));
    chatesc = 1;
    ELONA_TALK_SCENE_CUT();
    return talk_result_t::talk_end;
}

static void _give_potion_of_cure_corruption(int stat)
{
    inv[stat].modify_number(-1);
    txt(i18n::s.get("core.locale.talk.unique.pael.give.you_give"));
    snd(13);
}

bool talk_give_potion_of_cure_corruption()
{
    list(0, listmax) = 1;
    listn(0, listmax) = i18n::s.get("core.locale.talk.unique.pael.give.choice");
    ++listmax;
    list(0, listmax) = 0;
    listn(0, listmax) = i18n::_(u8"ui", u8"bye");
    ++listmax;
    int chatval_ = talk_window_query();
    if (chatval_ != 1)
    {
        return false;
    }
    int stat = inv_find(559, 0);
    if (stat == -1)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.pael.give.do_not_have");
        tc = tc * 1 + 0;
        list(0, listmax) = 0;
        listn(0, listmax) = i18n::_(u8"ui", u8"more");
        ++listmax;
        chatesc = 1;
        talk_window_query();
        if (scenemode)
        {
            if (scene_cut == 1)
            {
                return false;
            }
        }
        return false;
    }

    _give_potion_of_cure_corruption(stat);

    listmax = 0;
    buff =
        i18n::s.get("core.locale.talk.unique.pael.give.dialog", cdatan(0, 0));
    tc = tc * 1 + 0;
    list(0, listmax) = 0;
    listn(0, listmax) = i18n::_(u8"ui", u8"more");
    ++listmax;
    chatesc = 1;
    talk_window_query();
    if (scenemode)
    {
        if (scene_cut == 1)
        {
            return false;
        }
    }
    return true;
}



talk_result_t talk_game_begin()
{
    if (lomiaseaster)
    {
        cdata.player().blind = 100;
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.begin.easter_egg", 0);
        tc =
            tc * (chara_find(33) == 0) + (chara_find(33) != 0) * chara_find(33);
        list(0, listmax) = 0;
        listn(0, listmax) = i18n::_(u8"ui", u8"more");
        ++listmax;
        chatesc = 1;
        talk_window_query();
        if (scenemode)
        {
            if (scene_cut == 1)
            {
                return talk_result_t::talk_end;
            }
        }
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.begin.easter_egg", 1);
        tc =
            tc * (chara_find(34) == 0) + (chara_find(34) != 0) * chara_find(34);
        list(0, listmax) = 0;
        listn(0, listmax) = i18n::_(u8"ui", u8"more");
        ++listmax;
        chatesc = 1;
        talk_window_query();
        if (scenemode)
        {
            if (scene_cut == 1)
            {
                return talk_result_t::talk_end;
            }
        }
        update_screen();
        await(3000);
        txtnew();
        txtef(3);
        txt(
            i18n::s.get("core.locale.talk.unique.lomias.begin.easter_egg."
                        "something_is_killed"));
        txt(i18n::s.get("core.locale.talk.unique.lomias.begin.easter_egg.ugh"));
        snd(8);
        spillblood(28, 6, 10);
        flt();
        itemcreate(-1, 705, 28, 6, 0);
        update_screen();
        await(500);
        await(500);
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.begin.easter_egg", 2);
        tc =
            tc * (chara_find(33) == 0) + (chara_find(33) != 0) * chara_find(33);
        list(0, listmax) = 0;
        listn(0, listmax) = i18n::_(u8"ui", u8"more");
        ++listmax;
        chatesc = 1;
        talk_window_query();
        if (scenemode)
        {
            if (scene_cut == 1)
            {
                return talk_result_t::talk_end;
            }
        }
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.begin.easter_egg", 3);
        tc =
            tc * (chara_find(34) == 0) + (chara_find(34) != 0) * chara_find(34);
        list(0, listmax) = 0;
        listn(0, listmax) = i18n::_(u8"ui", u8"more");
        ++listmax;
        chatesc = 1;
        talk_window_query();
        if (scenemode)
        {
            if (scene_cut == 1)
            {
                return talk_result_t::talk_end;
            }
        }
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.begin.easter_egg", 4);
        tc =
            tc * (chara_find(33) == 0) + (chara_find(33) != 0) * chara_find(33);
        list(0, listmax) = 0;
        listn(0, listmax) = i18n::_(u8"ui", u8"more");
        ++listmax;
        chatesc = 1;
        talk_window_query();
        if (scenemode)
        {
            if (scene_cut == 1)
            {
                return talk_result_t::talk_end;
            }
        }
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.begin.easter_egg", 5);
        tc =
            tc * (chara_find(34) == 0) + (chara_find(34) != 0) * chara_find(34);
        list(0, listmax) = 0;
        listn(0, listmax) = i18n::_(u8"ui", u8"more");
        ++listmax;
        chatesc = 1;
        talk_window_query();
        if (scenemode)
        {
            if (scene_cut == 1)
            {
                return talk_result_t::talk_end;
            }
        }
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.begin.easter_egg", 6);
        tc =
            tc * (chara_find(33) == 0) + (chara_find(33) != 0) * chara_find(33);
        list(0, listmax) = 0;
        listn(0, listmax) = i18n::_(u8"ui", u8"more");
        ++listmax;
        chatesc = 1;
        talk_window_query();
        if (scenemode)
        {
            if (scene_cut == 1)
            {
                return talk_result_t::talk_end;
            }
        }
        await(1500);
        update_screen();
        fade_out();
        gsel(4);
        pos(0, 0);
        picload(filesystem::dir::graphic() / u8"face1.bmp", 1);
        gsel(0);
        cdata.player().blind = 0;
        txt(i18n::s.get(
            "core.locale.talk.unique.lomias.begin.easter_egg.was_dream"));
    }
    else
    {
        txtnew();
    }
    txt(i18n::s.get(
        "core.locale.talk.unique.lomias.begin.regain_consciousness"));
    listmax = 0;
    buff = i18n::s.get_enum("core.locale.talk.unique.lomias.begin", 0);
    tc = tc * (chara_find(34) == 0) + (chara_find(34) != 0) * chara_find(34);
    list(0, listmax) = 0;
    listn(0, listmax) = i18n::_(u8"ui", u8"more");
    ++listmax;
    chatesc = 1;
    talk_window_query();
    if (scenemode)
    {
        if (scene_cut == 1)
        {
            return talk_result_t::talk_end;
        }
    }
    listmax = 0;
    buff = i18n::s.get_enum("core.locale.talk.unique.lomias.begin", 1);
    tc = tc * 1 + 0;
    list(0, listmax) = 0;
    listn(0, listmax) = i18n::_(u8"ui", u8"more");
    ++listmax;
    chatesc = 1;
    talk_window_query();
    if (scenemode)
    {
        if (scene_cut == 1)
        {
            return talk_result_t::talk_end;
        }
    }
    listmax = 0;
    buff = i18n::s.get_enum("core.locale.talk.unique.lomias.begin", 2);
    tc = tc * 1 + 0;
    list(0, listmax) = 0;
    listn(0, listmax) = i18n::_(u8"ui", u8"more");
    ++listmax;
    chatesc = 1;
    talk_window_query();
    if (scenemode)
    {
        if (scene_cut == 1)
        {
            return talk_result_t::talk_end;
        }
    }
    listmax = 0;
    buff = i18n::s.get_enum("core.locale.talk.unique.lomias.begin", 3);
    tc = tc * (chara_find(33) == 0) + (chara_find(33) != 0) * chara_find(33);
    list(0, listmax) = 0;
    listn(0, listmax) = i18n::_(u8"ui", u8"more");
    ++listmax;
    chatesc = 1;
    talk_window_query();
    if (scenemode)
    {
        if (scene_cut == 1)
        {
            return talk_result_t::talk_end;
        }
    }
    listmax = 0;
    buff = i18n::s.get_enum(
        "core.locale.talk.unique.lomias.begin", 4, cdatan(0, 0));
    tc = tc * (chara_find(34) == 0) + (chara_find(34) != 0) * chara_find(34);
    list(0, listmax) = 0;
    listn(0, listmax) = i18n::_(u8"ui", u8"more");
    ++listmax;
    chatesc = 1;
    talk_window_query();
    if (scenemode)
    {
        if (scene_cut == 1)
        {
            return talk_result_t::talk_end;
        }
    }
    mdata_map_bgm = 67;
    play_music();
    return talk_result_t::talk_unique;
}


void talk_end()
{
    quest_teleport = false;
    if (scenemode == 0)
    {
        screenupdate = -1;
        update_screen();
    }
    return;
}



int talk_window_query()
{
    cs_bk = -1;
    key_list = key_enter;
    for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
    {
        key_list(cnt) = key_select(cnt);
        ++keyrange;
    }
    keyrange = listmax;
    key = "";
    objprm(0, ""s);
    keylog = "";
    talk_window_init_and_show();
    while (1)
    {
        talk_window_show();
        font(14 - en * 2);
        cs_listbk();
        for (int cnt = 0, cnt_end = (keyrange); cnt < cnt_end; ++cnt)
        {
            if (cs == cnt)
            {
                color(100, 160, 250);
            }
            else
            {
                color(255, 255, 255);
            }
            noteget(s, cnt);
            x = wx + 136;
            y = wy + wh - 56 - keyrange * 19 + cnt * 19 + 2;
            display_key(x, y, cnt);
            cs_list(cs == cnt, listn(0, cnt), x + 30, y, 4, 0);
            color(0, 0, 0);
        }
        cs_bk = cs;
        redraw();
        await(config::instance().wait1);
        key_check();
        cursor_check();
        int a{};
        a = stick(stick_key::escape);
        if (a == stick_key::escape)
        {
            if (scenemode)
            {
                scene_cut = 1;
                talk_reset_variables();
                return -1;
            }
        }
        p = -1;
        for (int cnt = 0, cnt_end = (keyrange); cnt < cnt_end; ++cnt)
        {
            if (key == key_select(cnt))
            {
                p = list(0, cnt);
                break;
            }
        }
        if (p != -1)
        {
            talk_reset_variables();
            return p(0);
        }
        if (key == key_cancel)
        {
            if (chatesc == 1)
            {
                talk_reset_variables();
                return -1;
            }
        }
    }
}



void talk_reset_variables()
{
    snd(10);
    cs = 0;
    listmax = 0;
}



void talk_window_init_and_show()
{
    talk_window_init();
    talk_window_show();
}



void talk_window_init()
{
    if (scenemode == 0)
    {
        screenupdate = -1;
        update_screen();
        cs = 0;
    }
    talk_conv(buff, 56 - en * 3);
    chatpicloaded = 0;
    wx = (windoww - 600) / 2 + inf_screenx;
    wy = winposy(380);
    ww = 600;
    wh = 380;
    gmode(6, 80);
    pos(wx + 4, wy - 16);
    gcopy(7, 0, 0, 600, 380);
}



void talk_window_show()
{
    gmode(2);
    pos(wx, wy - 20);
    gcopy(7, 0, 0, 600, 380);
    if (cdata[tc].portrait >= 0 || scenemode)
    {
        p = cdata[tc].sex * 64 + cdata[tc].portrait;
        if (scenemode)
        {
            p = elona::stoi(actor(1, rc));
        }
        pos(wx + 42, wy + 42);
        gcopy(4, p % 16 * 48, p / 16 * 72, 48, 72, 80, 112);
    }
    else
    {
        const auto portrait_filepath = filesystem::dir::user()
            / (u8"graphic/face"s + std::abs(cdata[tc].portrait + 1) + u8".bmp");
        if (!fs::exists(portrait_filepath) || cdata[tc].portrait == -1)
        {
            int chara_chip_id = cdata[tc].image % 1000;
            draw_chara(
                cdata[tc],
                wx + 82,
                wy + 125 - chara_chips[chara_chip_id].offset_y,
                2);
        }
        else
        {
            if (chatpicloaded == 0)
            {
                gsel(4);
                pos(0, 0);
                picload(portrait_filepath, 1);
                gsel(0);
                chatpicloaded = 1;
            }
            pos(wx + 42, wy + 42);
            gcopy(4, 0, 0, 80, 112, 80, 112);
        }
    }
    font(10 - en * 2);
    display_topic(
        i18n::s.get("core.locale.talk.window.impress"), wx + 28, wy + 170);
    display_topic(
        i18n::s.get("core.locale.talk.window.attract"), wx + 28, wy + 215);
    font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
    if (cdatan(1, tc) == ""s)
    {
        s = cdatan(0, tc) + u8" "s;
    }
    else
    {
        s = i18n::s.get(
                "core.locale.talk.window.of", cdatan(0, tc), cdatan(1, tc))
            + " ";
    }
    if (cdata[tc].sex == 0)
    {
        s += cnven(i18n::_(u8"ui", u8"male"));
    }
    else
    {
        s += cnven(i18n::_(u8"ui", u8"female"));
    }
    if (cdatan(1, tc) != ""s)
    {
        s += " " + i18n::s.get("core.locale.talk.window.fame", cdata[tc].fame);
    }
    if ((cdata[tc].character_role >= 1000 && cdata[tc].character_role < 2000)
        || cdata[tc].character_role == 2003)
    {
        s += " "
            + i18n::s.get(
                  "core.locale.talk.window.shop_rank", cdata[tc].shop_rank);
    }
    if (gdata_reveals_religion)
    {
        s += u8" ("s + i18n::_(u8"god", cdata[tc].god_id, u8"name") + u8")"s;
    }
    if (scenemode)
    {
        s = actor(0, rc);
    }
    pos(wx + 120, wy + 16);
    color(20, 10, 5);
    mes(s);
    color(0, 0, 0);
    font(13 - en * 2);
    if (chatval_show_impress)
    {
        s = i18n::_(
            u8"ui",
            u8"impression",
            u8"_"s + chara_impression_level(cdata[tc].impression));
        if (cdata[tc].impression < 150)
        {
            s(1) = ""s + cdata[tc].impression;
        }
        else
        {
            s(1) = u8"???"s;
        }
        pos(wx + 32, wy + 198);
        color(20, 10, 5);
        mes(u8"("s + s(1) + u8")"s + s);
        color(0, 0, 0);
        if (cdata[tc].interest >= 0)
        {
            for (int cnt = 0, cnt_end = (cdata[tc].interest / 5 + 1);
                 cnt < cnt_end;
                 ++cnt)
            {
                draw("interest_icon", wx + 26 + cnt * 4, wy + 245);
            }
        }
    }
    else
    {
        pos(wx + 60, wy + 198);
        mes(u8"-"s);
        pos(wx + 60, wy + 245);
        mes(u8"-"s);
    }
    font(14 - en * 2);
    color(20, 10, 5);
    notesel(buff);
    for (int cnt = 0, cnt_end = (noteinfo()); cnt < cnt_end; ++cnt)
    {
        p = cnt;
        x = wx + 150;
        y = wy + 43 + cnt * 19;
        noteget(s, p);
        pos(x, y);
        mes(s);
    }
    color(0, 0, 0);
}



int talk_guide_quest_client()
{
    constexpr int max_quest = 5;

    int ret{};

    for (int i = 0; i < max_quest; ++i)
    {
        const auto quest_id = gdata(160 + i);
        if (qdata(8, quest_id) != 1)
            continue;
        if (gdata_current_dungeon_level != 1)
            continue;

        auto client = -1;
        if (qdata(3, quest_id) == 1011)
        {
            if (qdata(1, quest_id) == gdata_current_map)
            {
                client = qdata(10, quest_id);
            }
        }
        if (qdata(3, quest_id) == 1002)
        {
            if (qdata(1, qdata(10, quest_id)) == gdata_current_map)
            {
                client = qdata(0, qdata(10, quest_id));
            }
        }
        if (client != -1)
        {
            // Check duplicate
            bool duplicated{};
            for (int j = 0; j < i; ++j)
            {
                if (gdata(160 + j) == quest_id)
                {
                    duplicated = true;
                    break;
                }
            }
            if (!duplicated)
            {
                rtval(ret) = client;
                ++ret;
            }
        }
    }

    return ret;
}



int talk_check_trade(int prm_1081)
{
    j_at_m193 = 0;
    for (int cnt = 0; cnt < 5; ++cnt)
    {
        p_at_m193 = gdata(160 + cnt);
        if (qdata(8, p_at_m193) == 1)
        {
            if (gdata_current_dungeon_level == 1)
            {
                if (qdata(1, p_at_m193) == gdata_current_map)
                {
                    if (prm_1081 == qdata(10, p_at_m193))
                    {
                        j_at_m193 = 1;
                        break;
                    }
                }
            }
        }
    }
    return j_at_m193;
}


} // namespace elona
