// class to manage min and max ranges
#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
    public:
        // member variables min and max of range
        double min, max;

        // constructor
        interval() : min(+infinity), max(-infinity) {} // default range empty (no valid range)

        interval(double min, double max) : min(min), max(max) {}

        // get size of interval
        double size() const {
            return max - min;
        }

        // check if a value is in the range
        // weak check -> min <= x <= max
        bool contains(double x) const {
            return min <= x && max >= x;
        }

        // strong check min < x < max
        bool surrounds(double x) const {
            return min < x && max > x;
        }

        // helper functon to clamp values within the interval 
        double clamp(double x) const {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }

        // declare 2 static class variables, empty and universe
        // 'static' means belong to class itself, not any instance -> for many intervals, only 1 shared set exists
        static const interval empty, universe;
};

// define constant intervals of the class static variables ::empty and ::universe
const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);


#endif