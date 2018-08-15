#pragma once
#include "lua_api_common.hpp"

namespace elona
{
namespace lua
{

namespace Input
{
bool yes_no(const std::string&);

sol::optional<int> prompt_number(const std::string&, int);
sol::optional<int> prompt_number_with_initial(const std::string&, int, int);

sol::optional<std::string> prompt_text(const std::string&, bool);

sol::optional<int> prompt_choice(sol::variadic_args);


void bind(sol::table&);
}; // namespace Input

} // namespace lua
} // namespace elona
