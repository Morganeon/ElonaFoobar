#include "menu.hpp"
#include <iostream>
#include "ability.hpp"
#include "audio.hpp"
#include "calc.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "class.hpp"
#include "command.hpp"
#include "config.hpp"
#include "db_item.hpp"
#include "defines.hpp"
#include "draw.hpp"
#include "enchantment.hpp"
#include "equipment.hpp"
#include "i18n.hpp"
#include "input.hpp"
#include "item.hpp"
#include "macro.hpp"
#include "menu.hpp"
#include "network.hpp"
#include "quest.hpp"
#include "random.hpp"
#include "trait.hpp"
#include "ui.hpp"
#include "variables.hpp"

#include "ui/ui_menu_adventurers.hpp"
#include "ui/ui_menu_alias.hpp"
#include "ui/ui_menu_book.hpp"
#include "ui/ui_menu_ctrl_ally.hpp"
#include "ui/ui_menu_game_help.hpp"
#include "ui/ui_menu_god.hpp"
#include "ui/ui_menu_hire.hpp"
#include "ui/ui_menu_item_desc.hpp"
#include "ui/ui_menu_npc_tone.hpp"
#include "ui/ui_menu_quest_board.hpp"
#include "ui/ui_menu_scene.hpp"
#include "ui/ui_menu_spell_writer.hpp"

namespace elona
{

std::string strhint6;

void text_set()
{
    strhint1 = i18n::s.get("core.locale.ui.hint.cursor");
    strhint2 = ""s + key_pageup + u8","s + key_pagedown
        + i18n::s.get("core.locale.ui.hint.page");
    strhint3 = i18n::s.get("core.locale.ui.hint.close");
    strhint3b = i18n::s.get("core.locale.ui.hint.back");
    strhint4 = i18n::s.get("core.locale.ui.hint.enter");
    strhint5 =
        ""s + key_identify + i18n::s.get("core.locale.ui.hint.known_info");
    strhint5b = ""s + key_mode + i18n::s.get("core.locale.ui.hint.mode");
    strhint6 = i18n::s.get("core.locale.ui.hint.portrait");
    strhint7 = i18n::s.get("core.locale.ui.hint.shortcut");

    SDIM4(_melee, 20, 3, 8);
    if (jp)
    {
        _melee(0, 0) = u8"殴って"s;
        _melee(1, 0) = u8"殴られた。"s;
        _melee(2, 0) = u8"手"s;
    }
    else
    {
        _melee(0, 0) = u8"punch"s;
        _melee(1, 0) = u8"punch"s;
        _melee(2, 0) = u8"hand"s;
    }
    if (jp)
    {
        _melee(0, 3) = u8"噛み付いて"s;
        _melee(1, 3) = u8"噛み付かれた。"s;
        _melee(2, 3) = u8"牙"s;
    }
    else
    {
        _melee(0, 3) = u8"bite"s;
        _melee(1, 3) = u8"bite"s;
        _melee(2, 3) = u8"fang"s;
    }
    if (jp)
    {
        _melee(0, 4) = u8"睨んで"s;
        _melee(1, 4) = u8"睨まれた。"s;
        _melee(2, 4) = u8"眼"s;
    }
    else
    {
        _melee(0, 4) = u8"gaze"s;
        _melee(1, 4) = u8"gaze"s;
        _melee(2, 4) = u8"eye"s;
    }
    if (jp)
    {
        _melee(0, 1) = u8"引っ掻き"s;
        _melee(1, 1) = u8"引っ掻かれた。"s;
        _melee(2, 1) = u8"爪"s;
    }
    else
    {
        _melee(0, 1) = u8"claw"s;
        _melee(1, 1) = u8"claw"s;
        _melee(2, 1) = u8"claw"s;
    }
    if (jp)
    {
        _melee(0, 5) = u8"刺し"s;
        _melee(1, 5) = u8"刺された。"s;
        _melee(2, 5) = u8"針"s;
    }
    else
    {
        _melee(0, 5) = u8"sting"s;
        _melee(1, 5) = u8"sting"s;
        _melee(2, 5) = u8"needle"s;
    }
    if (jp)
    {
        _melee(0, 6) = u8"触って"s;
        _melee(1, 6) = u8"触られた。"s;
        _melee(2, 6) = u8"手"s;
    }
    else
    {
        _melee(0, 6) = u8"touch"s;
        _melee(1, 6) = u8"touch"s;
        _melee(2, 6) = u8"hand"s;
    }
    if (jp)
    {
        _melee(0, 7) = u8"胞子を撒き散らし"s;
        _melee(1, 7) = u8"胞子を飛ばされた。"s;
        _melee(2, 7) = u8"胞子"s;
    }
    else
    {
        _melee(0, 7) = u8"attack"s;
        _melee(1, 7) = u8"attack"s;
        _melee(2, 7) = u8"spore"s;
    }
    randcolor(0) = 0;
    randcolor(1) = 4;
    randcolor(2) = 2;
    randcolor(3) = 5;
    randcolor(4) = 6;
    _randcolor(0) = 0;
    _randcolor(1) = 2;
    _randcolor(2) = 4;
    _randcolor(3) = 5;
    _randcolor(4) = 6;
    _randcolor(5) = 3;
    homepage = lang(
        u8"http://homepage3.nifty.com/rfish/index.html"s,
        u8"http://homepage3.nifty.com/rfish/index_e.html"s);
    strblank = lang("", u8" "s);
}

void show_quick_menu()
{
    int tx, ty;

    // On Android, draw the menu in the center of the screen.
    // Otherwise, it would be obscured by the keypad in landscape
    // mode.
    if (defines::is_android)
    {
        tx = (windoww / 2) - 100;
        ty = (windowh / 2) - 100;
    }
    else
    {
        tx = 50;
        ty = windowh - 255;
    }

    page = quickpage;
    listmax = 0;
    snd(5);
    cs = -1;
label_2698:
    listmax = 0;
    if (page == 3)
    {
        page = 2;
    }
    if (page == -1)
    {
        page = 0;
    }
    if (page == 0)
    {
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = i18n::s.get("core.locale.ui.quick_menu.help");
        listn(1, listmax) = key_help;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = i18n::s.get("core.locale.ui.quick_menu.log");
        listn(1, listmax) = key_msglog;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = i18n::s.get("core.locale.ui.quick_menu.chara");
        listn(1, listmax) = key_charainfo;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = i18n::s.get("core.locale.ui.quick_menu.journal");
        listn(1, listmax) = key_journal;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
    }
    if (page == 1)
    {
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = i18n::s.get("core.locale.ui.quick_menu.wear");
        listn(1, listmax) = key_wear;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = i18n::s.get("core.locale.ui.quick_menu.rest");
        listn(1, listmax) = key_rest;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = i18n::s.get("core.locale.ui.quick_menu.spell");
        listn(1, listmax) = key_cast;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = i18n::s.get("core.locale.ui.quick_menu.skill");
        listn(1, listmax) = key_skill;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = i18n::s.get("core.locale.ui.quick_menu.fire");
        listn(1, listmax) = key_fire;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = i18n::s.get("core.locale.ui.quick_menu.dig");
        listn(1, listmax) = key_dig;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
    }
    if (page == 2)
    {
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = i18n::s.get("core.locale.ui.quick_menu.pray");
        listn(1, listmax) = key_pray;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = i18n::s.get("core.locale.ui.quick_menu.ammo");
        listn(1, listmax) = key_ammo;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = i18n::s.get("core.locale.ui.quick_menu.interact");
        listn(1, listmax) = key_interact;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = i18n::s.get("core.locale.ui.quick_menu.bash");
        listn(1, listmax) = key_bash;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
    }
    s(0) = i18n::s.get("core.locale.ui.quick_menu.info");
    s(1) = i18n::s.get("core.locale.ui.quick_menu.action");
    s(2) = i18n::s.get("core.locale.ui.quick_menu.etc");
    s(3) = u8"null"s;
    p = page - 1;
    if (p == -1)
    {
        p = 3;
    }
    listn(0, 0) = s(p);
    listn(0, 4) = s(page);
    p = page + 1;
    if (p == 3)
    {
        p = 3;
    }
    listn(0, 8) = s(p);
    t = 0;
label_2699_internal:
    font(12 + sizefix - en * 2);
    x(0) = 25;
    x(1) = 50;
    x(2) = 50;
    x(3) = 100;
    x(4) = 100;
    x(5) = 100;
    x(6) = 150;
    x(7) = 150;
    x(8) = 175;
    y(0) = 50;
    y(1) = 15;
    y(2) = 85;
    y(3) = 0;
    y(4) = 50;
    y(5) = 100;
    y(6) = 15;
    y(7) = 85;
    y(8) = 50;
    s = u8"○コマンド"s;
    gmode(2);
    for (int cnt = 0; cnt < 9; ++cnt)
    {
        p = cnt;
        if (listn(0, p) == u8"null"s)
        {
            continue;
        }
        if (cnt == 0 || cnt == 4 || cnt == 8)
        {
            draw("quickmenu_submenu", x(cnt) + tx, y(cnt) + ty);
        }
        else
        {
            draw("quickmenu_action", x(cnt) + tx, y(cnt) + ty);
        }
        gmode(4, (t + cnt) % 10 * (t + cnt) % 10 * 12 * ((t + cnt) % 50 < 10));
        if (cs == cnt)
        {
            gmode(5, 140);
        }
        if (cnt == 0 || cnt == 4 || cnt == 8)
        {
            draw("quickmenu_submenu", x(cnt) + tx, y(cnt) + ty);
        }
        else
        {
            draw("quickmenu_action", x(cnt) + tx, y(cnt) + ty);
        }
        gmode(2);
        s = listn(0, p);
        bmes(s, x(cnt) + tx + 25 - strlen_u(s) * 3, y(cnt) + ty + 19);
    }
    ++t;
    redraw();
    await(config::instance().wait1);
    key_check(key_wait_delay_t::none);
    if (key == key_north)
    {
        key = listn(1, 3);
        cs = 3;
    }
    if (key == key_south)
    {
        key = listn(1, 5);
        cs = 5;
    }
    if (key == key_west)
    {
        cs = 0;
    }
    if (key == key_east)
    {
        cs = 8;
    }
    if (key == key_northwest)
    {
        key = listn(1, 1);
        cs = 1;
    }
    if (key == key_northeast)
    {
        key = listn(1, 6);
        cs = 6;
    }
    if (key == key_southwest)
    {
        key = listn(1, 2);
        cs = 2;
    }
    if (key == key_southeast)
    {
        key = listn(1, 7);
        cs = 7;
    }
    if (key == key_cancel)
    {
        cs = -1;
        snd(5);
        key = "";
        goto label_2700_internal;
        return;
    }
    if (key != ""s)
    {
        if (cs == -1)
        {
            snd(5);
            key = "";
            goto label_2700_internal;
            return;
        }
        if (key == key_enter)
        {
            key = listn(1, cs);
            goto label_2700_internal;
            return;
        }
    }
    if (key == ""s || cs == 1 || cs == 6 || cs == 2 || cs == 7)
    {
        if (cs != -1)
        {
            key = listn(1, cs);
            goto label_2700_internal;
            return;
        }
    }
    goto label_2699_internal;
label_2700_internal:
    if (cs == 0 || key == key_northeast)
    {
        cs = -1;
        snd(5);
        --page;
        screenupdate = -1;
        update_screen();
        goto label_2698;
    }
    if (cs == 8 || key == key_northwest)
    {
        cs = -1;
        snd(5);
        ++page;
        screenupdate = -1;
        update_screen();
        goto label_2698;
    }
    quickpage = page;
    cs = 0;
    update_screen();
    quickkeywait = 1;
    return;
}

void show_ex_help()
{
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_help.bmp", 1);
    gsel(0);
    page = 0;
    notesel(buff);
    {
        buff(0).clear();
        std::ifstream in{(filesystem::dir::data() / u8"exhelp.txt").native(),
                         std::ios::binary};
        std::string tmp;
        while (std::getline(in, tmp))
        {
            buff(0) += tmp + '\n';
        }
    }
    p = instr(buff, 0, u8"%"s + ghelp + u8","s + lang(u8"JP"s, u8"EN"s));
    if (p == -1)
    {
        dialog(
            u8"help index not found %"s + ghelp + u8","s
            + lang(u8"JP"s, u8"EN"s));
        return;
    }
    buff = strmid(buff, p, instr(buff, p, u8"%END"s));
    notedel(0);
    if (noteinfo() == 0)
    {
        return;
    }
    snd(53);
    while (1)
    {
        gmode(2);
        dx = 480;
        dy = 175;
        window2((windoww - dx) / 2 + inf_screenx, winposy(dy), dx, dy, 4, 0);
        wx = (windoww - dx) / 2 + inf_screenx;
        wy = winposy(dy);
        window2(
            (windoww - 325) / 2 + inf_screenx, winposy(dy) + 6, 325, 32, 0, 1);
        pos(wx + 5, wy + 4);
        gcopy(3, 960, 96, 48, 48);
        pos(wx + dx - 55, wy + 4);
        gcopy(3, 960, 96, 48, 48);
        pos(wx + 10, wy + 42);
        gcopy(3, 960, 144, 96, 120);
        font(16 - en * 2, snail::font_t::style_t::bold);
        bmes(
            i18n::s.get("core.locale.ui.exhelp.title"),
            wx + 142,
            wy + 13,
            {255, 245, 235},
            {80, 60, 50});
        tx = wx + 120;
        ty = wy + 55;
        font(15 - en * 2);
        {
            int y = ty;
            int cnt = 0;
            for (int cnt_end = cnt + (10); cnt < cnt_end; ++cnt)
            {
                noteget(s, page);
                ++page;
                if (page > noteinfo() || s == ""s)
                {
                    break;
                }
                color(30, 30, 30);
                const auto ny = gmes(s, tx, y, 330, {30, 30, 30}, true).y;
                color(0, 0, 0);
                y = ny;
            }
        }
        gmode(2);
        redraw();
        help_halt();
        if (page >= noteinfo())
        {
            break;
        }
    }
    return;
}

void show_game_help()
{
    ui::ui_menu_game_help().show();
}

turn_result_t show_chat_history()
{
    curmenu = 2;
    key_list(0) = key_enter;
    keyrange = 0;
    pagesize = 0;
    gsel(7);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"ie_scroll.bmp");
    gsel(0);
    windowshadow = 1;
    snd(92);
    drawmenu(2);
    ww = clamp(windoww - 90, windoww - 90, 720);
    wh = 440;
    wx = (windoww - ww) / 2 + inf_screenx;
    wy = winposy(wh);
    window_animation(wx, wy, ww, wh, 9, 4);
    s = i18n::s.get("core.locale.ui.message.hit_any_key");
    showscroll(s, wx, wy, ww, wh);
    net_read();
    buff = "";
    notesel(buff);
    header = instr(netbuf, 0, u8"<!--START-->"s) + 13;
    while (1)
    {
        s = "";
        tail = instr(netbuf, header, u8"%"s);
        if (tail == -1)
        {
            break;
        }
        header += tail + 1;
        tail = instr(netbuf, header, u8"%"s);
        s += strmid(netbuf, header, tail) + u8"  "s;
        header += tail + 1;
        tail = instr(netbuf, header, u8"%"s);
        s += strmid(netbuf, header + 4, tail - 4);
        header += tail + 1;
        tail = instr(netbuf, header, u8"%"s);
        header += tail + 2;
        s += u8"\n"s;
        buff += ""s + s;
    }
    font(13 - en * 2);
    i = 0;
    for (int cnt = 0; cnt < 20; ++cnt)
    {
        if (i >= 20)
        {
            break;
        }
        noteget(s, cnt);
        if (en)
        {
            s = strutil::replace(s, u8"&quot;", u8"\"");
        }
        if (s == ""s)
        {
            if (cnt == 0)
            {
                s = u8"No new messages received."s;
            }
            break;
        }
        i += talk_conv(s, (ww - 110 - en * 50) / 7);
        pos(wx + 48, (19 - i) * 16 + wy + 48);
        color(30, 20, 10);
        mes(s);
        color(0, 0, 0);
        ++i;
    }
    redraw();
label_2708_internal:
    await(config::instance().wait1);
    key_check();
    cursor_check();
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 2)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 2;
                }
            }
            key = "";
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    return turn_result_t::show_message_log;
                }
                if (curmenu == 1)
                {
                    return turn_result_t::menu_journal;
                }
                if (curmenu == 2)
                {
                    return turn_result_t::show_chat_history;
                }
            }
        }
    }
    if (key != ""s)
    {
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    goto label_2708_internal;
}

