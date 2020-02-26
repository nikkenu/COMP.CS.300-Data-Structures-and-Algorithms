// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <unordered_map>
#include <memory>
#include <functional>
#include <set>
#include <cmath>
#include <math.h>

// Types for IDs
using StopID = long int;
using RegionID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
StopID const NO_STOP = -1;
RegionID const NO_REGION = "!!NO_REGION!!";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Basicly there's only couple if-clauses
    // and size function in unordered_map is O(1).
    int stop_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Basicly it goes throught
    // every element in unordered map and deletes it.
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Iterating map items is O(n)
    // and putting them to back of the vector is O(1).
    std::vector<StopID> all_stops();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Time complexity for finding map item
    // is O(1) in best case, but the worst case is O(n). Also, inserting to
    // unordered map has same time complexity that finding method has.
    bool add_stop(StopID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Because find method is O(1) in best case,
    // and in worst case it's O(n).
    Name get_stop_name(StopID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Because find method is O(1) in best case,
    // and in worst case it's O(n).
    Coord get_stop_coord(StopID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StopID> stops_alphabetically();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StopID> stops_coord_order();

    // Estimate of performance:
    // Short rationale for estimate:
    StopID min_coord();

    // Estimate of performance:
    // Short rationale for estimate:
    StopID max_coord();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StopID> find_stops(Name const& name);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_stop_name(StopID id, Name const& newname);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_stop_coord(StopID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_region(RegionID id, Name const& name);

    // Estimate of performance:
    // Short rationale for estimate:
    Name get_region_name(RegionID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<RegionID> all_regions();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_stop_to_region(StopID id, RegionID parentid);

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<RegionID> stop_regions(StopID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    void creation_finished();

    // Estimate of performance:
    // Short rationale for estimate:
    std::pair<Coord, Coord> region_bounding_box(RegionID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StopID> stops_closest_to(StopID id);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_stop(StopID id);

    // Estimate of performance:
    // Short rationale for estimate:
    RegionID stops_common_region(StopID id1, StopID id2);
    
    double pythagorasCalc(Coord coord);

    double twoPointDistance(Coord coord1, Coord coord2);

private:
    struct RegionStructure
    {
        Name name = NO_NAME;
        std::shared_ptr<RegionID> parent;
        std::set<std::shared_ptr<RegionID>> children;
    };

    struct StopStructure
    {
        Name name = NO_NAME;
        Coord coordinate;
        std::shared_ptr<RegionID> region;
    };

    struct CompareDistance
    {
        bool operator()(const std::pair<StopID, StopStructure>&itemOne, const std::pair<StopID,StopStructure>& itemTwo) const {
            double itemOneDistance = sqrt(pow(itemOne.second.coordinate.x, 2) + pow(itemOne.second.coordinate.y,2));
            double itemTwoDistance = sqrt(pow(itemTwo.second.coordinate.x, 2) + pow(itemTwo.second.coordinate.y,2));
            if(itemOneDistance < itemTwoDistance){
                return true;
            } else if(itemOneDistance > itemTwoDistance){
                return false;
            } else {
                if(itemOne.second.coordinate.y < itemTwo.second.coordinate.y){
                    return true;
                } else if(itemOne.second.coordinate.y > itemTwo.second.coordinate.y){
                    return false;
                } else {
                    return true;
                }
            }
        }
    };

    std::unordered_map<RegionID, RegionStructure> m_regionContainer;
    std::unordered_map<StopID, StopStructure> m_container;
    typedef std::function<bool(std::pair<StopID, StopStructure>, std::pair<StopID, StopStructure>)> Comparator;
    typedef std::function<bool(std::pair<StopID, double>, std::pair<StopID, double>)> ComparatorDouble;
};

#endif // DATASTRUCTURES_HH
