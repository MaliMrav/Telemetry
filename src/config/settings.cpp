#include "settings.h"
#include "TZinfo.h"

const String TimeConfig::TIMEZONE = getTzInfo("Australia/Sydney");

const String TimeConfig::WDAY_NAMES[7]  = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
const String TimeConfig::MONTH_NAMES[12] = {"JAN","FEB","MAR","APR","MAY","JUN",
                                            "JUL","AUG","SEP","OCT","NOV","DEC"};
