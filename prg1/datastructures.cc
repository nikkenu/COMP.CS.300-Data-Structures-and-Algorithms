// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <set>
#include <cmath>
#include <math.h>
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

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{

}

Datastructures::~Datastructures()
{
    // Replace this comment with your implementation
}

int Datastructures::stop_count()
{
    int size = m_container.size();
    if(size != 1){
        return size;
    } else {
        return NO_VALUE;
    }
}

void Datastructures::clear_all()
{
    m_container.clear();
    m_regionContainer.clear();
}

std::vector<StopID> Datastructures::all_stops()
{
    if(m_container.empty()){
        return {NO_STOP};
    } else {
        std::vector<StopID> tempContainer = {};
        std::transform(m_container.begin(), m_container.end(), std::back_inserter(tempContainer), [](std::pair<StopID, StopStructure> item){
           return item.first;
        });
        return tempContainer;
    }
}

bool Datastructures::add_stop(StopID id, const Name& name, Coord xy)
{
    if(m_container.find(id) != m_container.end()){
        return false;
    } else {
        StopStructure ss({name, xy});
        m_container.insert(std::make_pair(id, ss));
        return true;
    }
}

Name Datastructures::get_stop_name(StopID id)
{
    auto search = m_container.find(id);
    if(search != m_container.end()){
        return search->second.name;
    } else {
        return NO_NAME;
    }
}

Coord Datastructures::get_stop_coord(StopID id)
{
    auto search = m_container.find(id);
    if(search != m_container.end()){
        return search->second.coordinate;
    } else {
        return NO_COORD;
    }
}

std::vector<StopID> Datastructures::stops_alphabetically()
{
    if(m_container.empty()){
        return {NO_STOP};
    } else {
        Comparator comp = [](std::pair<StopID, StopStructure> itemOne, std::pair<StopID, StopStructure> itemTwo){
            return itemOne.second.name < itemTwo.second.name;
        };
        std::set<std::pair<StopID, StopStructure>, Comparator> stopsAlphabetically(m_container.begin(), m_container.end(), comp);
        std::vector<StopID> stopsAlphabeticallyInVector = {};
        std::for_each(stopsAlphabetically.begin(), stopsAlphabetically.end(), [&stopsAlphabeticallyInVector](std::pair<StopID, StopStructure> item){
           stopsAlphabeticallyInVector.push_back(item.first);
        });
        return stopsAlphabeticallyInVector;
    }
}

std::vector<StopID> Datastructures::stops_coord_order()
{
    if(m_container.empty()){
        return {NO_STOP};
    } else {
        std::set<std::pair<StopID, StopStructure>, CompareDistance> stopsInDistanceOrder(m_container.begin(), m_container.end(), CompareDistance());
        std::vector<StopID> stopsByCoordInVector = {};
        std::for_each(stopsInDistanceOrder.begin(), stopsInDistanceOrder.end(), [&stopsByCoordInVector](std::pair<StopID, StopStructure> item){
           stopsByCoordInVector.push_back(item.first);
        });
        return stopsByCoordInVector;
    }
}

StopID Datastructures::min_coord()
{
    if(m_container.empty()){
        return NO_STOP;
    } else {
        std::pair<StopID, StopStructure> stopWithMinCoord = *min_element(m_container.begin(), m_container.end(), CompareDistance());
        return stopWithMinCoord.first;
    }

}

StopID Datastructures::max_coord()
{
    if(m_container.empty()){
        return NO_STOP;
    } else {
        std::pair<StopID, StopStructure> stopWithMaxCoord = *max_element(m_container.begin(), m_container.end(), CompareDistance());
        return stopWithMaxCoord.first;
    }
}

