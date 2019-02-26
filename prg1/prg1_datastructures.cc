// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <iostream>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}



Datastructures::Datastructures()
{
    TownCount = 0;
    addedItemsToAlpha = 0;
    addedItemsToDist = 0;
}

Datastructures::~Datastructures()
{
    clear();
}

unsigned int Datastructures::size()
{
    return TownCount; // Replace with actual implementation
}

void Datastructures::clear()
{
    for(; TownCount > 0; TownCount--){
        delete TownsByAlphabets.back();
        TownsByAlphabets.pop_back();
        TownsByDistance.pop_back();
    }
    addedItemsToAlpha = 0;
    addedItemsToDist = 0;
}

std::vector<TownData*> Datastructures::all_towns()
{
    return TownsByAlphabets;
}

TownData* Datastructures::add_town(const std::string& name, int x, int y)
{
    TownData* town = new TownData{name, x, y, std::abs(x) + std::abs(y)};
    TownsByAlphabets.push_back(town);
    TownsByDistance.push_back(town);
    ++TownCount;
    ++addedItemsToAlpha;
    ++addedItemsToDist;
    return town;
}

std::vector<TownData*> Datastructures::towns_alphabetically()
{
    if(addedItemsToAlpha == 0){
        return TownsByAlphabets;
    }
    else {
        int townIndex = TownCount - addedItemsToAlpha;
        if(townIndex == 0){
            mergeSort(TownsByAlphabets, 0, TownCount-1, &Datastructures::alphabetical);
        }
        else{
            mergeSort(TownsByAlphabets, townIndex, TownCount-1, &Datastructures::alphabetical);
            merge(TownsByAlphabets, 0, townIndex-1, TownCount-1, &Datastructures::alphabetical);
        }
    }
    addedItemsToAlpha = 0;
    return TownsByAlphabets;
}

std::vector<TownData*> Datastructures::towns_distance_increasing()
{
    if(addedItemsToDist == 0){
        return TownsByDistance;
    }
    else {
        int townIndex = TownCount - addedItemsToDist;
        if(townIndex == 0){
            mergeSort(TownsByDistance, 0, TownCount-1, &Datastructures::byDistance);
        }
        else{
            mergeSort(TownsByDistance, townIndex, TownCount-1, &Datastructures::byDistance);
            merge(TownsByDistance, 0, townIndex-1, TownCount-1, &Datastructures::byDistance);
        }
    }
    addedItemsToDist = 0;
    return TownsByDistance;
}


TownData* Datastructures::find_town(std::string const& name)
{
    sort_towns_by_alphabets();
    int result = alphabetBinarySearch(TownsByAlphabets, 0, TownCount - 1, name);
    if(result == -1){
        return nullptr;
    }
    return TownsByAlphabets[result];
}

TownData* Datastructures::min_distance()
{
    if(!TownsByAlphabets.empty()){
        if(addedItemsToDist == 0){
            return TownsByDistance[0];
        }

        sort_towns_by_distance();

        return TownsByDistance[0];
    }
    return nullptr;
}

TownData* Datastructures::max_distance()
{
    if(!TownsByDistance.empty()){
        if(addedItemsToDist == 0){
            return TownsByDistance[TownCount-1];
        }

        sort_towns_by_distance();

        return TownsByDistance[TownCount-1];
    }
    return nullptr;
}

TownData* Datastructures::nth_distance(unsigned int n)
{
    if(n == 0 || n > TownCount){
        return nullptr;
    }

    sort_towns_by_distance();

    return TownsByDistance[n - 1];
}

void Datastructures::remove_town(const std::string& town_name)
{
    sort_towns_by_alphabets();
    int result = alphabetBinarySearch(TownsByAlphabets, 0, TownCount - 1, town_name);

    if(result == -1){
        return;
    }

    TownsByAlphabets.erase(TownsByAlphabets.begin() + result);
    --TownCount;
    TownsByDistance.pop_back();

    for(int i = 0; i < TownCount; i++){
        TownsByDistance[i] = TownsByAlphabets[i];
    }

    addedItemsToDist = TownCount;
}

