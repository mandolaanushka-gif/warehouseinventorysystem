
#include "wims.h"
#include <ctime>

// ================= CURRENT DATE FUNCTION =================

string getCurrentDate()
{
    time_t currentTime = time(0);

    tm* localTime = localtime(&currentTime);

    if (localTime == NULL)
    {
        return "Date Not Available";
    }

    stringstream date;

    date << localTime->tm_mday << "-"
         << localTime->tm_mon + 1 << "-"
         << localTime->tm_year + 1900;

    return date.str();
}