turn_result_t show_message_log()
{
    curmenu = 0;
    windowshadow = 1;
    key_list(0) = key_enter;
    keyrange = 0;
    pagesize = 0;
    wx = inf_msgx - 2;
    wy = inf_msgy - (inf_maxlog - 3) * inf_msgspace - 1;
    ww = windoww - inf_msgx + 6;
    wh(0) = (inf_maxlog - 3) * inf_msgspace;
    wh(1) = 1;
    wh(2) = -1;
    snd(93);
    show_title(i18n::s.get("core.locale.ui.message.hit_any_key"));
    drawmenu(2);
    window_animation_corner(wx, wy, ww, wh, 8, 4);

    p = (windoww - inf_msgx) / 192;
    window2(wx, wy, ww, wh, 1, -1);
    for (int cnt = 0, cnt_end = (inf_maxlog - 3); cnt < cnt_end; ++cnt)
    {
        int cnt2 = cnt;
        pos(cnt);
        for (int cnt = 0, cnt_end = (p + 1); cnt < cnt_end; ++cnt)
        {
            if (cnt == p)
            {
                x = (windoww - inf_msgx) % 192;
            }
            else
            {
                x = 192;
            }
            pos(cnt * 192 + inf_msgx, inf_msgy - (cnt2 + 1) * inf_msgspace);
            gcopy(3, 496, 536 + cnt2 % 4 * inf_msgspace, x, inf_msgspace);
        }
    }
    for (int cnt = 0, cnt_end = (p + 1); cnt < cnt_end; ++cnt)
    {
        if (cnt == p)
        {
            sx = (windoww - inf_msgx) % 192;
        }
        else
        {
            sx = 192;
        }
        pos(cnt * 192 + inf_msgx, inf_msgy);
        gcopy(3, 496, 528, sx, 6);
    }
    gsel(4);
    gmode(0);
    boxf();
    for (int cnt = 0, cnt_end = (inf_maxlog - 3); cnt < cnt_end; ++cnt)
    {
        p = msgline - cnt - 3;
        if (p < 0)
        {
            p += inf_maxlog;
        }
        else if (p >= inf_maxlog)
        {
            p -= inf_maxlog;
        }
        if (p < 0)
        {
            continue;
        }
        pos(inf_msgx, inf_msgy - cnt * inf_msgspace);
        gcopy(8, 0, p * inf_msgspace, windoww - inf_msgx, inf_msgspace);
    }
    gsel(0);
    gmode(2);
    pos(0, -3);
    gcopy(4, 0, 0, windoww, inf_msgy);

    redraw();
label_2711_internal:
    await(config::instance().wait1);
    key_check();
    cursor_check();
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 2)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 2;
                }
            }
            key = "";
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    return turn_result_t::show_message_log;
                }
                if (curmenu == 1)
                {
                    return turn_result_t::menu_journal;
                }
                if (curmenu == 2)
                {
                    return turn_result_t::show_chat_history;
                }
            }
        }
    }
    if (key != ""s)
    {
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    goto label_2711_internal;
}

void load_showroom_user_info()
{
    notesel(headtemp);
    noteget(s, 1);
    username = ""s + s;
    noteget(s, 5);
    usermsg = ""s + s;
    noteget(s, 6);
    userrelation = elona::stoi(s(0));
    return;
}

int cnvjkey(const std::string& prm_1092)
{
    int p_at_m198 = 0;
    p_at_m198 = -2;
    for (int cnt = 0; cnt < 12; ++cnt)
    {
        if (prm_1092 == jkey(cnt))
        {
            p_at_m198 = cnt;
            break;
        }
    }
    return p_at_m198 + 1;
}

turn_result_t play_scene()
{
    auto result = ui::ui_menu_scene().show();

    if (result.canceled)
    {
        return turn_result_t::pc_turn_user_error;
    }
    else
    {
        return turn_result_t::play_scene;
    }
}

turn_result_t show_spell_list()
{
    listmax = 0;
    page = 0;
    pagesize = 16;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    cs = commark(1) % 1000;
    page = commark(1) / 1000;
    curmenu = 0;
    for (int cnt = 0; cnt < 200; ++cnt)
    {
        if (spell(cnt) > 0)
        {
            list(0, listmax) = cnt + 400;
            list(1, listmax) = the_ability_db[400 + cnt]
                                   ->related_basic_attribute; // TODO coupling
            ++listmax;
        }
    }
    sort_list_by_column1();
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_spell.bmp", 1);
    gsel(0);
    windowshadow = 1;
label_2028_internal:
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
    drawmenu(1);
label_2029_internal:
    s(0) = i18n::s.get("core.locale.ui.spell.title");
    s(1) = strhint2 + strhint3 + strhint7;
    display_window((windoww - 720) / 2 + inf_screenx, winposy(438), 720, 438);
    display_topic(i18n::s.get("core.locale.ui.spell.name"), wx + 28, wy + 36);
    display_topic(
        i18n::s.get("core.locale.ui.spell.cost") + "("
            + i18n::s.get("core.locale.ui.spell.stock") + ")" + " "
            + i18n::s.get("core.locale.ui.spell.lv_chance"),
        wx + 220,
        wy + 36);
    display_topic(
        i18n::s.get("core.locale.ui.spell.effect"), wx + 400, wy + 36);
    pos(wx + 46, wy - 16);
    gcopy(3, 912, 48, 48, 48);
    pos(wx + ww - 78, wy);
    gcopy(3, 960, 96, 72, 144);
    pos(wx + ww - 180, wy);
    gcopy(3, 1032, 96, 72, 96);
    keyrange = 0;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_select(cnt);
        ++keyrange;
        if (cnt % 2 == 0)
        {
            boxf(wx + 70, wy + 66 + cnt * 19, 620, 18, {12, 14, 16, 16});
        }
        display_key(wx + 58, wy + 66 + cnt * 19 - 2, cnt);
    }
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        i = list(0, p);
        pos(wx + 40, wy + 74 + cnt * 19);
        gmode(2);
        gcopy_c(
            1,
            (the_ability_db[i]->related_basic_attribute - 10) * inf_tiles,
            672,
            inf_tiles,
            inf_tiles);
        s = "";
        for (int cnt = 0; cnt < 20; ++cnt)
        {
            if (gdata(40 + cnt) == list(0, p))
            {
                s = u8"{"s + cnt + u8"}"s;
            }
        }
        cs_list(
            cs == cnt,
            i18n::_(u8"ability", std::to_string(i), u8"name") + s,
            wx + 84,
            wy + 66 + cnt * 19 - 1);
        s = ""s + calcspellcostmp(i, cc) + u8" ("s + spell((i - 400)) + u8")"s;
        pos(wx + 328 - strlen_u(s) * 7, wy + 66 + cnt * 19 + 2);
        mes(s);
        draw_spell_power_entry();
        s = strmid(s, 0, 40);
        pos(wx + 340, wy + 66 + cnt * 19 + 2);
        mes(""s + sdata(i, cc) + u8"/"s + calcspellfail(i, cc) + u8"%"s);
        pos(wx + 420, wy + 66 + cnt * 19 + 2);
        mes(s);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    commark(1) = page * 1000 + cs;
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (p != -1)
    {
        menucycle = 0;
        efid = p;
        return do_cast_command();
    }
    if (key == u8"sc"s)
    {
        snd(20);
        p = list(0, pagesize * page + cs);
        if (gdata(40 + sc) == p)
        {
            gdata(40 + sc) = 0;
            goto label_2028_internal;
        }
        for (int cnt = 0; cnt < 20; ++cnt)
        {
            if (gdata(40 + cnt) == p)
            {
                gdata(40 + cnt) = 0;
            }
        }
        gdata(40 + sc) = p;
        txt(lang(
            u8"{"s + sc + u8"}キーにショートカットを割り当てた。"s,
            u8"You have assigned the shortcut to {"s + sc + u8"} key."s));
        display_msg(inf_screeny + inf_tiles);
        goto label_2028_internal;
    }
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 1)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 1;
                }
            }
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    snd(102);
                    return turn_result_t::show_spell_list;
                }
                if (curmenu == 1)
                {
                    snd(101);
                    return turn_result_t::show_skill_list;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_2028_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_2028_internal;
        }
    }
    if (key == key_cancel)
    {
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    goto label_2029_internal;
}



turn_result_t show_skill_list()
{
    listmax = 0;
    page = 0;
    pagesize = 16;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    cs = commark(0) % 1000;
    page = commark(0) / 1000;
    curmenu = 1;
    for (int cnt = 300; cnt < 400; ++cnt)
    {
        if (sdata(cnt, cc) > 0)
        {
            list(0, listmax) = cnt;
            list(1, listmax) =
                the_ability_db[cnt]->related_basic_attribute * 1000 + cnt;
            ++listmax;
        }
    }
    for (int cnt = 0; cnt < 61; ++cnt)
    {
        if (spact(cnt) != 0)
        {
            list(0, listmax) = cnt + 600;
            list(1, listmax) =
                the_ability_db[cnt + 600]->related_basic_attribute * 1000 + cnt;
            ++listmax;
        }
    }
    sort_list_by_column1();
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_skill.bmp", 1);
    gsel(0);
    windowshadow = 1;
label_2008_internal:
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
    drawmenu(1);
label_2009_internal:
    s(0) = i18n::s.get("core.locale.ui.skill.title");
    s(1) = strhint2 + strhint3 + strhint7;
    display_window(
        (windoww - 600) / 2 + inf_screenx, winposy(438), 600, 438, 0, 60);
    display_topic(i18n::s.get("core.locale.ui.skill.name"), wx + 28, wy + 36);
    display_topic(i18n::s.get("core.locale.ui.skill.cost"), wx + 220, wy + 36);
    display_topic(
        i18n::s.get("core.locale.ui.skill.detail"), wx + 320, wy + 36);
    pos(wx + 46, wy - 16);
    gcopy(3, 960, 48, 48, 48);
    pos(wx + ww - 78, wy + wh - 165);
    gcopy(3, 960, 96, 72, 144);
    pos(wx + ww - 168, wy);
    gcopy(3, 1032, 96, 102, 48);
    keyrange = 0;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_select(cnt);
        ++keyrange;
        if (cnt % 2 == 0)
        {
            boxf(wx + 70, wy + 66 + cnt * 19, 490, 18, {12, 14, 16, 16});
        }
        display_key(wx + 58, wy + 66 + cnt * 19 - 2, cnt);
    }
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        i = list(0, p);
        pos(wx + 40, wy + 74 + cnt * 19);
        gmode(2);
        gcopy_c(
            1,
            (the_ability_db[list(0, p)]->related_basic_attribute - 10)
                * inf_tiles,
            672,
            inf_tiles,
            inf_tiles);
        s = "";
        for (int cnt = 0; cnt < 20; ++cnt)
        {
            if (gdata(40 + cnt) == list(0, p))
            {
                s = u8"{"s + cnt + u8"}"s;
            }
        }
        cs_list(
            cs == cnt,
            i18n::_(u8"ability", std::to_string(list(0, p)), u8"name") + s,
            wx + 84,
            wy + 66 + cnt * 19 - 1);
        s = ""s + the_ability_db[list(0, p)]->cost + u8" Sp"s;
        pos(wx + 288 - strlen_u(s) * 7, wy + 66 + cnt * 19 + 2);
        mes(s);
        draw_spell_power_entry();
        pos(wx + 325, wy + 66 + cnt * 19 + 2);
        mes(strmid(s, 0, 34));
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    commark(0) = page * 1000 + cs;
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (p != -1)
    {
        menucycle = 0;
        efid = p;
        screenupdate = -1;
        update_screen();
        return do_use_magic();
    }
    if (key == u8"sc"s)
    {
        snd(20);
        p = list(0, pagesize * page + cs);
        if (gdata(40 + sc) == p)
        {
            gdata(40 + sc) = 0;
            goto label_2008_internal;
        }
        for (int cnt = 0; cnt < 20; ++cnt)
        {
            if (gdata(40 + cnt) == p)
            {
                gdata(40 + cnt) = 0;
            }
        }
        gdata(40 + sc) = p;
        txt(lang(
            u8"{"s + sc + u8"}キーにショートカットを割り当てた。"s,
            u8"You have assigned the shortcut to {"s + sc + u8"} key."s));
        display_msg(inf_screeny + inf_tiles);
        goto label_2008_internal;
    }
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 1)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 1;
                }
            }
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    snd(102);
                    return turn_result_t::show_spell_list;
                }
                if (curmenu == 1)
                {
                    snd(101);
                    return turn_result_t::show_skill_list;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_2008_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_2008_internal;
        }
    }
    if (key == key_cancel)
    {
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    goto label_2009_internal;
}



void draw_spell_power_entry()
{
    s = "";
    if (the_ability_db[i]->sdataref1 / 1000 == 1)
    {
        p = the_ability_db[i]->sdataref1 % 1000;
        const auto duration = calc_buff_duration(p, calcspellpower(i, cc));
        const auto description = get_buff_description(p, calcspellpower(i, cc));
        s = ""s + duration + i18n::s.get("core.locale.ui.spell.turn_counter")
            + description;
        return;
    }
    const auto damage = calc_skill_damage(i, cc, calcspellpower(i, cc));
    if (damage)
    {
        dice1 = damage->dice_x;
        dice2 = damage->dice_y;
        bonus = damage->damage_bonus;
        ele = damage->element;
        elep = damage->element_power;
        if (cc == 0)
        {
            if (trait(165) != 0)
            {
                if (ele == 50 || ele == 51 || ele == 52)
                {
                    dice2 = dice2 * 125 / 100;
                }
            }
        }
        if (dice1 != 0)
        {
            s += ""s + dice1 + u8"d"s + dice2;
            if (bonus != 0)
            {
                if (bonus > 0)
                {
                    s += u8"+"s + bonus;
                }
                else
                {
                    s += bonus;
                }
            }
        }
        else if (i == 461)
        {
            s += ""s + clamp(bonus, 1, 100) + u8"%"s;
        }
        else
        {
            s += i18n::s.get("core.locale.ui.spell.power") + bonus;
        }
        s += u8" "s;
    }
    s += i18n::_(u8"ability", std::to_string(i), u8"description");
    return;
}