std::vector<TownData*> Datastructures::towns_distance_increasing_from(int x, int y)
{
    std::vector<TownData*> temp;
    temp.reserve(TownCount);
    int dist;
    for(int i = 0; i < TownCount; i++){
        dist = std::abs(TownsByDistance[i]->x - x) + std::abs(TownsByDistance[i]->y - y);
        temp.push_back(TownsByDistance[i]);
        temp[i]->TownDistance = dist;
    }

    mergeSort(temp, 0, TownCount-1, &Datastructures::byDistance);

    return temp;
}

void Datastructures::merge(std::vector<TownData*> &aVector, int left, int middle, int right,
                           bool (Datastructures::*compare)
                           (std::vector<TownData*> &left, std::vector<TownData*> &right, int i, int j))
{

    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    std::vector<TownData*> L;
    std::vector<TownData*> R;

    L.reserve(n1);
    R.reserve(n2);

    for(i = 0; i < n1; i++){
        L[i] = aVector[left+i];
    }
    for(j = 0; j < n2; j++){
        R[j] = aVector[middle+j+1];
    }

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2){
        if((this->*compare)(L, R, i, j)){     //(this->*compare)(temp, i, j))//{(L[i]->name <= R[j]->name){
            aVector[k] = L[i];
            i++;
        }
        else {
            aVector[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        aVector[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        aVector[k] = R[j];
        j++;
        k++;
    }
}

void Datastructures::mergeSort(std::vector<TownData*> &aVector, int left, int right,
                               bool (Datastructures::*compare)
                               (std::vector<TownData*> &left, std::vector<TownData*> &right, int i, int j))
{
    if (left < right){
        int middle = left+(right-left) / 2;
        mergeSort(aVector, left, middle, compare);
        mergeSort(aVector, middle+1, right, compare);
        merge(aVector, left, middle, right, compare);
    }
}

// Called if merge sort is done with alphabets.
bool Datastructures::alphabetical(std::vector<TownData*> &left, std::vector<TownData*> &right, int i, int j)
{
    return (left[i]->name <= right[j]->name);
}

// Called if merge sort is done with distances.
bool Datastructures::byDistance(std::vector<TownData*> &left, std::vector<TownData*> &right, int i, int j)
{
    return (left[i]->TownDistance <= right[j]->TownDistance);
}

// Normal binary search done with alphabets.
int Datastructures::alphabetBinarySearch(std::vector<TownData*> &aVector, int left, int right, std::string x)
{
    while(left<=right)
    {
        int middle = left + (right-left) / 2;

        if(aVector[middle]->name == x)
            return middle;

        if(aVector[middle]->name < x)
            left = middle + 1;
        else
            right = middle -1;
    }

    return -1;
}

// Does same as function towns_alphabetically() but doesn't return anything. Made just for sorting.
void Datastructures::sort_towns_by_alphabets()
{
    if(addedItemsToAlpha == 0){
        return;
    }
    else {
        int townIndex = TownCount - addedItemsToAlpha;
        if(townIndex == 0){
            mergeSort(TownsByAlphabets, 0, TownCount-1, &Datastructures::alphabetical);
        }
        else{
            mergeSort(TownsByAlphabets, townIndex, TownCount-1, &Datastructures::alphabetical);
            merge(TownsByAlphabets, 0, townIndex-1, TownCount-1, &Datastructures::alphabetical);
        }
    }
    addedItemsToAlpha = 0;
    return;
}

// Does same as function towns_distance_increasing() but doesn't return anything. Made just for sorting.
void Datastructures::sort_towns_by_distance()
{
    if(addedItemsToDist == 0){
        return;
    }
    else {
        int townIndex = TownCount - addedItemsToDist;
        if(townIndex == 0){
            mergeSort(TownsByDistance, 0, TownCount-1, &Datastructures::byDistance);
        }
        else{
            mergeSort(TownsByDistance, townIndex, TownCount-1, &Datastructures::byDistance);
            merge(TownsByDistance, 0, townIndex-1, TownCount-1, &Datastructures::byDistance);
        }
    }
    addedItemsToDist = 0;
    return;
}


