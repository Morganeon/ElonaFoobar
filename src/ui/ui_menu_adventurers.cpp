#include "ui_menu_adventurers.hpp"
#include "../audio.hpp"
#include "../character.hpp"
#include "../draw.hpp"
#include "../i18n.hpp"

namespace elona
{
namespace ui
{

void ui_menu_adventurers::init()
{
    listmax = 0;
    page = 0;
    pagesize = 16;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    for (int cnt = 0; cnt < 56; ++cnt)
    {
        if (cdata[cnt].state() == character::state_t::empty)
        {
            continue;
        }
        list(0, listmax) = cnt;
        list(1, listmax) = -cdata[cnt].fame;
        ++listmax;
    }
    sort_list_by_column1();
    windowshadow = 1;
}

void ui_menu_adventurers::update()
{
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
}

static void _draw_window()
{
    s(0) = i18n::s.get("core.locale.ui.adventurers.title");
    s(1) = strhint2 + strhint3;
    display_window((windoww - 640) / 2 + inf_screenx, winposy(448), 640, 448);
    display_topic(
        i18n::s.get("core.locale.ui.adventurers.name_and_rank"),
        wx + 28,
        wy + 36);
    display_topic(
        i18n::s.get("core.locale.ui.adventurers.fame_lv"), wx + 320, wy + 36);
    display_topic(
        i18n::s.get("core.locale.ui.adventurers.location"), wx + 420, wy + 36);
}

static void _draw_key(int cnt)
{
    if (cnt % 2 == 0)
    {
        boxf(wx + 70, wy + 66 + cnt * 19, 540, 18, {12, 14, 16, 16});
    }
    display_key(wx + 58, wy + 66 + cnt * 19 - 2, cnt);
}

static void _draw_keys()
{
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
        _draw_key(cnt);
    }
}

static void
_draw_list_entry_pic_and_rank(int cnt, const character& chara, int _p)
{
    draw_chara_scale_height(chara, wx + 40, wy + 74 + cnt * 19 - 8);

    pos(wx + 84, wy + 66 + cnt * 19 + 2);
    mes(cnvrank(_p + 1)
        + i18n::s.get("core.locale.ui.adventurers.rank_counter"));
}

static void _draw_list_entry_name(int cnt, const character& chara)
{
    std::string name =
        ""s + cdatan(1, chara.index) + u8" "s + cdatan(0, chara.index);
    cutname(name, 26);

    cs_list(cs == cnt, name, wx + 118, wy + 66 + cnt * 19 - 1);
}

static void _draw_list_entry_level(int cnt, const character& chara)
{
    std::string level = ""s + chara.fame + u8"("s + chara.level + u8")"s;
    pos(wx + 402 - strlen_u(level) * 7, wy + 66 + cnt * 19 + 2);
    mes(level);
}

static void _draw_list_entry_map_name(int cnt, const character& chara)
{
    std::string map_name = mapname(chara.current_map);

    if (map_name == ""s)
    {
        map_name = i18n::s.get("core.locale.ui.adventurers.unknown");
    }
    if (chara.state() == character::state_t::adventurer_dead)
    {
        map_name = i18n::s.get("core.locale.ui.adventurers.hospital");
    }

    pos(wx + 435, wy + 66 + cnt * 19 + 2);
    mes(map_name);
}

static void _draw_list_entry(int cnt, const character& chara, int _p)
{
    _draw_list_entry_pic_and_rank(cnt, chara, _p);
    _draw_list_entry_name(cnt, chara);
    _draw_list_entry_level(cnt, chara);
    _draw_list_entry_map_name(cnt, chara);
}

static void _draw_list_entries()
{
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }

        int chara_index = list(0, p);
        const character& chara = cdata[chara_index];

        _draw_list_entry(cnt, chara, p(0));
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
}

void ui_menu_adventurers::draw()
{
    _draw_window();
    _draw_keys();
    _draw_list_entries();
}

optional<ui_menu_adventurers::result_type> ui_menu_adventurers::on_key(
    const std::string& key)
{
    ELONA_GET_SELECTED_ITEM(p, 0);

    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            set_reupdate();
        }
    }
    else if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            set_reupdate();
        }
    }
    else if (key == key_cancel)
    {
        return ui_menu_adventurers::result::finish();
    }

    return none;
}
} // namespace ui
} // namespace elona