void trainer_get_gainable_skills()
{
    int dbmax = 0;
    dblist(0, dbmax) = 159;
    ++dbmax;
    dblist(0, dbmax) = 173;
    ++dbmax;
    if (gdata_current_map == mdata_t::map_id_t::yowyn)
    {
        dblist(0, dbmax) = 154;
        ++dbmax;
        dblist(0, dbmax) = 162;
        ++dbmax;
        dblist(0, dbmax) = 184;
        ++dbmax;
        dblist(0, dbmax) = 176;
        ++dbmax;
        dblist(0, dbmax) = 177;
        ++dbmax;
        dblist(0, dbmax) = 180;
        ++dbmax;
        dblist(0, dbmax) = 301;
        ++dbmax;
        dblist(0, dbmax) = 167;
        ++dbmax;
        dblist(0, dbmax) = 153;
        ++dbmax;
    }
    if (gdata_current_map == mdata_t::map_id_t::vernis)
    {
        dblist(0, dbmax) = 178;
        ++dbmax;
        dblist(0, dbmax) = 154;
        ++dbmax;
        dblist(0, dbmax) = 184;
        ++dbmax;
        dblist(0, dbmax) = 163;
        ++dbmax;
        dblist(0, dbmax) = 182;
        ++dbmax;
        dblist(0, dbmax) = 185;
        ++dbmax;
        dblist(0, dbmax) = 158;
        ++dbmax;
    }
    if (gdata_current_map == mdata_t::map_id_t::palmia)
    {
        dblist(0, dbmax) = 151;
        ++dbmax;
        dblist(0, dbmax) = 150;
        ++dbmax;
        dblist(0, dbmax) = 161;
        ++dbmax;
        dblist(0, dbmax) = 179;
        ++dbmax;
        dblist(0, dbmax) = 301;
        ++dbmax;
        dblist(0, dbmax) = 156;
        ++dbmax;
        dblist(0, dbmax) = 160;
        ++dbmax;
        dblist(0, dbmax) = 172;
        ++dbmax;
        dblist(0, dbmax) = 178;
        ++dbmax;
    }
    if (gdata_current_map == mdata_t::map_id_t::noyel)
    {
        dblist(0, dbmax) = 181;
        ++dbmax;
        dblist(0, dbmax) = 150;
        ++dbmax;
        dblist(0, dbmax) = 162;
        ++dbmax;
        dblist(0, dbmax) = 184;
        ++dbmax;
        dblist(0, dbmax) = 182;
        ++dbmax;
        dblist(0, dbmax) = 155;
        ++dbmax;
        dblist(0, dbmax) = 153;
        ++dbmax;
    }
    if (gdata_current_map == mdata_t::map_id_t::lumiest)
    {
        if (gdata_current_dungeon_level == 1)
        {
            dblist(0, dbmax) = 172;
            ++dbmax;
            dblist(0, dbmax) = 154;
            ++dbmax;
            dblist(0, dbmax) = 150;
            ++dbmax;
            dblist(0, dbmax) = 174;
            ++dbmax;
            dblist(0, dbmax) = 185;
            ++dbmax;
            dblist(0, dbmax) = 171;
            ++dbmax;
            dblist(0, dbmax) = 160;
            ++dbmax;
            dblist(0, dbmax) = 167;
            ++dbmax;
        }
    }
    if (gdata_current_map == mdata_t::map_id_t::lumiest)
    {
        if (gdata_current_dungeon_level == 3)
        {
            dblist(0, dbmax) = 172;
            ++dbmax;
            dblist(0, dbmax) = 165;
            ++dbmax;
            dblist(0, dbmax) = 155;
            ++dbmax;
            dblist(0, dbmax) = 164;
            ++dbmax;
            dblist(0, dbmax) = 178;
            ++dbmax;
            dblist(0, dbmax) = 188;
            ++dbmax;
            dblist(0, dbmax) = 171;
            ++dbmax;
            dblist(0, dbmax) = 187;
            ++dbmax;
        }
    }
    if (gdata_current_map == mdata_t::map_id_t::derphy)
    {
        if (gdata_current_dungeon_level == 1)
        {
            dblist(0, dbmax) = 158;
            ++dbmax;
            dblist(0, dbmax) = 157;
            ++dbmax;
            dblist(0, dbmax) = 170;
            ++dbmax;
            dblist(0, dbmax) = 171;
            ++dbmax;
            dblist(0, dbmax) = 176;
            ++dbmax;
            dblist(0, dbmax) = 174;
            ++dbmax;
            dblist(0, dbmax) = 183;
            ++dbmax;
            dblist(0, dbmax) = 163;
            ++dbmax;
        }
    }
    if (gdata_current_map == mdata_t::map_id_t::derphy)
    {
        if (gdata_current_dungeon_level == 3)
        {
            dblist(0, dbmax) = 300;
            ++dbmax;
            dblist(0, dbmax) = 175;
            ++dbmax;
            dblist(0, dbmax) = 158;
            ++dbmax;
            dblist(0, dbmax) = 157;
            ++dbmax;
            dblist(0, dbmax) = 189;
            ++dbmax;
        }
    }
    if (gdata_current_map == mdata_t::map_id_t::port_kapul)
    {
        if (gdata_current_dungeon_level == 1)
        {
            dblist(0, dbmax) = 156;
            ++dbmax;
            dblist(0, dbmax) = 160;
            ++dbmax;
            dblist(0, dbmax) = 183;
            ++dbmax;
            dblist(0, dbmax) = 185;
            ++dbmax;
            dblist(0, dbmax) = 174;
            ++dbmax;
            dblist(0, dbmax) = 151;
            ++dbmax;
        }
    }
    if (gdata_current_map == mdata_t::map_id_t::port_kapul)
    {
        if (gdata_current_dungeon_level == 3)
        {
            dblist(0, dbmax) = 153;
            ++dbmax;
            dblist(0, dbmax) = 166;
            ++dbmax;
            dblist(0, dbmax) = 167;
            ++dbmax;
            dblist(0, dbmax) = 169;
            ++dbmax;
            dblist(0, dbmax) = 152;
            ++dbmax;
            dblist(0, dbmax) = 189;
            ++dbmax;
            dblist(0, dbmax) = 168;
            ++dbmax;
            dblist(0, dbmax) = 186;
            ++dbmax;
        }
    }
    for (int cnt = 0, cnt_end = (dbmax); cnt < cnt_end; ++cnt)
    {
        p = dblist(0, cnt);
        f = 0;
        if (sdata.get(p, cc).original_level == 0)
        {
            if (the_ability_db[p]->related_basic_attribute != 0)
            {
                f = 1;
            }
        }
        if (f)
        {
            list(0, listmax) = p;
            list(1, listmax) =
                the_ability_db[p]->related_basic_attribute + 21000;
            ++listmax;
        }
    }
    return;
}

menu_result menu_character_sheet()
{
    menu_result result = {false, false, turn_result_t::none};
    int cs_buff = 0;
    int returnfromportrait = 0;
    int cs_buffmax = 0;
label_20331:
    page = 0;
    pagesize = 16;
    listmax = 0;
    cs = 0;
    if (csctrl != 4)
    {
        cc = 0;
    }
    csskill = -1;
    cs_buff = 0;
    curmenu = 0;
    if (csctrl == 2 || csctrl == 3)
    {
        page = 1;
    }
    list(0, listmax) = -1;
    list(1, listmax) = 20000;
    listn(0, listmax) =
        i18n::s.get("core.locale.ui.chara_sheet.category.skill");
    ++listmax;
    if (csctrl == 3)
    {
        trainer_get_gainable_skills();
    }
    else
    {
        for (int cnt = 150; cnt < 400; ++cnt)
        {
            f = 0;
            if (sdata(cnt, cc) != 0)
            {
                f = 1;
                if (csctrl == 2)
                {
                    if (sdata.get(cnt, cc).original_level == 0)
                    {
                        f = 0;
                    }
                }
            }
            if (f)
            {
                list(0, listmax) = cnt;
                list(1, listmax) =
                    the_ability_db[cnt]->related_basic_attribute + 21000;
                ++listmax;
            }
        }
    }
    list(0, listmax) = -1;
    list(1, listmax) = 30000;
    listn(0, listmax) =
        i18n::s.get("core.locale.ui.chara_sheet.category.weapon_proficiency");
    ++listmax;
    for (int cnt = 100; cnt < 150; ++cnt)
    {
        f = 0;
        if (csctrl != 3)
        {
            if (sdata(cnt, cc) != 0)
            {
                f = 1;
            }
        }
        else if (sdata(cnt, cc) == 0)
        {
            if (the_ability_db[cnt])
            {
                f = 1;
            }
        }
        if (f)
        {
            list(0, listmax) = cnt;
            list(1, listmax) =
                the_ability_db[cnt]->related_basic_attribute + 31000;
            ++listmax;
        }
    }
    if (csctrl != 2 && csctrl != 3)
    {
        list(0, listmax) = -1;
        list(1, listmax) = 40000;
        listn(0, listmax) =
            i18n::s.get("core.locale.ui.chara_sheet.category.resistance");
        ++listmax;
        for (int cnt = 50; cnt < 100; ++cnt)
        {
            if (sdata(cnt, cc) != 0)
            {
                list(0, listmax) = cnt;
                list(1, listmax) =
                    the_ability_db[cnt]->related_basic_attribute + 41000;
                ++listmax;
            }
        }
    }
    sort_list_by_column1();
    gsel(7);
    picload(filesystem::dir::graphic() / u8"ie_sheet.bmp");
    gsel(0);
    wx = (windoww - 700) / 2 + inf_screenx;
    wy = winposy(400) - 10;
    ww = 700;
    wh = 400;
    s = i18n::s.get("core.locale.ui.chara_sheet.title.default");
    if (csctrl == 2)
    {
        s = i18n::s.get("core.locale.ui.chara_sheet.title.training");
    }
    if (csctrl == 3)
    {
        s = i18n::s.get("core.locale.ui.chara_sheet.title.learning");
    }
    drawmenu(0);
    if (mode != 1)
    {
        snd(94);
    }
    window_animation(wx, wy, ww, wh, 9, 4);
    gsel(4);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"face1.bmp", 1);
    if (cdata[cc].portrait < 0) // TODO coupling
    {
        const auto filepath = filesystem::dir::user() / u8"graphic"
            / (u8"face"s + std::abs(cdata[cc].portrait + 1) + u8".bmp");
        if (cdata[cc].portrait != -1)
        {
            if (fs::exists(filepath))
            {
                pos(0, 0);
                picload(filepath, 1);
            }
        }
    }
    gsel(0);
    if (returnfromportrait == 0)
    {
        gmode(6, 80);
        pos(wx + 4, wy + 4);
        gcopy(7, 0, 0, 700, 400);
        gmode(2);
    }
    if (csctrl == 2)
    {
        txtnew();
        txt(i18n::s.get("core.locale.ui.chara_sheet.train_which_skill"));
    }
    returnfromportrait = 0;
label_2034_internal:
    if (csctrl != 1)
    {
        display_msg(inf_tiles + inf_screeny);
    }
    pagemax = (listmax - 1) / pagesize + 1;
    if (page < 0)
    {
        page = pagemax;
    }
    else if (page > pagemax)
    {
        page = 0;
    }
    if (csctrl == 1)
    {
        pagemax = 0;
    }
    cs_bk = -1;
    if (csctrl == 0)
    {
        if (page == 0)
        {
            s = i18n::s.get("core.locale.ui.chara_sheet.hint.hint") + strhint6
                + strhint2 + strhint3;
        }
        else
        {
            s = i18n::s.get("core.locale.ui.chara_sheet.hint.spend_bonus")
                + strhint2 + strhint3;
        }
    }
    if (csctrl == 1)
    {
        s = i18n::s.get("core.locale.ui.chara_sheet.hint.reroll") + strhint6
            + i18n::s.get("core.locale.ui.chara_sheet.hint.confirm");
    }
    if (csctrl == 2)
    {
        if (page == 0)
        {
            s = strhint6 + strhint2 + strhint3;
        }
        else
        {
            s = i18n::s.get("core.locale.ui.chara_sheet.hint.train_skill")
                + strhint2 + strhint3;
        }
    }
    if (csctrl == 3)
    {
        if (page == 0)
        {
            s = strhint6 + strhint2 + strhint3;
        }
        else
        {
            s = i18n::s.get("core.locale.ui.chara_sheet.hint.learn_skill")
                + strhint2 + strhint3;
        }
    }
    if (csctrl == 4)
    {
        if (page == 0)
        {
            s = i18n::s.get("core.locale.ui.chara_sheet.hint.blessing_info")
                + strhint6 + strhint2 + strhint3;
        }
        else
        {
            s = strhint2 + strhint3;
        }
    }
    if (csctrl != 1)
    {
        if (page != 0)
        {
            s += ""s + key_mode2 + u8" ["s
                + i18n::s.get("core.locale.ui.chara_sheet.hint.track_skill")
                + u8"]"s;
        }
    }
    color(0, 0, 0);
    show_title(s);
