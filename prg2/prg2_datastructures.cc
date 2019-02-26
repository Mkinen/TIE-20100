// Datastructures.cc

#include "datastructures.hh"
#include <random>

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
    addedToAplha = 0;
    addedToDist = 0;
    minDistance = NO_ID;
    maxDistance = NO_ID;
}

Datastructures::~Datastructures()
{
    clear();
}

unsigned int Datastructures::size()
{
    return TownCount;
}

void Datastructures::clear()
{
    alphabetical.clear();
    distance.clear();
    Towns.clear();

    addedToAplha = 0;
    addedToDist = 0;
    TownCount = 0;

    minDistance = NO_ID;
    maxDistance = NO_ID;
}

std::string Datastructures::get_name(TownID id)
{
    auto town = Towns.find(id);
    if(town == Towns.end()){
        return NO_NAME;
    }
    return town->second->name;
}

std::pair<int, int> Datastructures::get_coordinates(TownID id)
{
    auto town = Towns.find(id);
    if(town == Towns.end()){
        return {NO_VALUE, NO_VALUE};
    }
    return {town->second->x, town->second->y};
}

int Datastructures::get_tax(TownID id)
{
    auto town = Towns.find(id);
    if(town == Towns.end()){
        return NO_VALUE;
    }
    return town->second->tax;
}

std::vector<TownID> Datastructures::get_vassals(TownID id)
{
    auto town = Towns.find(id);
    if(town == Towns.end()){
        return {NO_ID};
    }

    if(Towns[id]->vassals.size() > 1){
        std::sort(Towns[id]->vassals.begin(), Towns[id]->vassals.end(), [this](TownID a, TownID b){
            return a < b;
        });
    }

    return Towns[id]->vassals;
}

std::vector<TownID> Datastructures::all_towns()
{
    return alphabetical;
}

bool Datastructures::add_town(TownID id, const std::string& name, int x, int y, int tax)
{
    if(Towns.find(id) == Towns.end()){
        std::shared_ptr<TownData> town = std::shared_ptr<TownData>(new TownData{name, x, y, std::abs(x) + std::abs(y), tax});
        Towns[id] = town;

        // Determines new minimum distance if necessary
        if(minDistance == NO_ID){
            minDistance = id;
        } else {
            if(Towns[minDistance]->TownDistance > Towns[id]->TownDistance){
                minDistance = id;
            }
        }

        // Determines new maximum distance if necessary
        if(maxDistance == NO_ID){
            maxDistance = id;
        } else {
            if(Towns[maxDistance]->TownDistance < Towns[id]->TownDistance){
                maxDistance = id;
            }
        }

        alphabetical.push_back(id);
        distance.push_back(id);

        ++TownCount;
        ++addedToAplha;
        ++addedToDist;

        return true;
    }

    return false;
}

bool Datastructures::change_town_name(TownID id, const std::string& newname)
{
    auto town = Towns.find(id);
    if(town == Towns.end()){
        return false;
    }

    if(alphabetical.size() == 1){
        town->second->name = newname;
        return true;
    }

    // If used ID is in range where alphabetical vector is already sorted, it is fixed to its new place.
    if(std::binary_search(alphabetical.begin(), alphabetical.end()-addedToAplha, id, [this](TownID a, TownID id){
                          return Towns[a]->name < Towns[id]->name;})){
        auto oldPosition = std::lower_bound(alphabetical.begin(), alphabetical.end()-addedToAplha, town->second->name, [this](TownID a, std::string name){
                return Towns[a]->name < name;
        });
        town->second->name = newname;
        auto newPosition = std::lower_bound(alphabetical.begin(), alphabetical.end()-addedToAplha, newname, [this](TownID a, std::string name){
                return Towns[a]->name < name;
        });

        if(newPosition < oldPosition){
            std::rotate(newPosition, oldPosition, oldPosition+1);
        } else if(newPosition > oldPosition){
            std::rotate(oldPosition, oldPosition+1, newPosition+1);
        }
    } else{
        town->second->name = newname;
    }
    return true;
}

