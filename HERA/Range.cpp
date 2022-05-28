#include "Range.h"

bool Range::contains(double number) {
    return number < end && number >= start;
}

Range::Range(double start, double anEnd, Connection con) : start(start), end(anEnd), connection(con) {}