label_2035_internal:
    s = "";
    if (csctrl == 0)
    {
        if (page != 0)
        {
            s = i18n::s.get(
                "core.locale.ui.chara_sheet.you_can_spend_bonus",
                cdata[cc].skill_bonus);
        }
    }
    display_window2(
        (windoww - 700) / 2 + inf_screenx, winposy(400) - 10, 700, 400, 7);
    if (page == 0)
    {
        keyrange = 0;
        key_list = key_enter;
        display_topic(
            i18n::s.get("core.locale.ui.chara_sheet.attributes"),
            wx + 28,
            wy + 122);
        display_topic(
            i18n::s.get("core.locale.ui.chara_sheet.combat_rolls"),
            wx + 400,
            wy + 253);
        display_topic(
            i18n::s.get("core.locale.ui.chara_sheet.history"),
            wx + 28,
            wy + 273);
        display_topic(
            i18n::s.get("core.locale.ui.chara_sheet.blessing_and_hex"),
            wx + 400,
            wy + 122);
        display_topic(
            i18n::s.get("core.locale.ui.chara_sheet.extra_info"),
            wx + 220,
            wy + 273);
        if (cdata[cc].portrait >= 0)
        {
            p = cdata[cc].sex * 64 + cdata[cc].portrait;
            pos(wx + 560, wy + 27);
            gcopy(4, p % 16 * 48, p / 16 * 72, 48, 72, 80, 112);
        }
        else
        {
            const auto filepath = filesystem::dir::user() / u8"graphic"
                / (u8"face"s + std::abs(cdata[cc].portrait + 1) + u8".bmp");
            if (cdata[cc].portrait != -1)
            {
                if (fs::exists(filepath))
                {
                    pos(wx + 560, wy + 27);
                    gcopy(4, 0, 0, 80, 112, 80, 112);
                }
            }
        }
        window2(wx + 557, wy + 23, 87, 120, 1, 10);
        if (cdata[cc].has_own_sprite() == 1)
        {
            pos(wx + 596 + 22, wy + 86 + 24);
            gmode(2);
            gcopy_c(20 + cc, 32, 0, 32, 48, 24, 40);
        }
        else
        {
            draw_chara_scale_height(cdata[cc], wx + 596 + 22, wy + 86 + 24);
        }
        font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
        s(0) = i18n::s.get("core.locale.ui.chara_sheet.exp.level");
        s(1) = i18n::s.get("core.locale.ui.chara_sheet.exp.exp");
        s(2) = i18n::s.get("core.locale.ui.chara_sheet.exp.next_level");
        s(3) = i18n::s.get("core.locale.ui.chara_sheet.exp.god");
        s(4) = i18n::s.get("core.locale.ui.chara_sheet.exp.guild");
        for (int cnt = 0; cnt < 5; ++cnt)
        {
            pos(wx + 355, wy + 46 + cnt * 15);
            color(20, 10, 0);
            mes(s(cnt));
            color(0, 0, 0);
        }
        s(0) = i18n::s.get("core.locale.ui.chara_sheet.personal.name");
        s(1) = i18n::s.get("core.locale.ui.chara_sheet.personal.aka");
        s(2) = i18n::s.get("core.locale.ui.chara_sheet.personal.race");
        s(3) = i18n::s.get("core.locale.ui.chara_sheet.personal.sex");
        s(4) = i18n::s.get("core.locale.ui.chara_sheet.personal.class");
        s(5) = i18n::s.get("core.locale.ui.chara_sheet.personal.age");
        s(6) = i18n::s.get("core.locale.ui.chara_sheet.personal.height");
        s(7) = i18n::s.get("core.locale.ui.chara_sheet.personal.weight");
        for (int cnt = 0; cnt < 8; ++cnt)
        {
            pos(wx + 30 + cnt / 4 * 190, wy + 61 + cnt % 4 * 15);
            color(20, 10, 0);
            mes(s(cnt));
            color(0, 0, 0);
        }
        for (int cnt = 0; cnt < 8; ++cnt)
        {
            pos(wx + 37, wy + 157 + cnt * 15);
            gmode(2);
            gcopy_c(1, cnt * inf_tiles, 672, inf_tiles, inf_tiles);
            pos(wx + 54, wy + 151 + cnt * 15);
            color(20, 10, 0);
            mes(i18n::_(u8"ui", u8"attribute", u8"_"s + cnt));
            color(0, 0, 0);
        }
        s(0) = i18n::s.get("core.locale.ui.chara_sheet.attribute.life");
        s(1) = i18n::s.get("core.locale.ui.chara_sheet.attribute.mana");
        s(2) = i18n::s.get("core.locale.ui.chara_sheet.attribute.sanity");
        s(3) = i18n::s.get("core.locale.ui.chara_sheet.attribute.speed");
        s(4) = "";
        s(5) = i18n::s.get("core.locale.ui.chara_sheet.attribute.fame");
        s(6) = i18n::s.get("core.locale.ui.chara_sheet.attribute.karma");
        s(7) = "";
        s(8) = i18n::s.get("core.locale.ui.chara_sheet.attribute.melee");
        s(9) = i18n::s.get("core.locale.ui.chara_sheet.attribute.shoot");
        s(10) = "";
        s(11) = "";
        s(12) = "";
        s(13) = "";
        s(14) = "";
        s(15) = "";
        for (int cnt = 0; cnt < 8; ++cnt)
        {
            pos(wx + 255, wy + 151 + cnt * 15);
            color(20, 10, 0);
            mes(s(cnt));
            color(0, 0, 0);
        }
        s(0) = i18n::s.get("core.locale.ui.chara_sheet.time.turns");
        s(1) = i18n::s.get("core.locale.ui.chara_sheet.time.days");
        s(2) = i18n::s.get("core.locale.ui.chara_sheet.time.kills");
        s(3) = i18n::s.get("core.locale.ui.chara_sheet.time.time");
        s(4) = "";
        s(5) = "";
        for (int cnt = 0; cnt < 5; ++cnt)
        {
            pos(wx + 32, wy + 301 + cnt * 15);
            color(20, 10, 0);
            mes(s(cnt));
            color(0, 0, 0);
        }
        s(0) = i18n::s.get("core.locale.ui.chara_sheet.weight.cargo_weight");
        s(1) = i18n::s.get("core.locale.ui.chara_sheet.weight.cargo_limit");
        s(2) = i18n::s.get("core.locale.ui.chara_sheet.weight.equip_weight");
        s(3) = i18n::s.get("core.locale.ui.chara_sheet.weight.deepest_level");
        for (int cnt = 0; cnt < 4; ++cnt)
        {
            pos(wx + 224, wy + 301 + cnt * 15);
            color(20, 10, 0);
            mes(s(cnt));
            color(0, 0, 0);
        }
        font(14 - en * 2);
        s(0) = ""s + cdata[cc].level;
        s(1) = ""s + cdata[cc].experience;
        s(2) = ""s + cdata[cc].required_experience;
        s(3) = i18n::_(u8"god", cdata[cc].god_id, u8"name");
        s(4) = guildname();
        for (int cnt = 0; cnt < 5; ++cnt)
        {
            pos(wx + 410 + en * 5, wy + 45 + cnt * 15);
            color(20, 10, 0);
            mes(s(cnt));
            color(0, 0, 0);
        }
        s(0) = cdatan(0, cc);
        s(1) = cdatan(1, cc);
        s(2) = cnven(lang(
            ""s + i18n::_(u8"race", cdatan(2, cc), u8"name"), cdatan(2, cc)));
        access_class_info(2, cdatan(3, cc));
        s(4) = cnven(lang(""s + classname, cdatan(3, cc)));
        if (cdata[cc].sex == 0)
        {
            s(3) = cnven(i18n::_(u8"ui", u8"male"));
        }
        else
        {
            s(3) = cnven(i18n::_(u8"ui", u8"female"));
        }
        s(5) = ""s + calcage(cc) + u8" "s
            + i18n::s.get("core.locale.ui.chara_sheet.personal.age_counter");
        s(6) = ""s + cdata[cc].height + u8" cm"s;
        s(7) = ""s + cdata[cc].weight + u8" kg"s;
        for (int cnt = 0; cnt < 8; ++cnt)
        {
            pos(wx + 68 + cnt / 4 * 190 + en * ((cnt > 3) * 12),
                wy + 60 + cnt % 4 * 15);
            color(20, 10, 0);
            mes(s(cnt));
            color(0, 0, 0);
        }
        for (int cnt = 0; cnt < 8; ++cnt)
        {
            s = u8"("s + sdata.get(10 + cnt, cc).original_level + u8")"s;
            if (encfind(cc, 60010 + cnt) != -1)
            {
                s += u8"*"s;
            }
            pos(wx + 92, wy + 151 + cnt * 15);
            color(20, 10, 0);
            mes(""s + sdata((10 + cnt), cc));
            color(0, 0, 0);
            pos(wx + 124, wy + 151 + cnt * 15);
            color(20, 10, 0);
            mes(s);
            color(0, 0, 0);
            p = sdata.get(10 + cnt, cc).potential;
            pos(wx + 176, wy + 152 + cnt * 15);
            if (p >= 200)
            {
                color(20, 10, 0);
                mes(u8"Superb"s);
                color(0, 0, 0);
                continue;
            }
            if (p >= 150)
            {
                color(20, 10, 0);
                mes(u8"Great"s);
                color(0, 0, 0);
                continue;
            }
            if (p >= 100)
            {
                color(20, 10, 0);
                mes(u8"Good"s);
                color(0, 0, 0);
                continue;
            }
            if (p >= 50)
            {
                color(20, 10, 0);
                mes(u8"Bad"s);
                color(0, 0, 0);
                continue;
            }
            mes(u8"Hopeless"s);
            color(0, 0, 0);
        }
        s(0) = ""s + sdata(2, cc) + u8"("s + sdata.get(2, cc).original_level
            + u8")"s;
        s(1) = ""s + sdata(3, cc) + u8"("s + sdata.get(3, cc).original_level
            + u8")"s;
        s(2) = ""s + cdata[cc].insanity;
        s(3) = ""s + cdata[cc].current_speed + u8"("s
            + sdata.get(18, cc).original_level + u8")"s;
        s(4) = "";
        s(5) = ""s + cdata[cc].fame;
        s(6) = ""s + cdata[cc].karma;
        s(7) = "";
        s(8) = "";
        for (int cnt = 0; cnt < 8; ++cnt)
        {
            color(20, 10, 0);
            pos(wx + 310, wy + 151 + cnt * 15);
            mes(s(cnt));
            color(0, 0, 0);
        }
        append_accuracy_info(0);
        tc = cc;
        font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
        color(20, 10, 0);
        pos(wx + 417, wy + 281 + p(2) * 16);
        mes(i18n::s.get("core.locale.ui.chara_sheet.damage.protect"));
        color(0, 0, 0);
        color(20, 10, 0);
        pos(wx + 590 - en * 16, wy + 281 + p(2) * 16);
        mes(i18n::s.get("core.locale.ui.chara_sheet.damage.evade"));
        color(0, 0, 0);
        attackskill = 106;
        int evade = calc_evasion(tc);
        prot = calcattackdmg(2);
        font(14 - en * 2);
        pos(wx + 460 + en * 8, wy + 279 + p(2) * 16);
        mes(""s + (100 - 10000 / (prot + 100)) + u8"% + "s + protdice1 + u8"d"s
            + protdice2);
        pos(wx + 625 - en * 8, wy + 279 + p(2) * 16);
        mes(""s + evade + u8"%"s);
        ++p(2);
        s(0) = i18n::s.get(
            "core.locale.ui.chara_sheet.time.turn_counter", gdata_play_turns);
        s(1) = i18n::s.get(
            "core.locale.ui.chara_sheet.time.days_counter", gdata_play_days);
        s(2) = ""s + gdata_kill_count;
        s(3) = ""s
            + cnvplaytime(
                   (gdata_play_time + timeGetTime() / 1000 - time_begin));
        s(4) = "";
        s(5) = "";
        for (int cnt = 0; cnt < 5; ++cnt)
        {
            pos(wx + 80, wy + 299 + cnt * 15);
            mes(s(cnt));
        }
        s(0) = ""s + cnvweight(gdata_cargo_weight);
        s(1) = cnvweight(gdata_current_cart_limit);
        s(2) = cnvweight(cdata[cc].sum_of_equipment_weight) + u8" "s
            + cnveqweight(cc);
        s(3) = i18n::s.get(
            "core.locale.ui.chara_sheet.weight.level_counter",
            cnvrank(gdata_deepest_dungeon_level));
        for (int cnt = 0; cnt < 4; ++cnt)
        {
            pos(wx + 287 + en * 14, wy + 299 + cnt * 15);
            mes(s(cnt));
        }
        cs_buffmax = 0;
        for (int cnt = 0; cnt < 15; ++cnt)
        {
            x = wx + 430 + cnt / 3 * 40;
            y = wy + 151 + cnt % 3 * 32;
            if (cdata[cc].buffs[cnt].id == 0)
            {
                gmode(4, 120);
                draw("buff_icon_none", x, y);
                gmode(2);
                continue;
            }
            ++cs_buffmax;
            pos(x, y);
            gcopy(5, cdata[cc].buffs[cnt].id * 32, 1120, 32, 32);
            if (cs_buff == cnt)
            {
                boxf(x, y, 32, 32, {200, 200, 255, 63});
            }
        }
        if (cs_buffmax != 0)
        {
            const auto duration = calc_buff_duration(
                cdata[cc].buffs[cs_buff].id, cdata[cc].buffs[cs_buff].power);
            const auto description = get_buff_description(
                cdata[cc].buffs[cs_buff].id, cdata[cc].buffs[cs_buff].power);
            s = ""s
                + i18n::_(
                      u8"buff",
                      std::to_string(cdata[cc].buffs[cs_buff].id),
                      u8"name")
                + u8": "s + cdata[cc].buffs[cs_buff].turns
                + i18n::s.get(
                      "core.locale.ui.chara_sheet.buff.duration", duration)
                + description;
        }
        else
        {
            s = i18n::s.get("core.locale.ui.chara_sheet.buff.is_not_currently");
        }
        font(13 - en * 2);
        pos(wx + 108, wy + 366);
        mes(s);
        font(11 + sizefix * 2 - en * 2, snail::font_t::style_t::bold);
        color(20, 10, 0);
        pos(wx + 70, wy + 369 - en * 3);
        mes(i18n::s.get("core.locale.ui.chara_sheet.buff.hint") + ":");
        color(0, 0, 0);
    }
    else
    {
        display_topic(
            i18n::s.get("core.locale.ui.chara_sheet.skill.name"),
            wx + 28,
            wy + 36);
        display_topic(
            i18n::s.get("core.locale.ui.chara_sheet.skill.level") + "("
                + i18n::s.get("core.locale.ui.chara_sheet.skill.potential")
                + ")",
            wx + 182,
            wy + 36);
        display_topic(
            i18n::s.get("core.locale.ui.chara_sheet.skill.detail"),
            wx + 320,
            wy + 36);
        keyrange = 0;
        p(1) = 0;
        for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
        {
            p = pagesize * (page - 1) + cnt;
            if (p >= listmax)
            {
                break;
            }
            key_list(cnt) = key_select(cnt);
            ++keyrange;
            if (list(0, p) < 0)
            {
                p(1) = 1;
                continue;
            }
            ++p(1);
            if (list(0, p) < 100)
            {
                x = 52;
                dx = 18;
            }
            else
            {
                x = 70;
                dx = 0;
            }
            if (p(1) % 2 == 0)
            {
                boxf(
                    wx + x, wy + 66 + cnt * 19, 595 + dx, 18, {12, 14, 16, 16});
            }
            if (list(0, p) < 100)
            {
                continue;
            }
            display_key(wx + 58, wy + 64 + cnt * 19, cnt);
        }
    }
    if (page > 0)
    {
        font(14 - en * 2);
        cs_listbk();
        for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
        {
            p = pagesize * (page - 1) + cnt;
            if (p >= listmax)
            {
                break;
            }
            if (list(0, p) >= 0)
            {
                i = list(0, p);
                if (i < 100)
                {
                    x = 54;
                    p(1) = 10;
                }
                else
                {
                    x = 84;
                    p(1) = the_ability_db[i]->related_basic_attribute - 10;
                }
                pos(wx + 38, wy + 75 + cnt * 19);
                gmode(2);
                gcopy_c(1, p(1) * inf_tiles, 672, inf_tiles, inf_tiles);
                s = i18n::_(u8"ability", std::to_string(i), u8"name");
                if (i >= 50 && i < 100)
                {
                    s = i18n::s.get(
                        "core.locale.ui.chara_sheet.skill.resist", cnven(s));
                }
                for (int cnt = 0; cnt < 3; ++cnt)
                {
                    if (gdata(750 + cnt) == cc * 10000 + i)
                    {
                        s = u8"*"s + s;
                    }
                }
                cs_list(cs == cnt, s, wx + x, wy + 66 + cnt * 19 - 1);
                if (list(0, p) >= 50 && list(0, p) < 100)
                {
                    p(1) = clamp(sdata(list(0, p), cc) / 50, 0, 6);
                    s = i18n::_(u8"ui", u8"resistance", u8"_"s + p(1));
                }
                else
                {
                    s = ""s + sdata.get(i, cc).original_level + u8"."s
                        + std::to_string(
                              1000
                              + sdata.get(list(0, p), cc).experience % 1000)
                              .substr(1);
                    if (sdata.get(i, cc).original_level != sdata(i, cc))
                    {
                        p(1) = sdata(i, cc) - sdata.get(i, cc).original_level;
                    }
                    s += u8"("s + sdata.get(i, cc).potential + u8"%)"s;
                }
                pos(wx + 280 - strlen_u(s) * 7, wy + 66 + cnt * 19 + 2);
                mes(s);
                pos(wx + 330, wy + 66 + cnt * 19 + 2);
                mes(i18n::_(u8"ability", std::to_string(i), u8"description"));
                if (csctrl == 2 || csctrl == 3)
                {
                    if (csctrl == 2)
                    {
                        s = ""s + calctraincost(i, cc) + u8"p "s;
                    }
                    else
                    {
                        s = ""s + calclearncost(i, cc) + u8"p "s;
                    }
                    pos(wx + 322 - strlen_u(s) * 7, wy + 66 + cnt * 19 + 2);
                    mes(s);
                }
                else if (sdata.get(i, cc).original_level != sdata(i, cc))
                {
                    i = sdata(i, cc) - sdata.get(i, cc).original_level;
                    if (list(0, p) >= 50)
                    {
                        i = i / 50;
                    }
                    else
                    {
                        i = i / 5;
                    }
                    s = enchantment_print_level(i);
                    pos(wx + 282, wy + 66 + cnt * 19 + 2);
                    mes(s);
                }
            }
            else
            {
                font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
                cs_list(cs == cnt, listn(0, p), wx + 88, wy + 66 + cnt * 19);
                font(14 - en * 2);
            }
        }
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    if (page == 0)
    {
        if (key == u8"p"s)
        {
            if (cc < 16)
            {
                change_appearance();
                if (mode != 1)
                {
                    nowindowanime = 1;
                }
                returnfromportrait = 1;
                goto label_20331;
            }
        }
        if (key == key_north)
        {
            --cs_buff;
            if (cs_buff < 0)
            {
                cs_buff = cs_buffmax - 1;
            }
            goto label_2034_internal;
        }
        if (key == key_south)
        {
            ++cs_buff;
            if (cs_buff >= cs_buffmax)
            {
                cs_buff = 0;
            }
            goto label_2034_internal;
        }
    }
    else if (csctrl != 1)
    {
        if (key == key_mode2)
        {
            for (int cnt = 0, cnt_end = (keyrange); cnt < cnt_end; ++cnt)
            {
                i = list(0, pagesize * (page - 1) + cs);
                break;
            }
            if (i != -1)
            {
                p = 750;
                for (int cnt = 750; cnt < 753; ++cnt)
                {
                    if (gdata(cnt) % 10000 == 0)
                    {
                        p = cnt;
                    }
                    if (gdata(cnt) == cc * 10000 + i)
                    {
                        p = cnt;
                        i = 0;
                        break;
                    }
                }
                gdata(p) = cc * 10000 + i;
                snd(20);
            }
        }
    }
    if (csctrl == 1)
    {
        if (key == key_enter)
        {
            snd(103);
            result.succeeded = false;
            return result;
        }
        if (key == key_cancel)
        {
            result.succeeded = true;
            return result;
        }
        goto label_2035_internal;
    }
    p = -1;
    for (int cnt = 0, cnt_end = (keyrange); cnt < cnt_end; ++cnt)
    {
        if (key == key_select(cnt))
        {
            p = list(0, pagesize * (page - 1) + cnt);
            break;
        }
    }
    if (p != -1)
    {
        if (csctrl != 4)
        {
            csskill = p;
            if (csctrl == 2 || csctrl == 3)
            {
                screenupdate = -1;
                update_screen();
                tc = tcbk;
                result.succeeded = false;
                return result;
            }
            if (cdata.player().skill_bonus < 1 || p < 0 || p < 100)
            {
                goto label_2034_internal;
            }
            if (sdata.get(csskill, 0).original_level == 0)
            {
                snd(27);
                goto label_2034_internal;
            }
            --cdata.player().skill_bonus;
            snd(19);
            chara_gain_skill_exp(cdata[cc], csskill, 400, 2, 1000);
            modify_potential(
                cdata[cc],
                csskill,
                clamp(15 - sdata.get(csskill, cc).potential / 15, 2, 15));
            render_hud();
            goto label_2034_internal;
        }
    }
    if (key == key_pageup)
    {
        ++page;
        snd(1);
        goto label_2034_internal;
    }
    if (key == key_pagedown)
    {
        --page;
        snd(1);
        goto label_2034_internal;
    }
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 3)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 3;
                }
            }
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    csctrl = 0;
                    goto label_20331;
                }
                if (curmenu == 1)
                {
                    result.turn_result = turn_result_t::menu_equipment;
                    return result;
                }
                if (curmenu == 2)
                {
                    result.turn_result = turn_result_t::menu_feats;
                    return result;
                }
                if (curmenu == 3)
                {
                    result.turn_result = turn_result_t::menu_materials;
                    return result;
                }
            }
        }
    }
    if (key == key_cancel)
    {
        menucycle = 0;
        if (csctrl == 0)
        {
            update_screen();
            result.turn_result = turn_result_t::pc_turn_user_error;
            return result;
        }
        else
        {
            screenupdate = -1;
            update_screen();
            tc = tcbk;
            result.succeeded = false;
            return result;
        }
    }
    goto label_2035_internal;
}

