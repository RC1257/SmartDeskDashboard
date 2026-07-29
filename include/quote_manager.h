#pragma once

#include <stdint.h>

namespace QuoteManager
{
    // Initialize and display today's quote.
    void init();

    // Select a quote based on a day number.
    void setQuoteForDay(uint32_t day);

    // Current quote information.
    const char* getCurrentQuote();
    const char* getCurrentAuthor();
}