std::vector<StopID> Datastructures::find_stops(Name const& name)
{
    if(!m_container.empty()){
        std::vector<StopID> stopsByName = {};
        std::for_each(m_container.begin(), m_container.end(), [&name, &stopsByName](const std::pair<StopID, StopStructure> item){
           if(item.second.name == name){
               stopsByName.push_back(item.first);
           }
        });
        if(!stopsByName.empty()){
            return stopsByName;
        }
    }
    return {NO_STOP};
}

bool Datastructures::change_stop_name(StopID id, const Name& newname)
{
    auto it = m_container.find(id);
    if(it != m_container.end()){
        it->second.name = newname;
        return true;
    } else {
        return false;
    }
}

bool Datastructures::change_stop_coord(StopID id, Coord newcoord)
{
    auto it = m_container.find(id);
    if(it != m_container.end()){
        it->second.coordinate = newcoord;
        return true;
    } else {
        return false;
    }
}

bool Datastructures::add_region(RegionID id, const Name &name)
{
    if(m_regionContainer.find(id) != m_regionContainer.end()){
        return false;
    } else {
        RegionStructure rs({name});
        m_regionContainer.insert(std::make_pair(id, rs));
        return true;
    }

}

Name Datastructures::get_region_name(RegionID id)
{
    auto search = m_regionContainer.find(id);
    if(search != m_regionContainer.end()){
        return search->second.name;
    } else {
        return NO_NAME;
    }
}

std::vector<RegionID> Datastructures::all_regions()
{
    if(m_regionContainer.empty()){
        return {NO_REGION};
    } else {
        std::vector<RegionID> tempVector = {};
        std::transform(m_regionContainer.begin(), m_regionContainer.end(), std::back_inserter(tempVector), [](std::pair<RegionID, RegionStructure> item){
           return item.first;
        });
        return tempVector;
    }
}

bool Datastructures::add_stop_to_region(StopID id, RegionID parentid)
{
    auto it = m_container.find(id);
    auto it2 = m_regionContainer.find(parentid);
    if(it != m_container.end() && it2 != m_regionContainer.end() && it->second.region == nullptr){
        it->second.region = std::make_shared<RegionID>(parentid);
        return true;
    }
    return false;
}

bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    auto it = m_regionContainer.find(parentid);
    if(it != m_regionContainer.end()){
        it->second.children.insert(std::make_shared<RegionID>(id));

        auto it2 = m_regionContainer.find(id);
        if(it2 != m_regionContainer.end()){
            it2->second.parent = std::make_shared<RegionID>(parentid);
        }
        return true;
    }
    return false;
}

std::vector<RegionID> Datastructures::stop_regions(StopID id)
{
    auto it = m_container.find(id);
    if(it != m_container.end()){
        if(it->second.region != nullptr){
           std::vector<RegionID> regionsInVector = {};
           RegionID stopsMainRegion = *it->second.region;
           regionsInVector.push_back(stopsMainRegion);
           auto it2 = m_regionContainer.find(stopsMainRegion);
           if(it2 != m_regionContainer.end()){
                if(it2->second.parent != nullptr){
                    regionsInVector.push_back(*it2->second.parent);
                }
                if(!it2->second.children.empty()){
                    std::transform(it2->second.children.begin(), it2->second.children.end(), std::back_inserter(regionsInVector), [](std::shared_ptr<RegionID> item){
                       return *item;
                    });
                }
           }
           return regionsInVector;
        }
    }
    return {NO_REGION};
}

void Datastructures::creation_finished()
{
    // Replace this comment with your implementation
    // You don't have to use this method for anything, if you don't need it
}