menu_result menu_equipment()
{
    menu_result result = {false, false, turn_result_t::none};
    int cs_prev = 0;
    int mainhand = 0;
    cc = 0;
    page = 0;
    pagesize = 14;
    listmax = 0;
    cs = 0;
    cs_bk = -1;
    if (cs_prev != 0)
    {
        cs = cs_prev;
        cs_prev = 0;
    }
    curmenu = 1;
    mainhand = 0;
    for (int i = 0; i < 30; ++i)
    {
        if (cdata[cc].body_parts[i] != 0)
        {
            if (trait(206) != 0)
            {
                if (cdata[cc].body_parts[i] / 10000 == 2)
                {
                    continue;
                }
            }
            if (trait(203) != 0)
            {
                if (cdata[cc].body_parts[i] / 10000 == 9)
                {
                    continue;
                }
            }
            if (trait(205) != 0)
            {
                if (cdata[cc].body_parts[i] / 10000 == 3)
                {
                    continue;
                }
            }
            if (mainhand == 0)
            {
                if (cdata[cc].body_parts[i] / 10000 == 5)
                {
                    mainhand = i + 100;
                }
            }
            list(0, listmax) = i + 100;
            list(1, listmax) = cdata[cc].body_parts[i] / 10000;
            ++listmax;
        }
    }
    sort_list_by_column1();
    drawmenu();
    ww = 690;
    wh = 380;
    wx = (windoww - 690) / 2 + inf_screenx;
    wy = winposy(380);
    if (nowindowanime == 0)
    {
        snd(95);
    }
    window_animation(wx, wy, ww, wh, 9, 4);
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_wear.bmp", 1);
    gsel(0);
    windowshadow = 1;
label_2051_internal:
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
label_2052_internal:
    s(0) = i18n::s.get("core.locale.ui.equip.title");
    s(1) = strhint5 + strhint5b + strhint3;
    display_window(
        (windoww - 690) / 2 + inf_screenx, winposy(428), 690, 428, 64);
    display_topic(
        i18n::s.get("core.locale.ui.equip.category") + "/"
            + i18n::s.get("core.locale.ui.equip.name"),
        wx + 28,
        wy + 30);
    if (showresist == 0)
    {
        display_topic(
            i18n::s.get("core.locale.ui.equip.weight"), wx + 524, wy + 30);
    }
    pos(wx + 46, wy - 16);
    gcopy(3, 768, 48, 48, 48);
    pos(wx + ww - 106, wy);
    gcopy(3, 960, 96, 96, 120);
    pos(wx, wy + wh - 164);
    gcopy(3, 960, 216, 72, 144);
    if (showresist)
    {
        pos(wx + 320, wy + 40);
        mes(i18n::s.get("core.locale.ui.equip.resist"));
    }
    display_note(
        i18n::s.get("core.locale.ui.equip.equip_weight") + ": "
        + cnvweight(cdata[cc].sum_of_equipment_weight) + cnveqweight(cc) + " "
        + i18n::s.get("core.locale.ui.equip.hit_bonus") + ":"
        + cdata[cc].hit_bonus + " "
        + i18n::s.get("core.locale.ui.equip.damage_bonus") + ":"
        + cdata[cc].damage_bonus + u8"  DV/PV:"s + cdata[cc].dv + u8"/"s
        + cdata[cc].pv);
    font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
    gmode(2);
    keyrange = 0;
    f = 0;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_select(cnt);
        ++keyrange;
        if (cnt % 2 == 0)
        {
            boxf(wx + 100, wy + 60 + cnt * 19, 558, 18, {12, 14, 16, 16});
        }
        q = i18n::_(u8"ui", u8"body_part", u8"_"s + list(1, p));
        if (list(0, p) == mainhand)
        {
            q = i18n::s.get("core.locale.ui.equip.main_hand");
        }
        pos(wx + 22, wy + 60 + cnt * 19 - 4);
        gcopy(3, 600 + (list(1, p) - 1) * 24, 336, 24, 24);
        pos(wx + 46, wy + 60 + cnt * 19 + 3);
        mes(q);
    }
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        display_key(wx + 88, wy + 60 + cnt * 19 - 2, cnt);
        p(1) = cdata[cc].body_parts[list(0, p) - 100];
        s(0) = u8"-    "s;
        s(1) = u8"-"s;
        if (p(1) % 10000 != 0)
        {
            p(1) = p(1) % 10000 - 1;
            s(0) = itemname(p(1));
            s(1) = cnvweight(inv[p(1)].weight);

            draw_item_with_portrait(inv[p(1)], wx + 126, wy + 70 + cnt * 19);

            if (showresist)
            {
                equipinfo(p(1), wx + 320, wy + 60 + cnt * 19 + 2);
                s = strmid(s, 0, 22);
            }
        }
        else
        {
            p(1) = -1;
        }
        cs_list(
            cs == cnt,
            s,
            wx + 140 - 26,
            wy + 60 + cnt * 19 - 1,
            30,
            p(1) != -1,
            p(1));
        pos(wx + 640 - strlen_u(s(1)) * 7, wy + 60 + cnt * 19 + 2);
        mes(s(1));
    }
    cs_bk = cs;
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (p != -1)
    {
        cs_prev = cs;
        body = p;
        if (cdata[cc].body_parts[body - 100] % 10000 != 0)
        {
            gdata(808) = 1;
            ci = cdata[cc].body_parts[body - 100] % 10000 - 1;
            if (is_cursed(inv[ci].curse_state))
            {
                txt(i18n::s.get(
                    "core.locale.ui.equip.cannot_be_taken_off", inv[ci]));
                goto label_2051_internal;
            }
            unequip_item(cc);
            chara_refresh(cc);
            snd(13);
            txtnew();
            txt(i18n::s.get("core.locale.ui.equip.you_unequip", inv[ci]));
            if (cdata[cc].body_parts[body - 100] / 10000 == 5)
            {
                equip_melee_weapon();
            }
            render_hud();
            goto label_2051_internal;
        }
        nowindowanime = 1;
        menucycle = 0;
        invctrl = 6;
        snd(100);
        result.turn_result = turn_result_t::ctrl_inventory;
        return result;
    }
    if (key == key_identify)
    {
        p = list(0, pagesize * page + cs);
        if (cdata[cc].body_parts[p - 100] % 10000 != 0)
        {
            ci = cdata[cc].body_parts[p - 100] % 10000 - 1;
            cs_prev = cs;
            item_show_description();
            nowindowanime = 1;
            returnfromidentify = 0;
            screenupdate = -1;
            update_screen();
            result.turn_result = turn_result_t::menu_equipment;
            return result;
        }
    }
    if (key == key_mode)
    {
        if (showresist == 1)
        {
            showresist = 0;
        }
        else
        {
            showresist = 1;
        }
        snd(1);
        goto label_2051_internal;
    }
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 3)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 3;
                }
            }
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    csctrl = 0;
                    result.turn_result = turn_result_t::menu_character_sheet;
                    return result;
                }
                if (curmenu == 1)
                {
                    result.turn_result = turn_result_t::menu_equipment;
                    return result;
                }
                if (curmenu == 2)
                {
                    result.turn_result = turn_result_t::menu_feats;
                    return result;
                }
                if (curmenu == 3)
                {
                    result.turn_result = turn_result_t::menu_materials;
                    return result;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_2051_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_2051_internal;
        }
    }
    if (key == key_cancel)
    {
        menucycle = 0;
        create_pcpic(cc, true);
        update_screen();
        result.turn_result = turn_result_t::pc_turn_user_error;
        return result;
    }
    goto label_2052_internal;
}

menu_result menu_materials()
{
    menu_result result = {false, false, turn_result_t::none};
    listmax = 0;
    page = 0;
    pagesize = 15;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    curmenu = 3;
    for (int cnt = 0; cnt < 400; ++cnt)
    {
        if (mat(cnt) != 0)
        {
            list(0, listmax) = cnt;
            ++listmax;
        }
    }
    gsel(7);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"ie_scroll.bmp");
    gsel(0);
    snd(92);
    drawmenu();
    wx = (windoww - 600) / 2 + inf_screenx;
    wy = winposy(430);
    ww = 600;
    wh = 430;
    window_animation(wx, wy, ww, wh, 9, 4);
    windowshadow = 1;
label_1860_internal:
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
    drawmenu();
label_1861_internal:
    s = strhint2 + strhint3b;
    showscroll(s, wx, wy, ww, wh);
    display_topic(
        i18n::s.get("core.locale.ui.material.name"), wx + 38, wy + 36);
    display_topic(
        i18n::s.get("core.locale.ui.material.detail"), wx + 296, wy + 36);
    keyrange = 0;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_select(cnt);
        ++keyrange;
        if (cnt % 2 == 0)
        {
            boxf(wx + 70, wy + 66 + cnt * 19, 490, 18, {12, 14, 16, 16});
        }
        display_key(wx + 68, wy + 66 + cnt * 19 - 2, cnt);
    }
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        i = list(0, p);
        s = ""s + matname(i) + " " + i18n::s.get("core.locale.crafting.menu.x")
            + " " + mat(i);
        cs_list(cs == cnt, s, wx + 96, wy + 66 + cnt * 19 - 1, 0, 0);
        s = matdesc(i);
        pos(wx + 308, wy + 66 + cnt * 19 + 2);
        mes(s);

        draw_item_material(matref(2, i), wx + 47, wy + 69 + cnt * 19 + 2);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 3)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 3;
                }
            }
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    csctrl = 0;
                    result.turn_result = turn_result_t::menu_character_sheet;
                    return result;
                }
                if (curmenu == 1)
                {
                    result.turn_result = turn_result_t::menu_equipment;
                    return result;
                }
                if (curmenu == 2)
                {
                    result.turn_result = turn_result_t::menu_feats;
                    return result;
                }
                if (curmenu == 3)
                {
                    result.turn_result = turn_result_t::menu_materials;
                    return result;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_1860_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_1860_internal;
        }
    }
    if (key == key_cancel)
    {
        update_screen();
        result.turn_result = turn_result_t::pc_turn_user_error;
        return result;
    }
    goto label_1861_internal;
}

void set_pcc_info(int val0)
{
    rtval = -2;
    if (page == 0)
    {
        if (val0 == 0)
        {
            rtval = -2;
        }
        if (val0 == 1)
        {
            rtval(0) = 100;
            rtval(1) = 0;
            rtval(2) = cdata[cc].portrait;
        }
        if (val0 == 2)
        {
            rtval(0) = 1;
            rtval(1) = 0;
            rtval(2) = pcc(1, cc) % 1000;
            rtvaln = u8"hair"s;
        }
        if (val0 == 3)
        {
            rtval(0) = 10;
            rtval(1) = 0;
            rtval(2) = pcc(10, cc) % 1000;
            rtvaln = u8"subhair"s;
        }
        if (val0 == 4)
        {
            rtval(0) = 1;
            rtval(1) = 1;
            rtval(2) = pcc(1, cc) / 1000;
        }
        if (val0 == 5)
        {
            rtval(0) = 15;
            rtval(1) = 0;
            rtval(2) = pcc(15, cc) % 1000;
            rtvaln = u8"body"s;
        }
        if (val0 == 6)
        {
            rtval(0) = 9;
            rtval(1) = 0;
            rtval(2) = pcc(9, cc) % 1000;
            rtvaln = u8"cloth"s;
        }
        if (val0 == 7)
        {
            rtval(0) = 7;
            rtval(1) = 0;
            rtval(2) = pcc(7, cc) % 1000;
            rtvaln = u8"pants"s;
        }
        if (val0 == 8)
        {
            rtval = -1;
            rtvaln = "";
        }
        if (val0 == 9)
        {
            if (cc != 0)
            {
                rtval(0) = 101;
                rtval(1) = 0;
                rtval(2) = cdata[cc].has_own_sprite();
            }
            else
            {
                rtval(0) = 16;
                rtval(1) = 0;
                rtval(2) = pcc(16, cc) % 1000;
                rtvaln = u8"ride"s;
            }
        }
    }
    else
    {
        if (val0 == 0)
        {
            rtval(0) = 15;
            rtval(1) = 1;
            rtval(2) = pcc(15, cc) / 1000;
        }
        if (val0 == 1)
        {
            rtval(0) = 9;
            rtval(1) = 1;
            rtval(2) = pcc(9, cc) / 1000;
        }
        if (val0 == 2)
        {
            rtval(0) = 7;
            rtval(1) = 1;
            rtval(2) = pcc(7, cc) / 1000;
        }
        if (val0 == 3)
        {
            rtval(0) = 11;
            rtval(1) = 0;
            rtval(2) = pcc(11, cc) % 1000;
            rtvaln = u8"etc"s;
        }
        if (val0 == 4)
        {
            rtval(0) = 12;
            rtval(1) = 0;
            rtval(2) = pcc(12, cc) % 1000;
            rtvaln = u8"etc"s;
        }
        if (val0 == 5)
        {
            rtval(0) = 13;
            rtval(1) = 0;
            rtval(2) = pcc(13, cc) % 1000;
            rtvaln = u8"etc"s;
        }
        if (val0 == 6)
        {
            rtval(0) = 14;
            rtval(1) = 0;
            rtval(2) = pcc(14, cc) % 1000;
            rtvaln = u8"eye"s;
        }
        if (val0 == 7)
        {
            rtval = -1;
            rtvaln = "";
        }
    }
    return;
}

int change_appearance()
{
    create_pcpic(cc, false);
    page = 0;
    pagesize = 19;
    cs = 0;
    cs_bk = -1;
    ww = 380;
    wh = 340;
    wx = (windoww - ww) / 2 + inf_screenx;
    wy = winposy(wh);
    snd(97);
    window_animation(wx, wy, ww, wh, 9, 7);
    gsel(4);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"face1.bmp", 1);
    buffer(7, 800, 112);
    boxf();
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        const auto filepath = filesystem::dir::user() / u8"graphic"
            / (u8"face"s + (cnt + 1) + u8".bmp");
        if (fs::exists(filepath))
        {
            pos(cnt * 80, 0);
            picload(filepath, 1);
        }
    }
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_mirror.bmp", 1);
    gsel(0);
    windowshadow = 1;
