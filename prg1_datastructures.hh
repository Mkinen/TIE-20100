// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>

struct TownData
{
    std::string name;
    int x;
    int y;
    int TownDistance;
};

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: This is because it only returns int value.
    unsigned int size();

    // Estimate of performance: Θ(n)
    // Short rationale for estimate: The function geos linearly through all the items that have been added to vectors and struct.
    void clear();

    // Estimate of performance: O(1)
    // Short rationale for estimate: It only returns the needed vector.
    std::vector<TownData*> all_towns();

    // Estimate of performance: Θ(1)
    // Short rationale for estimate: This function creates one new TownData for struct and pushes it to vectors.
    // Also manages some variables.
    TownData* add_town(std::string const& name, int x, int y);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: This function calls merge sort which is why its worst-case is nlogn. It also might go with just Θ(1) when
    // the towns are already in order. If used vector is ordered to some point, it only orders the items that are not in order and then combines
    // these two parts.
    std::vector<TownData*> towns_alphabetically();

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: Same rationale as above because exact same function as towns_alphabetically().
    std::vector<TownData*> towns_distance_increasing();

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: Has the same merge sort as two functions above. Also has binary search which has worst-case of O(logn).
    TownData* find_town(std::string const& name);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: Also uses merge sort if town are not in order already. If towns are in order, the function just returns
    // the first item in vector that is oredered by distance.
    TownData* min_distance();

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: Works as min_distance() but returns the last iteam.
    TownData* max_distance();

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: Uses merge sort to sort TownsByDistance vector and then returns asked item.
    // Also checks for valid input.
    TownData* nth_distance(unsigned int n);

    // Non-compulsory operations

    // Estimate of performance: O(nlogn), Ω(n)
    // Short rationale for estimate: Uses merge sort to sort the items in vector. After that it searches the needed item with binary search.
    // Then removes the item and then it needs to copy TownsByAlphabets vector to TownsByDistance which is linear function.
    // Also removing (erase()) is linear.
    void remove_town(std::string const& town_name);

    // Estimate of performance: Θ(nlogn)
    // Short rationale for estimate: Creates temporary vector in which saves towns with new calculated distances. After that merge sort is needed
    // for sorting. The new vector can't be in order and because of that performance is Θ(nlogn).
    std::vector<TownData*> towns_distance_increasing_from(int x, int y);

private:
    // Add stuff needed for your class implementation here

    // Two vectors for storing data.
    std::vector<TownData*> TownsByAlphabets; // Stores towns by alphabets.
    std::vector<TownData*> TownsByDistance; // Stores towns by distances.

    // Three variables for count keeping.
    int TownCount; // Counts amount of towns.
    int addedItemsToAlpha; // Counts how many towns have been added after last alphabetical merge sort.
    int addedItemsToDist; // Counts how many towns have been added after last merge sort for distances.

    // Definition for merge.
    void merge(std::vector<TownData*> &aVector, int low, int middle, int high,
               bool (Datastructures::*compare)
               (std::vector<TownData*> &left, std::vector<TownData*> &right, int i, int j));

    // Definition for merge sort.
    void mergeSort(std::vector<TownData*> &aVector, int low, int high,
                   bool (Datastructures::*compare)
                   (std::vector<TownData*> &left, std::vector<TownData*> &right, int i, int j));

    // These are needed for deciding whether to compare alphabets or distances.
    bool alphabetical(std::vector<TownData*> &left, std::vector<TownData*> &right, int i, int j);
    bool byDistance(std::vector<TownData*> &left, std::vector<TownData*> &right, int i, int j);

    // Definitions for binary search.
    int alphabetBinarySearch(std::vector<TownData*> &aVector, int left, int right, std::string x);

    // Just functions that sort vectors TownsByAlphabets and TownsByDistance if needed. Does not return anything.
    void sort_towns_by_alphabets();
    void sort_towns_by_distance();

};

#endif // DATASTRUCTURES_HH
