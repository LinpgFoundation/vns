#ifndef TAGS_HPP
#define TAGS_HPP

#include <string_view>

struct tags
{
    constexpr static std::string_view vns = "vns";
    constexpr static std::string_view background_image = "bgi";
    constexpr static std::string_view background_music = "bgm";
    constexpr static std::string_view show = "show";
    constexpr static std::string_view hide = "hide";
    constexpr static std::string_view display = "disp";
    constexpr static std::string_view id = "id";
    constexpr static std::string_view language = "lang";
    constexpr static std::string_view section = "sect";
    constexpr static std::string_view end = "end";
    constexpr static std::string_view scene = "scene";
    constexpr static std::string_view block = "block";
    constexpr static std::string_view option = "opt";
    constexpr static std::string_view jump = "jmp";
    constexpr static std::string_view jump_ = "jmp_";
    constexpr static std::string_view label = "label";
    static const std::unordered_map<std::string_view, std::string_view> inline alternatives = {{"language", language},
                                                                                               {"option",   option},
                                                                                               {"display",  display},
                                                                                               {"jump",     jump},
                                                                                               {"jump_",    jump_},
                                                                                               {"section",  section}};
};

#endif
