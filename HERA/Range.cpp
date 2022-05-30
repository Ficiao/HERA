#include "Range.h"

// Returns true if number is contained within the range, returns false otherwise
bool Range::contains(int number) {
    return number < end && number >= start;
}

Range::Range(int start, int anEnd, Connection con) : start(start), end(anEnd), connection(con) {}