bool Datastructures::remove_town(TownID id)
{
    auto town = Towns.find(id);
    if(town == Towns.end()){
        return false;
    }

    TownID MastersID = Towns[id]->master;
    int numberOfVassals = Towns[id]->vassals.size();

    if(MastersID == NO_ID && numberOfVassals > 0){
        for(int i = 0; i < numberOfVassals; i++){
            Towns[Towns[id]->vassals[i]]->master = NO_ID;
        }
    } else if(MastersID != NO_ID && numberOfVassals > 0){
        auto positionInMastersVassals = std::lower_bound(Towns[MastersID]->vassals.begin(), Towns[MastersID]->vassals.end(), id, [this](TownID a, TownID id){
                return a < id;
        });
        Towns[MastersID]->vassals.erase(positionInMastersVassals);

        for(int i = 0; i < numberOfVassals; i++){
            Towns[Towns[id]->vassals[i]]->master = Towns[id]->master;
            Towns[Towns[id]->master]->vassals.push_back(Towns[id]->vassals[i]);
        }
    }

    towns_distance_increasing_with_no_return();
    towns_alphabetically_with_no_return();

    if(id == minDistance){
        if(TownCount == 1){
            minDistance = NO_ID;
        } else {
            minDistance = NO_ID;
            minDistance = distance[1];
        }
    }

    if(id == maxDistance){
        if(TownCount == 1){
            maxDistance = NO_ID;
        } else {
            maxDistance = NO_ID;
            maxDistance = distance[TownCount-2];
        }
    }

    auto posDist = std::lower_bound(distance.begin(), distance.end(), id, [this](TownID a, TownID id){
            return Towns[a]->TownDistance < Towns[id]->TownDistance;
    });
    auto posAlpha = std::lower_bound(alphabetical.begin(), alphabetical.end(), id, [this](TownID a, TownID id){
            return Towns[a]->name < Towns[id]->name;
    });

    alphabetical.erase(posAlpha);
    distance.erase(posDist);
    Towns.erase(id);
    --TownCount;

    return true;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    if(addedToAplha == 0){
        return alphabetical;
    }
    else {
        int sortedUntilIndex = TownCount - addedToAplha;
        if(sortedUntilIndex == 0){
            std::sort(alphabetical.begin(), alphabetical.end(), [this](TownID a, TownID b){
                return Towns[a]->name < Towns[b]->name;
            });
        }
        else{
            std::sort(alphabetical.begin() + sortedUntilIndex, alphabetical.end(), [this](TownID a, TownID b){
                return Towns[a]->name < Towns[b]->name;
            });
            std::inplace_merge(alphabetical.begin(), alphabetical.begin() + sortedUntilIndex, alphabetical.end());
        }
    }
    addedToAplha = 0;
    return alphabetical;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    if(addedToDist == 0){
        return distance;
    }
    else {
        int sortedUntilIndex = TownCount - addedToDist;
        if(sortedUntilIndex == 0){
            std::sort(distance.begin(), distance.end(), [this](TownID a, TownID b){
                return Towns[a]->TownDistance < Towns[b]->TownDistance;
            });
        }
        else{

            std::sort(distance.begin() + sortedUntilIndex, distance.end(), [this](TownID a, TownID b){
                return Towns[a]->TownDistance < Towns[b]->TownDistance;
            });
            std::inplace_merge(distance.begin(), distance.begin() + sortedUntilIndex, distance.end());
        }
    }
    addedToDist = 0;
    return distance;
}

std::vector<TownID> Datastructures::find_towns(std::string const& name)
{

    towns_alphabetically_with_no_return();
    std::vector<TownID> foundTowns = {};

    auto lower = std::lower_bound(alphabetical.begin(), alphabetical.end(), name, [this](TownID a, std::string const& name){
            return Towns[a]->name < name;
    });


    while(lower != alphabetical.end() && Towns[*lower]->name == name){
        foundTowns.push_back(*lower);
        ++lower;

    }

    std::sort(foundTowns.begin(), foundTowns.end(), [this](TownID a, TownID b){
        return a < b;
    });


    return foundTowns;
}

TownID Datastructures::min_distance()
{
    return minDistance;
}

TownID Datastructures::max_distance()
{
    return maxDistance;
}

TownID Datastructures::nth_distance(unsigned int n)
{
    if(TownCount < n || n == 0){
        return NO_ID;
    }
    towns_distance_increasing_with_no_return();
    return distance[n - 1];
}

