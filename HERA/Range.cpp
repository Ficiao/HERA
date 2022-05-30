#include "Range.h"

//TODO: comment
bool Range::contains(int number) {
    return number < end && number >= start;
}

Range::Range(int start, int anEnd, Connection con) : start(start), end(anEnd), connection(con) {}