std::pair<Coord,Coord> Datastructures::region_bounding_box(RegionID id)
{
    Coord leftDownCorner = NO_COORD;
    Coord rightTopCorner = NO_COORD;
    std::vector<RegionID> regions = {id};
    auto it = m_regionContainer.find(id);
    if(it != m_regionContainer.end()){
        if(!it->second.children.empty()){
            std::for_each(it->second.children.begin(), it->second.children.end(), [&regions](std::shared_ptr<RegionID> item){
               regions.push_back(*item);
            });
        }
    }

    for(auto i : m_container){
        if(i.second.region != nullptr){
            if(std::count(regions.begin(), regions.end(), *i.second.region)){
                int x = i.second.coordinate.x;
                int y = i.second.coordinate.y;
                if(leftDownCorner == NO_COORD){
                    leftDownCorner = {x,y};
                } else {
                    if(leftDownCorner.x > x) leftDownCorner.x = x;
                    if(leftDownCorner.y > y) leftDownCorner.y = y;
                }

                if(rightTopCorner == NO_COORD){
                    rightTopCorner = {x,y};
                } else {
                    if(rightTopCorner.x < x) rightTopCorner.x = x;
                    if(rightTopCorner.y < y) rightTopCorner.y = y;
                }
            }
        }
    }
    return {leftDownCorner, rightTopCorner};
}

std::vector<StopID> Datastructures::stops_closest_to(StopID id)
{
    std::unordered_map<StopID, std::pair<Coord, double>> temp;
    auto it = m_container.find(id);
    if(it != m_container.end() && m_container.size() >= 6){
        Name idName = it->second.name;
        Coord idCoord = it->second.coordinate;
        std::for_each(m_container.begin(), m_container.end(), [this, &idName,&idCoord,&temp](std::pair<StopID, StopStructure> item){
           if(idName != item.second.name){
               std::pair<Coord, double> pairTemp = std::make_pair(item.second.coordinate, twoPointDistance(idCoord, item.second.coordinate));
               temp.insert(std::make_pair(item.first, pairTemp));
           }
        });

        std::set<std::pair<StopID, std::pair<Coord, double>>, CompareClosest> distanceShorted(temp.begin(), temp.end(), CompareClosest());
        std::vector<StopID> tempVector = {};
        std::for_each(distanceShorted.begin(), distanceShorted.end(), [&tempVector](std::pair<StopID, std::pair<Coord, double>> item){
           if(tempVector.size() < 5){
                tempVector.push_back(item.first);
           }
        });
        return tempVector;
    }
    return {NO_STOP};
}

bool Datastructures::remove_stop(StopID id)
{
    auto it = m_container.find(id);
    if(it != m_container.end()){
        m_container.erase(id);
        return true;
    }
    return false;
}

RegionID Datastructures::stops_common_region(StopID id1, StopID id2)
{
    auto it1 = m_container.find(id1);
    auto it2 = m_container.find(id2);
    if(it1 != m_container.end() && it2 != m_container.end() && it1->second.region != nullptr && it2->second.region != nullptr){
        RegionID commonRegion = NO_REGION;
        if(*it1->second.region == *it2->second.region){
            commonRegion = *it1->second.region;
        }

        std::set<RegionID> id1Regions = {};
        std::set<RegionID> id2Regions = {};
        auto it3 = m_regionContainer.find(*it1->second.region);
        auto it4 = m_regionContainer.find(*it2->second.region);

        std::for_each(it3->second.children.begin(), it3->second.children.begin(), [&id1Regions](std::shared_ptr<RegionID> item){
            id1Regions.insert(*item);
        });

        std::for_each(it4->second.children.begin(), it4->second.children.begin(), [&id2Regions](std::shared_ptr<RegionID> item){
            id2Regions.insert(*item);
        });

        for(auto i : id1Regions){
            auto k = id2Regions.find(i);
            if(k != id2Regions.end()){
                commonRegion = i;
            }
        }
        return commonRegion;

    }
    return NO_REGION;
}

double Datastructures::pythagorasCalc(Coord coord)
{
    return sqrt(pow(coord.x, 2) + pow(coord.y,2));
}

double Datastructures::twoPointDistance(Coord coord1, Coord coord2)
{
    return sqrt(pow(coord1.x - coord2.x,2) + pow(coord1.y - coord2.y,2));
}
