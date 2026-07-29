#pragma once

namespace ThemeManager
{
    enum class Theme
    {
        LIGHT,
        DARK,
        PINK
    };

    void init();

    void setTheme(Theme theme);
    void applyCurrentTheme();

    Theme getTheme();
}