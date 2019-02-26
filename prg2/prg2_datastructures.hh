// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <unordered_map>
#include <memory>
#include <algorithm>

// Type for town IDs
using TownID = std::string;

// Return value for cases where required town was not found
TownID const NO_ID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
std::string const NO_NAME = "-- unknown --";

struct TownData
{
    std::string name;
    int x;
    int y;
    int TownDistance;
    int tax;
    int distanceFrom = 0;
    TownID master = NO_ID;
    std::vector<TownID> vassals = {};
};

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Just returns integer.
    unsigned int size();

    // Estimate of performance: Θ(n)
    // Short rationale for estimate: Deletes linearly every saved element from 3 different containers.
    void clear();

    // Estimate of performance: Θ(n), O(n)
    // Short rationale for estimate: Find for unordered map is constant at average but in worst case it is linear.
    std::string get_name(TownID id);

    // Estimate of performance: Θ(n), O(n)
    // Short rationale for estimate: Find for unordered map is constant at average but in worst case it is linear.
    std::pair<int, int> get_coordinates(TownID id);

    // Estimate of performance: Θ(n), O(n)
    // Short rationale for estimate: Find for unordered map is constant at average but in worst case it is linear.
    int get_tax(TownID id);

    // Estimate of performance: Θ(nlogn)
    // Short rationale for estimate: This is because vassals need to be sorted before returning them.
    std::vector<TownID> get_vassals(TownID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: This just returns vector.
    std::vector<TownID> all_towns();

    // Estimate of performance: Θ(1)
    // Short rationale for estimate: Adds new town and its variables to containers and
    // modifies already determined variables if necessary.
    bool add_town(TownID id, std::string const& name, int x, int y, int tax);

    // Estimate of performance: Θ(nlogn)
    // Short rationale for estimate: This function uses binary search to find if value is in certain range
    // and also to find positions.
    bool change_town_name(TownID id, std::string const& newname);

    // Estimate of performance: Θ(nlogn)
    // Short rationale for estimate: Uses sort function from stl. If the vector is already sorted,
    // estimate of performance is O(1).
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance:  Θ(nlogn)
    // Short rationale for estimate: Uses sort function from stl. If the vector is already sorted,
    // estimate of performance is O(1).
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance:  Θ(nlogn)
    // Short rationale for estimate: Vector is sorted at the start of function which causes function to be  Θ(nlogn).
    std::vector<TownID> find_towns(std::string const& name);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Just returns variable that has needed TownID.
    TownID min_distance();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Just returns variable that has needed TownID.
    TownID max_distance();

    // Estimate of performance: Θ(nlogn)
    // Short rationale for estimate: Sorts vector at the start of the function to provide needed distance.
    // This causes function to be Θ(nlogn).
    TownID nth_distance(unsigned int n);

    // Estimate of performance: Θ(1)
    // Short rationale for estimate: Find functions for unordered map is constant and also adding new values to containsers.
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: Θ(n)
    // Short rationale for estimate: Linearity depends on the amount of masters. Likely not so heavy function after all.
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory operations

    // Estimate of performance:  Θ(nlogn)
    // Short rationale for estimate: Here two vectors are sorted and there is also binary search used three times.
    bool remove_town(TownID id);

    // Estimate of performance: Θ(n)
    // Short rationale for estimate: Linear because new vector is needed to store every element with new distance.
    // This function also uses sort function for sorting new vector.
    std::vector<TownID> towns_distance_increasing_from(int x, int y);

    // Estimate of performance: Θ(n^n)
    // Short rationale for estimate: It seems this functions complexity is Θ(n^n) because every vassal of every master need
    // to be checked. This function calls new fuction calles maxDepth which recursively goed through all vassals.
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: Θ(n^n)
    // Short rationale for estimate: Has same complexity as longest_vassal_path for same reasons.
    // Every vassal of vassals need to be checked for taxes.
    int total_net_tax(TownID id);

private:

    // Vectors for storing towns in alphabetical and distance oreder.
    std::vector<TownID> alphabetical;
    std::vector<TownID> distance;

    // Here are stored all TownIDs with their struct.
    std::unordered_map<TownID, std::shared_ptr<TownData>> Towns;

    // Variables to store amount of added towns after last sorting.
    int addedToAplha;
    int addedToDist;

    // Counts amount of towns.
    unsigned int TownCount;

    // Here are variables for storing minimum and maximum distances.
    TownID minDistance;
    TownID maxDistance;

    // Returns tax from one vassal and it's vassals.
    int taxesOfValssal(TownID id);

    // Used in longest_vassal_path. Returns longest depth of vassals in a vector.
    std::vector<TownID> maxDepth(TownID id);

    // These two just sorts vectors 'alphabetical' and 'distance' with no return.
    void towns_alphabetically_with_no_return();
    void towns_distance_increasing_with_no_return();
};

#endif // DATASTRUCTURES_HH