label_2040_internal:
    listmax = 0;
    if (page == 0)
    {
        s(0) = i18n::s.get("core.locale.ui.appearance.basic.done");
        s(1) = i18n::s.get("core.locale.ui.appearance.basic.portrait");
        s(2) = i18n::s.get("core.locale.ui.appearance.basic.hair");
        s(3) = i18n::s.get("core.locale.ui.appearance.basic.sub_hair");
        s(4) = i18n::s.get("core.locale.ui.appearance.basic.hair_color");
        s(5) = i18n::s.get("core.locale.ui.appearance.basic.body");
        s(6) = i18n::s.get("core.locale.ui.appearance.basic.cloth");
        s(7) = i18n::s.get("core.locale.ui.appearance.basic.pants");
        s(8) = i18n::s.get("core.locale.ui.appearance.basic.set_detail");
        if (cc != 0)
        {
            s(9) = i18n::s.get("core.locale.ui.appearance.basic.custom");
        }
        else
        {
            s(9) = i18n::s.get("core.locale.ui.appearance.basic.riding");
        }
        p = 9 + (cc != 0) + (cc == 0) * (gdata_mount != 0);
    }
    else
    {
        s(0) = i18n::s.get("core.locale.ui.appearance.detail.body_color");
        s(1) = i18n::s.get("core.locale.ui.appearance.detail.cloth_color");
        s(2) = i18n::s.get("core.locale.ui.appearance.detail.pants_color");
        s(3) = i18n::s.get("core.locale.ui.appearance.detail.etc_1");
        s(4) = i18n::s.get("core.locale.ui.appearance.detail.etc_2");
        s(5) = i18n::s.get("core.locale.ui.appearance.detail.etc_3");
        s(6) = i18n::s.get("core.locale.ui.appearance.detail.eyes");
        s(7) = i18n::s.get("core.locale.ui.appearance.detail.set_basic");
        p = 8;
    }
    for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
    {
        list(0, cnt) = cnt;
        listn(0, cnt) = s(cnt);
        ++listmax;
    }
    keyrange = 0;
    for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
    {
        p = cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_enter;
        ++keyrange;
    }
label_2041_internal:
    pagesize = 0;
    s(0) = i18n::s.get("core.locale.ui.appearance.basic.title");
    s(1) = i18n::s.get("core.locale.ui.appearance.hint");
    display_window(
        (windoww - 380) / 2 + inf_screenx, winposy(340) - 12, 380, 340);
    pagesize = listmax;
    display_topic(
        i18n::s.get("core.locale.ui.appearance.basic.category"),
        wx + 34,
        wy + 36);
    pos(wx + ww - 40, wy);
    gcopy(3, 960, 96, 48, 120);
    ++i;
    if (i % 100 < 45)
    {
        f = i % 16;
    }
    else
    {
        ++f;
    }
    window2(wx + 234, wy + 71, 88, 120, 1, 1);
    if (cs == 1 && page == 0)
    {
        if (cdata[cc].portrait >= 0)
        {
            p = cdata[cc].sex * 64 + cdata[cc].portrait;
            pos(wx + 238, wy + 75);
            gcopy(4, p % 16 * 48, p / 16 * 72, 48, 72, 80, 112);
        }
        else if (cdata[cc].portrait != -1)
        {
            pos(wx + 238, wy + 75);
            gcopy(
                7,
                std::abs((cdata[cc].portrait + 2)) * 80,
                0,
                80,
                112,
                80,
                112);
        }
    }
    else if (cdata[cc].has_own_sprite() == 1)
    {
        pos(wx + 280, wy + 130);
        gmode(2);
        gcopy_c(20 + cc, f / 4 % 4 * 32, f / 16 % 4 * 48, 32, 48, 48, 80);
    }
    else
    {
        draw_chara(cdata[cc], wx + 280, wy + 130);
    }
    gmode(2);
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = cnt;
        if (p >= listmax)
        {
            break;
        }
        set_pcc_info(cnt);
        s = listn(0, p);
        if (rtval >= 0)
        {
            if (rtval(2) >= 0)
            {
                s += u8" "s + rtval(2);
            }
            else if (rtval(2) == -1)
            {
                s += u8" N/A"s;
            }
            else
            {
                s += u8" u"s + (std::abs(rtval(2)) - 1);
            }
        }
        cs_list(cs == cnt, s, wx + 60, wy + 66 + cnt * 21 - 1, 0);
        if (rtval != -2)
        {
            pos(wx + 30, wy + 66 + cnt * 21 - 5);
            gcopy(3, 312, 336, 24, 24);
            pos(wx + 175, wy + 66 + cnt * 21 - 5);
            gcopy(3, 336, 336, 24, 24);
        }
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    set_pcc_info(cs);
    p = 0;
    if (rtval == -2)
    {
        if (key == key_enter)
        {
            create_pcpic(cc, true);
            return 1;
        }
        if (key == key_pageup || key == key_pagedown)
        {
            key = ""s;
        }
    }
    else if (key == key_enter)
    {
        key = key_pageup;
    }
    if (rtval == -1)
    {
        if (key == key_pageup || key == key_pagedown)
        {
            snd(5);
            if (page == 0)
            {
                page = 1;
            }
            else
            {
                page = 0;
                cs = 8;
            }
            goto label_2040_internal;
        }
    }
    if (key == key_pageup)
    {
        snd(5);
        if (rtval == 100)
        {
            if (cdata[cc].portrait < 31)
            {
                ++cdata[cc].portrait;
            }
            goto label_2041_internal;
        }
        if (rtval == 101)
        {
            cdata[cc].has_own_sprite() = true;
            goto label_2041_internal;
        }
        if (rtval(1) == 0)
        {
            if (fs::exists(
                    filesystem::dir::graphic()
                    / (u8"pcc_"s + rtvaln + u8"_" + (pcc(rtval, cc) % 1000 + 1)
                       + u8".bmp")))
            {
                ++pcc(rtval, cc);
                p = 1;
            }
        }
        else if (pcc(rtval, cc) / 1000 < 21)
        {
            pcc(rtval, cc) += 1000;
            p = 1;
        }
    }
    if (key == key_pagedown)
    {
        snd(5);
        if (rtval == 100)
        {
            if (cdata[cc].portrait > -10)
            {
                --cdata[cc].portrait;
            }
            goto label_2041_internal;
        }
        if (rtval == 101)
        {
            cdata[cc].has_own_sprite() = false;
            goto label_2041_internal;
        }
        if (rtval(1) == 0)
        {
            if ((pcc(rtval, cc) % 1000 == 1 && rtval != 15)
                || fs::exists(
                       filesystem::dir::graphic()
                       / (u8"pcc_"s + rtvaln + u8"_"s
                          + (pcc(rtval, cc) % 1000 - 1) + u8".bmp"s)))
            {
                --pcc(rtval, cc);
                p = 1;
            }
        }
        else if (pcc(rtval, cc) / 1000 > 0)
        {
            pcc(rtval, cc) -= 1000;
            p = 1;
        }
    }
    create_pcpic(cc, false);
    if (key == key_cancel)
    {
        create_pcpic(cc, true);
        return 0;
    }
    if (mode == 1)
    {
        if (getkey(snail::key::f1))
        {
            return -1;
        }
    }
    goto label_2041_internal;
}

int change_appearance_equipment()
{
    create_pcpic(cc, true);
    snd(26);
    page = 0;
    pagesize = 18;
    cs = 0;
    cs_bk = -1;
    windowshadow = 1;
    listmax = 0;
    s(0) = i18n::s.get("core.locale.ui.appearance.equipment.done");
    s(1) = i18n::s.get("core.locale.ui.appearance.equipment.chest");
    s(2) = i18n::s.get("core.locale.ui.appearance.equipment.leg");
    s(3) = i18n::s.get("core.locale.ui.appearance.equipment.belt");
    s(4) = i18n::s.get("core.locale.ui.appearance.equipment.glove");
    s(5) = i18n::s.get("core.locale.ui.appearance.equipment.mantle");
    p = 6;
    for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
    {
        list(0, cnt) = cnt;
        listn(0, cnt) = s(cnt);
        ++listmax;
    }
    keyrange = 0;
    for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
    {
        p = cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_enter;
        ++keyrange;
    }

    while (1)
    {
        pagesize = 0;
        s(0) = i18n::s.get("core.locale.ui.appearance.equipment.title");
        s(1) = i18n::s.get("core.locale.ui.appearance.hint");
        display_window(
            (windoww - 360) / 2 + inf_screenx, winposy(289) - 12, 360, 289);
        s = i18n::s.get("core.locale.ui.appearance.equipment.part");
        pagesize = listmax;
        display_topic(s, wx + 34, wy + 36);
        ++i;
        if (i % 100 < 45)
        {
            f = i % 16;
        }
        else
        {
            ++f;
        }
        window2(wx + 234, wy + 60, 88, 120, 1, 1);
        pos(wx + 280, wy + 120);
        gmode(2);
        gcopy_c(20 + cc, f / 4 % 4 * 32, f / 16 % 4 * 48, 32, 48, 48, 80);
        gmode(2);
        font(14 - en * 2);
        cs_listbk();
        for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
        {
            p = cnt;
            if (p >= listmax)
            {
                break;
            }
            s = listn(0, p);
            if (cnt != 0)
            {
                if (pcc(20 + cnt - 1, cc) == 0)
                {
                    s += u8"On"s;
                }
                else
                {
                    s += u8"Off"s;
                }
                pos(wx + 30, wy + 66 + cnt * 21 - 5);
                gcopy(3, 312, 336, 24, 24);
                pos(wx + 175, wy + 66 + cnt * 21 - 5);
                gcopy(3, 336, 336, 24, 24);
            }
            cs_list(cs == cnt, s, wx + 60, wy + 66 + cnt * 21 - 1, 0);
        }
        if (keyrange != 0)
        {
            cs_bk = cs;
        }
        redraw();
        await(config::instance().wait1);
        key_check();
        cursor_check();
        if (cs != 0)
        {
            if (key == key_enter)
            {
                key = key_pageup;
            }
            if (key == key_pageup || key == key_pagedown)
            {
                if (pcc(20 + cs - 1, cc) == 0)
                {
                    pcc(20 + cs - 1, cc) = 1;
                }
                else
                {
                    pcc(20 + cs - 1, cc) = 0;
                }
                create_pcpic(cc, true);
                snd(5);
            }
        }
        if ((cs == 0 && key == key_enter) || key == key_cancel)
        {
            snd(20);
            create_pcpic(cc, true);
            return 1;
        }
    }
}

void append_accuracy_info(int val0)
{
    p(1) = 0;
    p(2) = 0;
    attackskill = 106;
    ammo = -1;
    attacknum = 0;
    for (int cnt = 0; cnt < 30; ++cnt)
    {
        body = 100 + cnt;
        if (cdata[cc].body_parts[cnt] % 10000 == 0)
        {
            continue;
        }
        if (cdata[cc].body_parts[cnt] / 10000 == 10)
        {
            continue;
        }
        if (cdata[cc].body_parts[cnt] / 10000 == 11)
        {
            continue;
        }
        cw = cdata[cc].body_parts[cnt] % 10000 - 1;
        if (inv[cw].dice_x > 0)
        {
            attackskill = inv[cw].skill;
            ++p(1);
            s(1) =
                i18n::s.get("core.locale.ui.chara_sheet.damage.melee") + p(1);
            ++attacknum;
            show_weapon_dice(val0);
        }
    }
    if (attackskill == 106)
    {
        s(1) = i18n::s.get("core.locale.ui.chara_sheet.damage.unarmed");
        show_weapon_dice(val0);
    }
    attacknum = 0;
    int stat = can_do_ranged_attack();
    if (stat == 1)
    {
        s(1) = i18n::s.get("core.locale.ui.chara_sheet.damage.dist");
        show_weapon_dice(val0);
    }
    return;
}

void show_weapon_dice(int val0)
{
    tc = cc;
    font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
    color(20, 10, 0);
    if (val0 == 0)
    {
        pos(wx + 590, wy + 281 + p(2) * 16);
        mes(i18n::s.get("core.locale.ui.chara_sheet.damage.hit"));
        pos(wx + 417, wy + 281 + p(2) * 16);
        mes(s(1));
    }
    color(0, 0, 0);
    attackrange = 0;
    if (the_item_db[inv[cw].id]->category == 24000) // TODO coupling
    {
        attackrange = 1;
    }
    attackvar = 0;
    int tohit = calc_accuracy(false);
    dmg = calcattackdmg(1);
    font(14 - en * 2);
    s(2) = ""s + dmgmulti;
    s = ""s + tohit + u8"%"s;
    if (val0 == 0)
    {
        pos(wx + 625 - en * 8, wy + 279 + p(2) * 16);
        mes(s);
    }
    else
    {
        s(3) = s;
    }
    s = ""s + dice1 + u8"d"s + dice2 + cnvfix(dmgfix) + u8" x"s
        + strmid(
              s(2),
              0,
              3 + (elona::stoi(s(2)) >= 10) + (elona::stoi(s(2)) >= 100));
    if (val0 == 0)
    {
        pos(wx + 460 + en * 8, wy + 279 + p(2) * 16);
        mes(s);
    }
    else
    {
        noteadd(s(1) + "   : " + fixtxt(s(3), 12) + " " + fixtxt(s, 20));
    }
    ++p(2);
    return;
}

menu_result menu_feats()
{
    deco_traits_menu();
    return menu_feats_internal();
}

menu_result menu_feats_internal_b()
{
    return menu_feats_internal();
}

void deco_traits_menu()
{
    listmax = 0;
    page = 0;
    pagesize = 15;
    cs = 0;
    tc = 0;
    cs_bk = -1;
    curmenu = 2;
    snd(96);
    drawmenu();
    ww = 700;
    wh = 400;
    wx = (windoww - ww) / 2 + inf_screenx;
    wy = winposy(wh);
    window_animation(wx, wy, ww, wh, 9, 4);
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_feat.bmp", 1);
    gsel(0);
    windowshadow = 1;
}

