#ifdef ANDROID
#include "../hsp.hpp"
#include "../touch_input.hpp"
#endif

namespace elona
{
namespace snail
{

touch_input& touch_input::instance()
{
    static touch_input the_instance;
    return the_instance;
}

int touch_input::quick_action_size() const noexcept
{
    return static_cast<int>(
        _base_quick_action_size * application::instance().dpi());
}

void touch_input::initialize(const fs::path& asset_path)
{
    _quick_action_image =
        std::make_unique<basic_image>(asset_path / "quick_action.png");

    initialize_quick_actions();

    _initialized = true;
}

void touch_input::initialize_quick_actions()
{
    int width = application::instance().physical_width();
    int height = application::instance().physical_height();
    int size = quick_action_size();
    int space_between = size * 0.75;

    _quick_actions.clear();

    std::vector<std::pair<std::string, snail::key>> keys = {
        {"7", snail::key::keypad_7},
        {"8", snail::key::keypad_8},
        {"9", snail::key::keypad_9},
        {"4", snail::key::keypad_4},
        {"5", snail::key::keypad_5},
        {"6", snail::key::keypad_6},
        {"1", snail::key::keypad_1},
        {"2", snail::key::keypad_2},
        {"3", snail::key::keypad_3}};

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            auto pair = keys.at(j + ((2 - i) * 3));
            std::string name = pair.first;
            snail::key key = pair.second;

            int x = space_between + (j * space_between);
            int y = height - space_between - (i * space_between);

            _quick_actions.emplace_back(name, key, x, y);
        }
    }

    using tuples_t =
        std::tuple<std::string, optional<snail::key>, int, int, bool>[];
    for (const auto& tuple : tuples_t{
             {"OK", snail::key::enter, 1, 1, true},
             {"Back", snail::key::shift, 2, 1, true},
             {"z", none, 1, 2, true},
             {"x", none, 2, 2, true},
             {"g", none, 1, 3, true},
             {"d", none, 2, 3, true},
             {"f", none, 2, 4, true},
             {"Esc", snail::key::escape, 1, 4, true},
             {"v", none, 1, 4, false},
             {"/", none, 2, 4, false},
             {"*", none, 3, 4, false},
         })
    {
        std::string text = std::get<0>(tuple);
        optional<snail::key> key = std::get<1>(tuple);
        int w;
        if (std::get<4>(tuple))
        {
            w = width - space_between * std::get<2>(tuple);
        }
        else
        {
            w = space_between * std::get<2>(tuple);
        }
        int h = height - space_between * std::get<3>(tuple);

        _quick_actions.emplace_back(text, key, w, h);
    }
}

void touch_input::draw_quick_actions()
{
    if (!_initialized)
    {
        return;
    }

    auto& renderer = application::instance().get_renderer();
    renderer.set_blend_mode(snail::blend_mode_t::blend);
    renderer.set_text_alignment(renderer::text_alignment_t::center);
    renderer.set_text_baseline(renderer::text_baseline_t::middle);

    for (auto it = _quick_actions.begin(); it < _quick_actions.end(); it++)
    {
        draw_quick_action(*it);
    }

    renderer.set_blend_mode(snail::blend_mode_t::none);
    renderer.set_text_alignment(renderer::text_alignment_t::left);
    renderer.set_text_baseline(renderer::text_baseline_t::top);
}

bool touch_input::is_touched(int x, int y, const quick_action& action)
{
    int size = quick_action_size() / 2;
    int deadzone = ((float)size * 0.75);

    return x > action.center_x - deadzone && y > action.center_y - deadzone
        && x <= action.center_x + deadzone && y <= action.center_y + deadzone;
}

void touch_input::draw_quick_action(const quick_action& action)
{
    int size = quick_action_size() / 2;
    int deadzone = ((float)size * 0.75);
    float dpi = application::instance().dpi();
    int x = action.center_x - deadzone;
    int y = action.center_y - deadzone;
    auto& renderer = application::instance().get_renderer();

    renderer.render_image(
        *_quick_action_image.get(), x, y, deadzone * 2, deadzone * 2);

    if (renderer.has_font())
    {
        renderer.render_text(
            action.text,
            action.center_x,
            action.center_y,
            {255, 255, 255, quick_action_alpha()},
            _base_font_size * dpi);
    }

    if (action.touched)
    {
        renderer.set_draw_color(
            {192, 192, 192, static_cast<uint8_t>(quick_action_alpha() / 2)});
        renderer.fill_rect(x, y, deadzone * 2, deadzone * 2);
    }
}

void touch_input::on_touch_event(::SDL_TouchFingerEvent event)
{
    int norm_x = application::instance().physical_width() * event.x;
    int norm_y = application::instance().physical_height() * event.y;
    _last_touched_quick_action_idx = none;

    for (auto it = _quick_actions.begin(); it < _quick_actions.end(); it++)
    {
        quick_action& action = *it;

        if (_last_touched_quick_action_idx == none
            && event.type != event_type::up
            && is_touched(norm_x, norm_y, action))
        {
            action.touched = true;
            _last_touched_quick_action_idx = it - _quick_actions.begin();
        }
        else
        {
            action.touched = false;
        }
    }
}

} // namespace snail
} // namespace elona