std::vector<TownID> Datastructures::towns_distance_increasing_from(int x, int y)
{
    if(TownCount == 0){
        return {};
    }
    std::vector<TownID> temp;
    temp.reserve(TownCount);
    int dist;

    // Get an iterator pointing to begining of map
    auto it = Towns.begin();

    // Iterate over the map using iterator
    while(it != Towns.end())
    {
        dist = std::abs(it->second->x - x) + std::abs(it->second->y - y);
        it->second->distanceFrom = dist;
        temp.push_back(it->first);
        it++;
    }

    std::sort(temp.begin(), temp.end(), [this](TownID a, TownID b){
        return Towns[a]->distanceFrom < Towns[b]->distanceFrom;
    });

    return temp;
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    auto vassal = Towns.find(vassalid);
    auto master = Towns.find(masterid);
    if(vassal == Towns.end() || master == Towns.end() || vassal->second->master != NO_ID){
        return false;
    }
    vassal->second->master = masterid;
    master->second->vassals.push_back(vassalid);

    return true;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    auto town = Towns.find(id);
    if(town == Towns.end()){
        return {};
    }

    std::vector<TownID> taxPath = {id};
    TownID masterID = Towns[id]->master;

    while(masterID != NO_ID){
        taxPath.push_back(masterID);
        masterID = Towns[masterID]->master;
    }

    return taxPath;
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{

    auto town = Towns.find(id);
    if(town == Towns.end()){
        return {};
    }

    std::vector<TownID> vassalPath = maxDepth(id);

    return vassalPath;
}

int Datastructures::total_net_tax(TownID id)
{
    auto town = Towns.find(id);
    if(town == Towns.end()){
        return NO_VALUE;
    }

    std::vector<TownID> vassals = Towns[id]->vassals;
    TownID master = Towns[id]->master;
    int taxes = Towns[id]->tax;
    int numberOfVassals = vassals.size();

    if(numberOfVassals == 0 && master == NO_ID){
        return taxes;
    } else if(numberOfVassals == 0 && master != NO_ID){
        return (taxes-(taxes/10));
    } else{
        for(int i = 0; i < numberOfVassals; i++){
            int returnedTax = taxesOfValssal(vassals[i]);
            taxes += returnedTax;
        }
    }

    if(master == NO_ID){
        return taxes;
    } else{
        return (taxes-(taxes/10));
    }
}

int Datastructures::taxesOfValssal(TownID id)
{
    int taxes = Towns[id]->tax;
    std::vector<TownID> vassals = Towns[id]->vassals;
    int numberOfVassals = vassals.size();
    if(numberOfVassals == 0){
        return (taxes/10);
    }
    for(int i = 0; i < numberOfVassals; i++){
        int returnedTax = taxesOfValssal(vassals[i]);
        taxes += returnedTax;
    }
    return (taxes/10);
}

std::vector<TownID> Datastructures::maxDepth(TownID id)
{
    std::vector<TownID> vassalDepth = {id};
    std::vector<TownID> vassals = Towns[id]->vassals;
    if(vassals.size() == 0){
        return vassalDepth;
    }
    std::vector<TownID> test = {};
    int numberOfVassals = vassals.size();
    for(int i = 0; i < numberOfVassals; i++){
        std::vector<TownID> returnDepth = maxDepth(vassals[i]);
        if(returnDepth.size() > test.size()){
            test = returnDepth;
        }
    }

    vassalDepth.insert(vassalDepth.end(), test.begin(), test.end());

    return(vassalDepth);
}

void Datastructures::towns_alphabetically_with_no_return()
{
    if(addedToAplha == 0){
        return;
    }
    else {
        int sortedUntilIndex = TownCount - addedToAplha;
        if(sortedUntilIndex == 0){
            std::sort(alphabetical.begin(), alphabetical.end(), [this](TownID a, TownID b){
                return Towns[a]->name < Towns[b]->name;
            });
        }
        else{
            std::sort(alphabetical.begin() + sortedUntilIndex, alphabetical.end(), [this](TownID a, TownID b){
                return Towns[a]->name < Towns[b]->name;
            });
            std::inplace_merge(alphabetical.begin(), alphabetical.begin() + sortedUntilIndex, alphabetical.end());
        }
    }
    addedToAplha = 0;
    return;
}

void Datastructures::towns_distance_increasing_with_no_return()
{
    if(addedToDist == 0){
        return;
    }
    else {
        int sortedUntilIndex = TownCount - addedToDist;
        if(sortedUntilIndex == 0){
            std::sort(distance.begin(), distance.end(), [this](TownID a, TownID b){
                return Towns[a]->TownDistance < Towns[b]->TownDistance;
            });
        }
        else{

            std::sort(distance.begin() + sortedUntilIndex, distance.end(), [this](TownID a, TownID b){
                return Towns[a]->TownDistance < Towns[b]->TownDistance;
            });
            std::inplace_merge(distance.begin(), distance.begin() + sortedUntilIndex, distance.end());
        }
    }
    addedToDist = 0;
    return;
}