menu_result menu_feats_internal()
{
    menu_result result = {false, false, turn_result_t::none};
    int featrq = 0;
label_196901_internal:
    listmax = 0;
    if (tc == 0 && gdata_acquirable_feat_count > 0)
    {
        list(0, listmax) = -1;
        list(1, listmax) = 0;
        ++listmax;
    }
    f = 0;
    for (int cnt = 0; cnt < 217; ++cnt)
    {
        if (tc != 0)
        {
            break;
        }
        int stat = trait_get_info(0, cnt);
        if (stat == 0)
        {
            continue;
        }
        if (stat == 1)
        {
            if (traitref == 0)
            {
                if (gdata_acquirable_feat_count > 0)
                {
                    list(0, listmax) = cnt;
                    list(1, listmax) = cnt + 1;
                    ++listmax;
                }
            }
        }
        if (trait(cnt) != 0)
        {
            list(0, listmax) = cnt;
            list(1, listmax) = 10000 + cnt + 1;
            ++listmax;
            ++f;
        }
    }
    if (f != 0)
    {
        list(0, listmax) = -2;
        list(1, listmax) = 10000;
        ++listmax;
    }
    sort_list_by_column1();
    for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
    {
        i = list(0, cnt);
        if (i < 0)
        {
            if (i == -1)
            {
                s = i18n::s.get("core.locale.trait.window.available_feats");
            }
            if (i == -2)
            {
                s = i18n::s.get("core.locale.trait.window.feats_and_traits");
            }
            listn(0, cnt) = s;
            continue;
        }
        int tid = i;
        int stat = trait_get_info(0, tid);
        featrq = stat;
        s = "";
        if (list(1, cnt) < 10000)
        {
            if (trait(tid) < traitref(2))
            {
                s = traitrefn2(trait(tid));
            }
            else
            {
                s = traitrefn2(traitref(2) - 1) + u8"(MAX)"s;
            }
            if (featrq == -1)
            {
                s += u8"("s
                    + i18n::s.get("core.locale.trait.window.requirement")
                    + u8")"s;
            }
            pos(wx + 30, wy + 61 + cnt * 19);
            x = 84;
        }
        else
        {
            pos(wx + 45, wy + 61 + cnt * 19);
            x = 70;
            if (traitref == 0)
            {
                s = u8"["s
                    + i18n::s.get("core.locale.trait.window.category.feat")
                    + u8"]"s;
            }
            if (traitref == 1)
            {
                s = u8"["s
                    + i18n::s.get("core.locale.trait.window.category.mutation")
                    + u8"]"s;
            }
            if (traitref == 2)
            {
                s = u8"["s
                    + i18n::s.get("core.locale.trait.window.category.race")
                    + u8"]"s;
            }
            if (traitref == 3)
            {
                s = u8"["s
                    + i18n::s.get(
                          "core.locale.trait.window.category.ether_disease")
                    + u8"]"s;
            }
            s += traitrefn(2 + std::abs(trait(tid)));
        }
        listn(0, cnt) = s;
    }
    if (cdata[tc].is_incognito() == 1)
    {
        list(0, listmax) = 1;
        list(1, listmax) = 99999;
        listn(0, listmax) = u8"["s
            + i18n::s.get("core.locale.trait.window.category.etc") + u8"]"s
            + i18n::s.get("core.locale.trait.incognito");
        ++listmax;
    }
    if (cdata[tc].is_pregnant() == 1)
    {
        list(0, listmax) = 1;
        list(1, listmax) = 99999;
        listn(0, listmax) = u8"["s
            + i18n::s.get("core.locale.trait.window.category.etc") + u8"]"s
            + i18n::s.get("core.locale.trait.pregnant");
        ++listmax;
    }
    if (cdata[tc].has_anorexia() == 1)
    {
        list(0, listmax) = 1;
        list(1, listmax) = 99999;
        listn(0, listmax) = u8"["s
            + i18n::s.get("core.locale.trait.window.category.etc") + u8"]"s
            + i18n::s.get("core.locale.trait.anorexia");
        ++listmax;
    }
    if (cdata[tc].speed_correction_value != 0)
    {
        list(0, listmax) = 1;
        list(1, listmax) = 99999;
        listn(0, listmax) = u8"["s
            + i18n::s.get("core.locale.trait.window.category.etc") + u8"]"s
            + i18n::s.get(
                  "core.locale.trait.body_is_complicated",
                  cdata[tc].speed_correction_value);
        ++listmax;
    }
    if (tc == 0 && gdata_ether_disease_speed != 0)
    {
        if (gdata_ether_disease_speed > 0)
        {
            list(0, listmax) = 1;
            list(1, listmax) = 99999;
            listn(0, listmax) = u8"["s
                + i18n::s.get("core.locale.trait.window.category.etc") + u8"]"s
                + i18n::s.get("core.locale.trait.ether_disease_grows.fast");
            ++listmax;
        }
        else
        {
            list(0, listmax) = 1;
            list(1, listmax) = 99999;
            listn(0, listmax) = u8"["s
                + i18n::s.get("core.locale.trait.window.category.etc") + u8"]"s
                + i18n::s.get("core.locale.trait.ether_disease_grows.slow");
            ++listmax;
        }
    }
    if (dump_return == 1)
    {
        dump_return = 0;
        result.succeeded = false;
        return result;
    }
    std::vector<std::string> traits_by_enchantments;
    for (int i = 0; i < 30; ++i)
    {
        if (cdata[tc].body_parts[i] % 10000 != 0)
        {
            ci = cdata[tc].body_parts[i] % 10000 - 1;
            for (const auto& enc : inv[ci].enchantments)
            {
                if (enc.id == 0)
                    break;
                get_enchantment_description(enc.id, enc.power, 0, true);
                if (!s(0).empty())
                {
                    traits_by_enchantments.push_back(s);
                }
            }
        }
    }
    std::sort(
        std::begin(traits_by_enchantments), std::end(traits_by_enchantments));
    traits_by_enchantments.erase(
        std::unique(
            std::begin(traits_by_enchantments),
            std::end(traits_by_enchantments)),
        std::end(traits_by_enchantments));
    for (const auto& trait : traits_by_enchantments)
    {
        list(0, listmax) = 1;
        list(1, listmax) = 99999;
        listn(0, listmax) = i18n::s.get(
            "core.locale.trait.window.his_equipment", cnven(his(tc, 1)), trait);
        ++listmax;
    }
    if (tc != 0)
    {
        for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
        {
            if (jp)
            {
                listn(0, cnt) =
                    strutil::replace(listn(0, cnt), u8"あなた", he(tc, 1));
            }
            if (en)
            {
                listn(0, cnt) =
                    strutil::replace(listn(0, cnt), u8" your", his(tc, 1));
                listn(0, cnt) =
                    strutil::replace(listn(0, cnt), u8" you", him(tc, 1));
            }
        }
    }
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
    if (cs < 0)
    {
        for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
        {
            if (list(1, cnt) >= 10000)
            {
                if (list(0, cnt) - 10000 == cs)
                {
                    page = cnt / pagesize;
                    cs = cnt % pagesize;
                    break;
                }
            }
        }
        if (cs < 0)
        {
            cs = 0;
        }
    }
label_1970_internal:
    s(0) = i18n::s.get("core.locale.trait.window.title");
    s(1) = i18n::s.get("core.locale.trait.window.enter") + "  " + strhint2
        + strhint3 + u8"z,x ["s + i18n::s.get("core.locale.trait.window.ally")
        + u8"]"s;
    if (mode == 1)
    {
        i = 1;
    }
    else
    {
        i = 0;
    }
    display_window(
        (windoww - 730) / 2 + inf_screenx,
        winposy(430, i) + i * 15,
        730,
        430,
        55,
        40);
    s(0) = i18n::s.get("core.locale.trait.window.name");
    s(1) = i18n::s.get("core.locale.trait.window.level");
    s(2) = i18n::s.get("core.locale.trait.window.detail");
    display_topic(s, wx + 46, wy + 36);
    display_topic(s(2), wx + 255, wy + 36);
    pos(wx + 46, wy - 16);
    gcopy(3, 816, 48, 48, 48);
    pos(wx + ww - 56, wy + wh - 198);
    gcopy(3, 960, 96, 48, 192);
    pos(wx, wy);
    gcopy(3, 1008, 96, 48, 144);
    pos(wx + ww - 108, wy);
    gcopy(3, 960, 288, 96, 72);
    pos(wx, wy + wh - 70);
    gcopy(3, 1008, 240, 96, 48);
    keyrange = 0;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_select(cnt);
        ++keyrange;
        if (list(0, p) < 0)
        {
            continue;
        }
        if (cnt % 2 == 0)
        {
            boxf(wx + 57, wy + 66 + cnt * 19, 640, 18, {12, 14, 16, 16});
        }
        if (list(1, p) >= 10000 || list(0, p) < 0)
        {
            continue;
        }
        display_key(wx + 58, wy + 66 + cnt * 19 - 2, cnt);
    }
    if (tc == 0)
    {
        s = i18n::s.get(
            "core.locale.trait.window.you_can_acquire",
            gdata_acquirable_feat_count);
    }
    else
    {
        s = i18n::s.get(
            "core.locale.trait.window.your_trait", cnven(cdatan(0, tc)));
    }
    display_note(s, 50);
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        i = list(0, p);
        if (i < 0)
        {
            cs_list(cs == cnt, listn(0, p), wx + 114, wy + 66 + cnt * 19 - 1);
            continue;
        }
        int text_color{};
        if (list(1, p) != 99999)
        {
            int stat = trait_get_info(0, i);
            featrq = stat;
            if (trait(i) == 0)
            {
            }
            else if (trait(i) > 0)
            {
                text_color = 1;
            }
            else
            {
                text_color = 2;
            }
        }
        else
        {
            traitref = 5;
        }
        if (list(1, p) < 10000)
        {
            pos(wx + 30, wy + 61 + cnt * 19);
            x = 84;
        }
        else
        {
            pos(wx + 45, wy + 61 + cnt * 19);
            x = 70;
        }
        gcopy(3, 384 + traitref * 24, 336, 24, 24);
        switch (text_color)
        {
        case 0: color(10, 10, 10); break;
        case 1: color(0, 0, 200); break;
        case 2: color(200, 0, 0); break;
        }
        cs_list(cs == cnt, listn(0, p), wx + x, wy + 66 + cnt * 19 - 1, 0, -1);
        color(0, 0, 0);
        if (list(1, p) < 10000)
        {
            pos(wx + 270, wy + 66 + cnt * 19 + 2);
            switch (text_color)
            {
            case 0: color(10, 10, 10); break;
            case 1: color(0, 0, 200); break;
            case 2: color(200, 0, 0); break;
            }
            mes(traitrefn(2));
            color(0, 0, 0);
        }
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_INDEX(p);
    if (tc == 0)
    {
        if (gdata_acquirable_feat_count > 0)
        {
            if (p > 0)
            {
                if (list(1, p) < 10000)
                {
                    int tid = list(0, p);
                    trait_get_info(0, tid);
                    if (traitref(2) <= trait(tid))
                    {
                        if (mode != 1)
                        {
                            txt(i18n::s.get(
                                "core.locale.trait.window.already_maxed"));
                        }
                        goto label_196901_internal;
                    }
                    --gdata_acquirable_feat_count;
                    cs = -10000 + tid;
                    snd(61);
                    ++trait(tid);
                    chara_refresh(tc);
                    if (mode == 1)
                    {
                        if (gdata_acquirable_feat_count == 0)
                        {
                            result.succeeded = true;
                            return result;
                        }
                    }
                    else
                    {
                        render_hud();
                    }
                    goto label_196901_internal;
                }
            }
        }
    }
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 3)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 3;
                }
            }
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    csctrl = 0;
                    result.turn_result = turn_result_t::menu_character_sheet;
                    return result;
                }
                if (curmenu == 1)
                {
                    result.turn_result = turn_result_t::menu_equipment;
                    return result;
                }
                if (curmenu == 2)
                {
                    result.turn_result = turn_result_t::menu_feats;
                    return result;
                }
                if (curmenu == 3)
                {
                    result.turn_result = turn_result_t::menu_materials;
                    return result;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_196901_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_196901_internal;
        }
    }
    if (key == u8"z"s || key == u8"x"s)
    {
        p = tc;
        for (int cnt = 0; cnt < 16; ++cnt)
        {
            if (key == u8"z"s)
            {
                --p;
                if (p < 0)
                {
                    p = 15;
                }
            }
            if (key == u8"x"s)
            {
                ++p;
                if (p == 16)
                {
                    p = 0;
                }
            }
            if (cdata[p].state() != character::state_t::alive)
            {
                continue;
            }
            break;
        }
        tc = p;
        snd(1);
        page = 0;
        cs = 0;
        goto label_196901_internal;
    }
    if (key == key_cancel)
    {
        if (mode == 1)
        {
            result.succeeded = false;
            return result;
        }
        update_screen();
        result.turn_result = turn_result_t::pc_turn_user_error;
        return result;
    }
    if (mode == 1)
    {
        if (getkey(snail::key::f1))
        {
            show_game_help();
            result.pressed_f1 = true;
            return result;
        }
    }
    goto label_1970_internal;
}

menu_result menu_journal()
{

    menu_result result = {false, false, turn_result_t::none};
    curmenu = 1;
    page = 99;
    pagesize = 40;
    cs = 0;
    cc = 0;
    keyrange = 0;
    key_list(0) = key_enter;
    buff = newsbuff;
    if (newsbuff == ""s)
    {
        buff = u8"No news"s;
    }
    notesel(buff);
    noteadd(u8" - News - "s, 0);
    noteadd(""s, 1);
    if (noteinfo() / (pagesize / 2) % 2 == 1)
    {
        for (int cnt = 0, cnt_end = (pagesize / 2); cnt < cnt_end; ++cnt)
        {
            noteadd(""s, 2);
        }
    }
    page = noteinfo() / pagesize;
    for (int cnt = 0,
             cnt_end = cnt + (pagesize / 2 - noteinfo() % (pagesize / 2));
         cnt < cnt_end;
         ++cnt)
    {
        noteadd(""s);
    }
    noteadd(u8" - Quest - "s);
    noteadd(""s);
    quest_update_main_quest_journal();
    for (int cnt = 0, cnt_end = (gdata_number_of_existing_quests);
         cnt < cnt_end;
         ++cnt)
    {
        if (qdata(8, cnt) == 0)
        {
            continue;
        }
        rq = cnt;
        quest_set_data(2);
    }
    append_subquest_journal(0);
    for (int cnt = 0,
             cnt_end = cnt + (pagesize / 2 - noteinfo() % (pagesize / 2));
         cnt < cnt_end;
         ++cnt)
    {
        noteadd(""s);
    }
    noteadd(u8" - Quest Item - "s);
    noteadd(""s);
    append_quest_item_journal();
    for (int cnt = 0,
             cnt_end = cnt + (pagesize / 2 - noteinfo() % (pagesize / 2));
         cnt < cnt_end;
         ++cnt)
    {
        noteadd(""s);
    }
    gold = 0;
    p = clamp(cdata.player().fame / 10, 100, 25000);
    if (cdata.player().fame >= 25000)
    {
        p += (cdata.player().fame - 25000) / 100;
    }
    gold += p;
    noteadd(u8" - Title & Ranking - "s);
    noteadd(""s);
    noteadd(
        i18n::s.get("core.locale.ui.journal.rank.fame") + ": "
        + cdata.player().fame);
    noteadd(""s);
    for (int cnt = 0; cnt < 9; ++cnt)
    {
        if (gdata(120 + cnt) < 10000)
        {
            noteadd(
                ""s + ranktitle(cnt) + u8" Rank."s + gdata((120 + cnt)) / 100);
            s = i18n::s.get("core.locale.ui.journal.rank.pay", calcincome(cnt));
            gold += calcincome(cnt);
            if (cnt != 3 && cnt != 4 && cnt != 5 && cnt != 8)
            {
                s += i18n::s.get(
                    "core.locale.ui.journal.rank.deadline", gdata(140 + cnt));
            }
            noteadd(s);
            noteadd(""s);
        }
    }
    noteadd(i18n::s.get(
        "core.locale.ui.journal.rank.arena",
        gdata(802),
        cnvrank(gdata_ex_arena_level)));
    noteadd(""s);
    for (int cnt = 0,
             cnt_end = cnt + (pagesize / 2 - noteinfo() % (pagesize / 2));
         cnt < cnt_end;
         ++cnt)
    {
        noteadd(""s);
    }
    noteadd(u8" - Income & Expense - "s);
    noteadd(""s);
    noteadd(i18n::s.get("core.locale.ui.journal.income.salary.title"));
    noteadd(i18n::s.get("core.locale.ui.journal.income.salary.sum", gold));
    noteadd(""s);
    noteadd(i18n::s.get("core.locale.ui.journal.income.bills.title"));
    noteadd(i18n::s.get(
        "core.locale.ui.journal.income.bills.labor", gdata_cost_to_hire));
    noteadd(i18n::s.get(
        "core.locale.ui.journal.income.bills.maintenance", calccostbuilding()));
    noteadd(
        i18n::s.get("core.locale.ui.journal.income.bills.tax", calccosttax()));
    noteadd(i18n::s.get(
        "core.locale.ui.journal.income.bills.sum",
        (gdata_cost_to_hire + calccostbuilding() + calccosttax())));
    noteadd(""s);
    noteadd(i18n::s.get(
        "core.locale.ui.journal.income.bills.unpaid", gdata_left_bill));
    for (int cnt = 0,
             cnt_end = cnt + (pagesize / 2 - noteinfo() % (pagesize / 2));
         cnt < cnt_end;
         ++cnt)
    {
        noteadd(""s);
    }
    noteadd(u8" - Completed Quests - "s);
    noteadd(""s);
    append_subquest_journal(1);
    listmax = noteinfo();
    show_title(strhint2 + strhint3);
    drawmenu(2);
    wx = (windoww - 736) / 2 + inf_screenx;
    wy = winposy(448);
    snd(59);
    window_animation(wx, wy, 736, 448, 9, 4);
label_1973_internal:
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
    gsel(4);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"book.bmp", 1);
    gsel(0);
    pos(wx, wy);
    gcopy(4, 0, 0, 736, 448);
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        x = wx + 80 + cnt / 20 * 306;
        y = wy + 45 + cnt % 20 * 16;
        noteget(s, p);
        if (strmid(s, 0, 1) == u8"@"s)
        {
            s(1) = strmid(s, 1, 2);
            s = strmid(s, 3, s(0).size() - 3);
            font(10 + en - en * 2, snail::font_t::style_t::bold);
            color(0, 0, 200);
            if (s(1) == u8"QL"s)
            {
                color(100, 100, 0);
            }
            if (s(1) == u8"QC"s)
            {
                color(0, 100, 100);
            }
            if (s(1) == u8"QM"s)
            {
                color(0, 100, 0);
            }
            if (s(1) == u8"RE"s)
            {
                color(100, 0, 0);
                font(12 + sizefix - en * 2);
            }
            if (s(1) == u8"BL"s)
            {
                color(0, 0, 100);
                font(12 + sizefix - en * 2);
            }
        }
        else
        {
            font(12 + sizefix - en * 2);
        }
        pos(x, y);
        mes(s);
        color(0, 0, 0);
        if (p % 20 == 0)
        {
            font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
            pos(x + 90, y + 330);
            mes(u8"- "s + (p / 20 + 1) + u8" -"s);
            if (p % 40 == 20)
            {
                if (page < pagemax)
                {
                    pos(x + 200, y + 330);
                    mes(u8"(more)"s);
                }
            }
        }
    }
