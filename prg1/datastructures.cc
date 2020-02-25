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

void Datastructures::clear_all() //No efficiency test
{
    m_container.clear();
}

std::vector<StopID> Datastructures::all_stops() //No efficiency test
{
    if(m_container.empty()){
        return {NO_STOP};
    } else {
        std::vector<StopID> tempContainer = {};
        for(auto i : m_container){
            tempContainer.push_back(i.first);
        }
        return tempContainer;
    }
}

bool Datastructures::add_stop(StopID id, const Name& name, Coord xy)
{
    if(m_container.count(id)){
        return false;
    } else {
        auto sharedRegionStructure = std::make_shared<RegionStructure>();
        StopStructure ss({name, xy, sharedRegionStructure});
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
        for(std::pair<StopID, StopStructure> i : stopsAlphabetically){
            stopsAlphabeticallyInVector.push_back(i.first);
        }
        return stopsAlphabeticallyInVector;
    }
}

std::vector<StopID> Datastructures::stops_coord_order() //Should I initialize new Coords for items??
{
    if(m_container.empty()){
        return {NO_STOP};
    } else {
        Comparator comp = [this](std::pair<StopID, StopStructure> itemOne, std::pair<StopID, StopStructure> itemTwo){
            double itemOneDistance = pythagorasCalc(itemOne.second.coordinate);
            double itemTwoDistance = pythagorasCalc(itemTwo.second.coordinate);
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
        };
        std::set<std::pair<StopID, StopStructure>, Comparator> stopsAlphabetically(m_container.begin(), m_container.end(), comp);
        std::vector<StopID> stopsByCoordInVector = {};
        for(std::pair<StopID, StopStructure> i : stopsAlphabetically){
            stopsByCoordInVector.push_back(i.first);
        }
        return stopsByCoordInVector;
    }
}

StopID Datastructures::min_coord()
{
    if(m_container.empty()){
        return NO_STOP;
    } else {
        Comparator comp = [this](std::pair<StopID, StopStructure> itemOne, std::pair<StopID, StopStructure> itemTwo){
            double itemOneDistance = pythagorasCalc(itemOne.second.coordinate);
            double itemTwoDistance = pythagorasCalc(itemTwo.second.coordinate);
            if(itemOneDistance < itemTwoDistance)return true;
            else if(itemOneDistance > itemTwoDistance) return false;
            else return false;
        };
        std::pair<StopID, StopStructure> stopWithMinCoord = *min_element(m_container.begin(), m_container.end(), comp);
        return stopWithMinCoord.first;
    }

}

StopID Datastructures::max_coord()
{
    if(m_container.empty()){
        return NO_STOP;
    } else {
        Comparator comp = [this](std::pair<StopID, StopStructure> itemOne, std::pair<StopID, StopStructure> itemTwo){
            double itemOneDistance = pythagorasCalc(itemOne.second.coordinate);
            double itemTwoDistance = pythagorasCalc(itemTwo.second.coordinate);
            if(itemOneDistance > itemTwoDistance)return true;
            else if(itemOneDistance < itemTwoDistance) return false;
            else return false;
        };
        std::pair<StopID, StopStructure> stopWithMaxCoord = *min_element(m_container.begin(), m_container.end(), comp);
        return stopWithMaxCoord.first;
    }
}

std::vector<StopID> Datastructures::find_stops(Name const& name)
{
    if(!m_container.empty()){
        std::vector<StopID> stopsByName = {};
        for(auto item : m_container){
            if(item.second.name == name){
                stopsByName.push_back(item.first);
            } else {
                continue;
            }
        }
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
    // Replace this comment and the line below with your implementation
    return false;
}

Name Datastructures::get_region_name(RegionID id)
{
    // Replace this comment and the line below with your implementation
    return NO_NAME;
}

std::vector<RegionID> Datastructures::all_regions()
{
    // Replace this comment and the line below with your implementation
    return {NO_REGION};
}

bool Datastructures::add_stop_to_region(StopID id, RegionID parentid)
{
    // Replace this comment and the line below with your implementation
    return false;
}

bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    // Replace this comment and the line below with your implementation
    return false;
}

std::vector<RegionID> Datastructures::stop_regions(StopID id)
{
    // Replace this comment and the line below with your implementation
    return {NO_REGION};
}

void Datastructures::creation_finished()
{
    // Replace this comment with your implementation
    // You don't have to use this method for anything, if you don't need it
}

std::pair<Coord,Coord> Datastructures::region_bounding_box(RegionID id)
{
    // Replace this comment and the line below with your implementation
    return {NO_COORD, NO_COORD};
}

std::vector<StopID> Datastructures::stops_closest_to(StopID id)
{
    // Replace this comment and the line below with your implementation
    return {NO_STOP};
}

bool Datastructures::remove_stop(StopID id)
{
    // Replace this comment and the line below with your implementation
    return false;
}

RegionID Datastructures::stops_common_region(StopID id1, StopID id2)
{
    // Replace this comment and the line below with your implementation
    return NO_REGION;
}

double Datastructures::pythagorasCalc(Coord coord)
{
    return sqrt(pow(coord.x, 2) + pow(coord.y,2));
}
