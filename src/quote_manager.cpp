#include "../include/quote_manager.h"

#include "lvgl.h"
#include "ui.h"

#include <ctime>

namespace
{
    struct QuoteData
    {
        const char* text;
        const char* author;
    };

    constexpr QuoteData quotes[] =
    {
        {
            "Success is the sum of small efforts, repeated day in and day out.",
            "Robert Collier"
        },
        {
            "The secret of getting ahead is getting started.",
            "Mark Twain"
        },
        {
            "You have to work hard in the dark to shine in the light.",
            "Kobe Bryant"
        },
        {
            "A winner is a loser who tried one more time.",
            "George M. Moore Jr."
        },
        {
            "Do what you can, with what you have, where you are.",
            "Theodore Roosevelt"
        },
        {
            "The future depends on what you do today.",
            "Mahatma Gandhi"
        },
        {
            "Believe you can and you're halfway there.",
            "Theodore Roosevelt"
        }
    };

    constexpr uint32_t QUOTE_COUNT =
        sizeof(quotes) / sizeof(quotes[0]);

    uint32_t current_quote_index = 0;


    void update_display()
    {
        lv_label_set_text(
            ui_LabelQuotePhrase,
            quotes[current_quote_index].text
        );

        lv_label_set_text(
            ui_LabelQuoteAuthor,
            quotes[current_quote_index].author
        );
    }


    uint32_t get_current_day()
    {
        std::time_t now = std::time(nullptr);

        std::tm* local_time =
            std::localtime(&now);

        if(local_time == nullptr)
        {
            return 0;
        }

        return static_cast<uint32_t>(
            local_time->tm_yday
        );
    }
}


namespace QuoteManager
{
    void init()
    {
        /*
         * Determine today's quote when
         * the dashboard starts.
         */
        setQuoteForDay(
            get_current_day()
        );
    }


    void setQuoteForDay(uint32_t day)
    {
        /*
         * Cycle through the available quotes.
         */
        current_quote_index =
            day % QUOTE_COUNT;

        update_display();
    }


    const char* getCurrentQuote()
    {
        return quotes[
            current_quote_index
        ].text;
    }


    const char* getCurrentAuthor()
    {
        return quotes[
            current_quote_index
        ].author;
    }
}