label_1974_internal:
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 2)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 2;
                }
            }
            key = "";
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    result.turn_result = turn_result_t::show_message_log;
                    return result;
                }
                if (curmenu == 1)
                {
                    result.turn_result = turn_result_t::menu_journal;
                    return result;
                }
                if (curmenu == 2)
                {
                    result.turn_result = turn_result_t::show_chat_history;
                    return result;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(71);
            ++page;
            goto label_1973_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(71);
            --page;
            goto label_1973_internal;
        }
    }
    if (key == key_cancel || key == key_enter)
    {
        menucycle = 0;
        update_screen();
        result.turn_result = turn_result_t::pc_turn_user_error;
        return result;
    }
    goto label_1974_internal;
}

static turn_result_t _visit_quest_giver(int quest_index)
{
    // TODO move the below somewhere else to decouple quest_teleport
    tc = qdata(0, quest_index);
    rq = quest_index;
    client = tc;
    efid = 619;
    magic();
    tc = client;
    if (cdata.player().state() == character::state_t::alive)
    {
        quest_teleport = true;
        talk_to_npc();
    }
    if (chatteleport == 1)
    {
        chatteleport = 0;
        return turn_result_t::exit_map;
    }
    return turn_result_t::turn_end;
}

turn_result_t show_quest_board()
{
    if (config::instance().extrahelp)
    {
        if (gdata(204) == 0)
        {
            if (mode == 0)
            {
                if (cdata.player().continuous_action_turn == 0)
                {
                    gdata(204) = 1;
                    ghelp = 4;
                    show_ex_help();
                    screenupdate = -1;
                    update_screen();
                }
            }
        }
    }

    auto result = ui::ui_menu_quest_board().show();

    if (result.canceled)
    {
        return turn_result_t::turn_end;
    }

    int quest_index = *result.value;
    return _visit_quest_giver(quest_index);
}

int show_hire_menu(hire_operation operation)
{
    auto result = ui::ui_menu_hire(operation).show();

    if (result.canceled)
    {
        return -1;
    }
    else
    {
        return *result.value;
    }
}

int show_spell_writer_menu()
{
    ui::ui_menu_spell_writer().show();

    return -1;
}

void list_adventurers()
{
    ui::ui_menu_adventurers().show();
}

int select_alias(int val0)
{
    auto result = ui::ui_menu_alias(val0).show();

    if (result.canceled)
    {
        return 0;
    }

    if (val0 == 3)
    {
        return result.value->seed;
    }
    else
    {
        cmaka = result.value->alias;
        return 1;
    }
}

void show_city_chart()
{
label_22711:
    listmax = 0;
    page = 0;
    pagesize = 16;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    snd(99);
    curmenu = 0;
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_politics.bmp", 1);
    gsel(0);
    fillbg(3, 960, 96, 128, 128);
    render_hud();
    drawmenu(3);
    windowshadow = 1;
    city = adata(28, gdata_current_map);
    lv = 0;
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
label_2272_internal:
    s(0) = i18n::s.get("core.locale.ui.city_chart.title");
    s(1) = strhint3b;
    display_window((windoww - 580) / 2 + inf_screenx, winposy(400), 580, 400);
    keyrange = 0;
    int j0 = 0;
    int n = 0;
    cs_listbk();
    if (adata(28, gdata_current_map) == 0 || gdata_current_dungeon_level != 1)
    {
        font(14 - en * 2);
        pos(wx + 40, wy + 50);
        mes(i18n::s.get("core.locale.ui.city_chart.no_economy"));
    }
    else
    {
        display_topic(
            i18n::s.get(
                "core.locale.ui.city_chart.chart",
                mapname(adata(16, gdata_current_map))),
            wx + 40,
            wy + 34);
        for (int cnt = 0;; ++cnt)
        {
            if (pochart(j0, n, lv) == 0 || cnt == 0)
            {
                if (cnt != 0)
                {
                    ++n;
                }
                j0 = 0;
                i = 0;
                for (int cnt = 0; cnt < 10; ++cnt)
                {
                    if (pochart(cnt, n, lv) != 0)
                    {
                        ++i;
                    }
                }
                if (i == 0)
                {
                    break;
                }
                y = wy + 70 + n * 55;
            }
            x = wx + (ww - 70) / (i + 1) * (j0 + 1);
            pos(x - 26, y - 3);
            gcopy(3, 960, 288, 144, 24);
            p = pochart(j0, n, lv);
            key_list(cnt) = key_select(cnt);
            ++keyrange;
            display_key(x - 30, y + 21, cnt);
            font(12 + sizefix - en * 2);
            bmes(cnven(popostname(p)), x - 2, y + jp * 2);
            font(14 - en * 2);
            if (podata(0 + cnt, city) == 0)
            {
                s = i18n::s.get("core.locale.ui.city_chart.empty");
            }
            cs_list(cs == cnt, s, x - 2, y + 20);
            ++j0;
        }
        if (keyrange != 0)
        {
            cs_bk = cs;
        }
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (p != -1)
    {
        return;
    }
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 2)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 2;
                }
            }
            key = "";
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    goto label_22711;
                }
                if (curmenu == 1)
                {
                    show_economy_window();
                    return;
                }
                if (curmenu == 2)
                {
                    show_politics_window();
                    return;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_22711;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_22711;
        }
    }
    if (key == key_cancel)
    {
        update_screen();
        return;
    }
    goto label_2272_internal;
}

void showeconomy(
    int prm_1086,
    int prm_1087,
    const std::string& prm_1088,
    int prm_1089,
    int prm_1090)
{
    int p_at_m195 = 0;
    pos(prm_1086, prm_1087);
    mes(prm_1088);
    pos(prm_1086 + 130, prm_1087);
    mes(""s + prm_1089);
    p_at_m195 = prm_1089 - prm_1090;
    if (p_at_m195 >= 0)
    {
        color(0, 0, 150);
    }
    else
    {
        color(150, 0, 0);
    }
    pos(prm_1086 + 130 + ginfo(14) + 12, prm_1087);
    mes(u8"("s + p_at_m195 + u8")"s);
    color(0, 0, 0);
    return;
}

void show_economy_window()
{
    curmenu = 1;
    key_list(0) = key_enter;
    keyrange = 0;
    pagesize = 1;
    listmax = 2;
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_politics.bmp", 1);
    gsel(0);
    fillbg(3, 960, 96, 128, 128);
    render_hud();
    gsel(7);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"ie_scroll.bmp");
    gsel(0);
    windowshadow = 1;
    snd(92);
    drawmenu(3);
    city = 1;
    ww = 540;
    wh = 440;
    wx = (windoww - ww) / 2 + inf_screenx;
    wy = winposy(wh);
label_2277_internal:
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
label_2278_internal:
    s = strhint2 + strhint3b;
    showscroll(s, wx, wy, ww, wh);
    font(14 - en * 2);
    if (adata(28, gdata_current_map) == 0 || gdata_current_dungeon_level != 1)
    {
        pos(wx + 40, wy + 60);
        mes(i18n::s.get("core.locale.ui.city_chart.no_economy"));
    }
    else
    {
        if (page == 0)
        {
            display_topic(
                i18n::s.get("core.locale.ui.economy.information"),
                wx + 65,
                wy + 50);
            display_topic(
                i18n::s.get("core.locale.ui.economy.finance"),
                wx + 65,
                wy + 150);
            font(14 - en * 2);
            x = wx + 50;
            y = wy + 80;
            showeconomy(
                x,
                y,
                i18n::s.get("core.locale.ui.economy.population"),
                podata(100, city),
                podata(101, city));
            x = wx + 50;
            y = wy + 180;
            showeconomy(
                x,
                y,
                i18n::s.get("core.locale.ui.economy.basic_tax") + u8" ("s
                    + gdata(820) + u8"%)"s,
                podata(102, city),
                podata(103, city));
            showeconomy(
                x,
                y + 16,
                i18n::s.get("core.locale.ui.economy.excise_tax") + u8" ("s
                    + podata(150, city) + u8"%)"s,
                podata(104, city),
                podata(105, city));
        }
        if (page == 1)
        {
            display_topic(
                i18n::s.get("core.locale.ui.economy.population_detail"),
                wx + 65,
                wy + 50);
            display_topic(
                i18n::s.get("core.locale.ui.economy.finance_detail"),
                wx + 65,
                wy + 200);
            font(14 - en * 2);
        }
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 2)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 2;
                }
            }
            key = "";
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    show_city_chart();
                    return;
                }
                if (curmenu == 1)
                {
                    show_economy_window();
                    return;
                }
                if (curmenu == 2)
                {
                    show_politics_window();
                    return;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_2277_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_2277_internal;
        }
    }
    if (key != ""s)
    {
        update_screen();
        return;
    }
    goto label_2278_internal;
}

void show_politics_window()
{
    // TODO: untranslated
    listmax = 0;
    page = 0;
    pagesize = 13;
    cs = 0;
    cs_bk = -1;
    curmenu = 2;
    city = adata(28, gdata_current_map);
    list(0, listmax) = 1;
    listn(0, listmax) =
        i18n::s.get("core.locale.ui.politics.name", mapname(gdata(815)));
    ++listmax;
    if (mdata_map_type != mdata_t::map_type_t::town)
    {
        goto label_2281_internal;
    }
    list(0, listmax) = 0;
    listn(0, listmax) =
        i18n::s.get("core.locale.ui.politics.taxes", podata(150, city));
    ++listmax;
    p = rnd(1000);
    list(0, listmax) = 0;
    listn(0, listmax) =
        i18n::s.get("core.locale.ui.politics.well_pollution", p(0));
    ++listmax;
    list(0, listmax) = 0;
    listn(0, listmax) = i18n::s.get("core.locale.ui.politics.lawless");
    ++listmax;
label_2281_internal:
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_politics.bmp", 1);
    gsel(0);
    fillbg(3, 960, 96, 128, 128);
    render_hud();
    gsel(7);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"ie_scroll.bmp");
    gsel(0);
    windowshadow = 1;
    snd(92);
    drawmenu(3);
    city = 1;
    ww = 480;
    wh = 400;
    wx = (windoww - ww) / 2 + inf_screenx;
    wy = winposy(wh);
label_2282_internal:
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
label_2283_internal:
    s = strhint2 + strhint3b;
    showscroll(s, wx, wy, ww, wh);
    display_topic(i18n::s.get("core.locale.ui.politics.law"), wx + 65, wy + 45);
    font(12 + sizefix - en * 2);
    pos(wx + 185, wy + 52);
    mes(i18n::s.get("core.locale.ui.politics.global"));
    if (mdata_map_type == mdata_t::map_type_t::town)
    {
        pos(wx + 285, wy + 52);
        mes(i18n::s.get(
            "core.locale.ui.politics.law_of", mapname(gdata_current_map)));
    }
    pos(wx + 155, wy + 46);
    gmode(2);
    gcopy(3, 312, 360, 24, 24);
    pos(wx + 255, wy + 46);
    gmode(2);
    gcopy(3, 288, 360, 24, 24);
    font(14 - en * 2);
    keyrange = 0;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_select(cnt);
        ++keyrange;
        if (cnt % 2 == 0)
        {
            boxf(wx + 74, wy + 76 + cnt * 19, 365, 18, {12, 14, 16, 16});
        }
        display_key(wx + 72, wy + 76 + cnt * 19 - 2, cnt);
    }
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        i = list(0, p);
        s = listn(0, p);
        cs_list(cs == cnt, s, wx + 100, wy + 76 + cnt * 19 - 1, 0, 0);
        pos(wx + 42, wy + 68 + cnt * 19 + 2);
        gmode(2);
        gcopy(3, 288 + list(0, p) * 24, 360, 24, 24);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 2)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 2;
                }
            }
            key = "";
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    show_city_chart();
                    return;
                }
                if (curmenu == 1)
                {
                    show_economy_window();
                    return;
                }
                if (curmenu == 2)
                {
                    show_politics_window();
                    return;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_2282_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_2282_internal;
        }
    }
    if (key == key_cancel)
    {
        update_screen();
        return;
    }
    goto label_2283_internal;
}

void begin_to_believe_god(int god_id)
{
    bool already_believing = !cdata.player().god_id.empty();

    auto result = ui::ui_menu_god(god_id, already_believing).show();

    if (!result.canceled && result.value)
    {
        rtval = *result.value;
        god_proc_switching_penalty();
    }
}


void house_board_update_screen()
{
    screenupdate = -1;
    update_screen();
}



int ctrl_ally(ctrl_ally_operation operation)
{
    auto result = ui::ui_menu_ctrl_ally(operation).show();

    if (!result.canceled && result.value)
    {
        return *result.value;
    }
    else
    {
        return -1;
    }
}



void screen_analyze_self()
{
    // TODO: untranslated
    if (rc < 0)
    {
        rc = tc;
        if (rc < 0)
        {
            rc = 0;
        }
    }
    listmax = 0;
    page = 0;
    pagesize = 14;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    snd(26);
    buff = "";
    notesel(buff);
    chara_delete(56);
    cdata.tmp().piety_point = cdata.player().piety_point;
    cdata.tmp().god_id = cdata.player().god_id;
    for (int cnt = 0; cnt < 600; ++cnt)
    {
        sdata(cnt, rc) = 1;
    }
    apply_god_blessing(56);
    if (!cdata.player().god_id.empty())
    {
        buff += u8"<title1>◆ "s
            + i18n::_(u8"god", cdata.player().god_id, u8"name")
            + u8"による能力の恩恵<def>\n"s;
        for (int cnt = 0; cnt < 600; ++cnt)
        {
            p = sdata(cnt, rc) - 1;
            cnvbonus(cnt, p);
        }
    }
    refreshmode = 1;
    chara_refresh(0);
    refreshmode = 0;
    buff += u8"\n"s;
    buff += u8"<title1>◆ 特徴と特殊状態による能力の恩恵<def>\n"s;
    listmax = noteinfo();
label_1965_internal:
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
    s(0) = lang(u8"自己の分析", u8"Analyze Self");
    s(1) = strhint2 + strhint3b;
    display_window((windoww - 400) / 2 + inf_screenx, winposy(448), 400, 448);
    s = lang(u8"分析結果", u8"Analysis Result");
    display_topic(s, wx + 28, wy + 36);
    font(14 - en * 2);
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        noteget(s, p);
        pos(wx + 54, wy + 66 + cnt * 19 + 2);
        gmes(s, wx, wy + 66 + cnt * 19 + 2, 600, {30, 30, 30}, false);
    }
    redraw();
label_1966_internal:
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    p = -1;
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_1965_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_1965_internal;
        }
    }
    if (key == key_cancel)
    {
        return;
    }
    goto label_1966_internal;
}

int change_npc_tone()
{
    auto result = ui::ui_menu_npc_tone().show();

    if (result.canceled)
    {
        return -1;
    }

    snd(20);
    txt(i18n::s.get(
        "core.locale.action.interact.change_tone.is_somewhat_different",
        cdata[tc]));

    if (result.value)
    {
        cdata[tc].has_custom_talk() = true;
        cdatan(4, tc) = *result.value;
    }
    else
    {
        cdata[tc].has_custom_talk() = false;
        cdatan(4, tc) = "";
    }

    return 1;
}



void show_book_window()
{
    ui::ui_menu_book(inv[ci].param1).show();
}


void item_show_description()
{
    if (ci < 0)
    {
        dialog(i18n::s.get("core.locale.item.desc.window.error"));
        return;
    }

    ui::ui_menu_item_desc(inv[ci]).show();

    returnfromidentify = 1;
}


} // namespace elona
