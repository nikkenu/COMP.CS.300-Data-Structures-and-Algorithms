// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <cmath>
#include <stdexcept>

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
    if(!m_container.empty()){
        return m_container.size();
    } else {
        return 0;
    }
}

void Datastructures::clear_all()
{
    m_container.clear();
    m_regionContainer.clear();
    m_sortedStopsByName.clear();
    m_sortedStopsByCoord.clear();
    m_isSortedStopsByName = false;
    m_isSortedStopsByCoord = false;

    // PRG2 IMPLEMETATION BELOW
    m_routeContainer.clear();
    m_graph.clear();
}

std::vector<StopID> Datastructures::all_stops()
{
    if(!m_container.empty()){
        std::vector<StopID> tempContainer = {};
        std::transform(m_container.begin(), m_container.end(), std::back_inserter(tempContainer), [](const std::pair<StopID, StopStructure>& item){
           return item.first;
        });
        return tempContainer;
    }
    return {};
}

bool Datastructures::add_stop(StopID id, const Name& name, Coord xy)
{
    if(m_container.find(id) != m_container.end()){
        return false;
    } else {
        StopStructure ss;
        ss.name = name;
        ss.coordinate = xy;
        m_container.insert({id, ss});
        m_sortedStopsByName.push_back({name,id});
        m_sortedStopsByCoord.push_back({xy,id});
        m_isSortedStopsByName = false;
        m_isSortedStopsByCoord = false;

        // PRG2 BELOW
        std::shared_ptr<Node> node = std::make_shared<Node>();
        node->stopID = id;
        node->coordinate = xy;
        m_graph.insert({id, node});
        return true;
    }
}

Name Datastructures::get_stop_name(StopID id)
{
    auto it = m_container.find(id);
    if(it != m_container.end()){
        return it->second.name;
    } else {
        return NO_NAME;
    }
}

Coord Datastructures::get_stop_coord(StopID id)
{
    auto it = m_container.find(id);
    if(it != m_container.end()){
        return it->second.coordinate;
    } else {
        return NO_COORD;
    }
}

std::vector<StopID> Datastructures::stops_alphabetically()
{
    if(m_container.empty()){
        return {};
    } else {
        if(!m_isSortedStopsByName){
            sortStopsByName();
        }
        std::vector<StopID> stopsAlphabeticallyInVector = {};
        std::for_each(m_sortedStopsByName.begin(),m_sortedStopsByName.end(), [&stopsAlphabeticallyInVector](const std::pair<Name, StopID>& item){
            stopsAlphabeticallyInVector.push_back(item.second);
        });
        return stopsAlphabeticallyInVector;
    }
}

std::vector<StopID> Datastructures::stops_coord_order()
{
    if(m_container.empty()){
        return {};
    } else {
        if(!m_isSortedStopsByCoord){
            sortStopsByCoord();
        }
        std::vector<StopID> stopsByCoordInVector = {};
        std::for_each(m_sortedStopsByCoord.begin(), m_sortedStopsByCoord.end(), [&stopsByCoordInVector](const std::pair<Coord,StopID>& item){
            stopsByCoordInVector.push_back(item.second);
        });

        return stopsByCoordInVector;
    }
}

StopID Datastructures::min_coord()
{
    if(m_container.empty()){
        return NO_STOP;
    } else {
        auto result = *std::min_element(m_sortedStopsByCoord.cbegin(), m_sortedStopsByCoord.cend(), [](const auto& lhs, const auto& rhs) {
                auto dist1 = sqrt(pow(lhs.first.x, 2) + pow(lhs.first.y,2));
                auto dist2 = sqrt(pow(rhs.first.x, 2) + pow(rhs.first.y,2));
                return dist1 < dist2;
            });
        return result.second;
    }

}

StopID Datastructures::max_coord()
{
    if(m_container.empty()){
        return NO_STOP;
    } else {
        auto result = *std::max_element(m_sortedStopsByCoord.cbegin(), m_sortedStopsByCoord.cend(), [](const auto& lhs, const auto& rhs) {
                auto dist1 = sqrt(pow(lhs.first.x, 2) + pow(lhs.first.y,2));
                auto dist2 = sqrt(pow(rhs.first.x, 2) + pow(rhs.first.y,2));
                return dist1 < dist2;
            });
        return result.second;
    }
}

std::vector<StopID> Datastructures::find_stops(Name const& name)
{
    if(!m_container.empty()){
        std::vector<StopID> stopsByName = {};

        if(!m_isSortedStopsByName){
            sortStopsByName();
        }

        auto it = std::find_if(m_sortedStopsByName.begin(), m_sortedStopsByName.end(), [&name](const std::pair<Name, StopID>& item){
           return item.first == name;
        });

        while(it != m_sortedStopsByName.end()){
            if(it->first == name){
                stopsByName.push_back(it->second);
                it++;
            } else {
                break;
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
        Name oldName = it->second.name;
        it->second.name = newname;
        std::pair<Name, StopID> temp(oldName, id);
        auto it2 = std::find(m_sortedStopsByName.begin(), m_sortedStopsByName.end(), temp);
        if(it2 != m_sortedStopsByName.end()){
            it2->first = newname;
            m_isSortedStopsByName = false;
        }
        return true;
    } else {
        return false;
    }
}

bool Datastructures::change_stop_coord(StopID id, Coord newcoord)
{
    auto it = m_container.find(id);
    if(it != m_container.end()){
        Coord oldCoord = it->second.coordinate;
        it->second.coordinate = newcoord;

        std::pair<Coord, StopID> temp(oldCoord, id);
        auto it2 = std::find(m_sortedStopsByCoord.begin(), m_sortedStopsByCoord.end(), temp);
        if(it2 != m_sortedStopsByCoord.end()){
            it2->first = newcoord;
            m_isSortedStopsByCoord = false;
        }

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
        RegionStructure rs;
        rs.name = name;
        m_regionContainer.insert({id, rs});
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
        return {};
    } else {
        std::vector<RegionID> tempVector = {};
        std::transform(m_regionContainer.begin(), m_regionContainer.end(), std::back_inserter(tempVector), [](const std::pair<RegionID, RegionStructure>& item){
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
    if(it != m_container.end() && it->second.region != nullptr){
        std::vector<RegionID> regions = {*it->second.region};
        auto regIt = m_regionContainer.find(*it->second.region);
        if(regIt->second.parent != nullptr){
            findRegionParents(*regIt->second.parent, regions);
        }
        return regions;
    }
    return {};
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
        findRegionChilds(it->first, regions);
    }

    std::for_each(m_container.begin(), m_container.end(), [&regions, &leftDownCorner, &rightTopCorner](const std::pair<StopID, StopStructure>& i){
        if(i.second.region != nullptr){
            auto it2 = std::find(regions.begin(), regions.end(), *i.second.region);
            if(it2 != regions.end()){
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
    });
    return {leftDownCorner, rightTopCorner};
}

std::vector<StopID> Datastructures::stops_closest_to(StopID id)
{
    std::unordered_map<StopID, std::pair<Coord, double>> temp;
    auto it = m_container.find(id);
    if(it != m_container.end()){
        Name idName = it->second.name;
        Coord idCoord = it->second.coordinate;
        std::for_each(m_container.begin(), m_container.end(), [this, &idName,&idCoord,&temp](const std::pair<StopID, StopStructure>& item){
           if(idName != item.second.name){
               std::pair<Coord, double> pairTemp = std::make_pair(item.second.coordinate, twoPointDistance(idCoord, item.second.coordinate));
               temp.insert(std::make_pair(item.first, pairTemp));
           }
        });

        std::set<std::pair<StopID, std::pair<Coord, double>>, CompareClosest> distanceShorted(temp.begin(), temp.end(), CompareClosest());
        std::vector<StopID> tempVector = {};
        std::for_each(distanceShorted.begin(), distanceShorted.end(), [&tempVector](const std::pair<StopID, std::pair<Coord, double>>& item){
           if(tempVector.size() < 5){
                tempVector.push_back(item.first);
           }
        });

        return tempVector;
    }
    return {};
}

bool Datastructures::remove_stop(StopID id)
{
    auto it = m_container.find(id);
    if(it != m_container.end()){
        m_container.erase(id);

        auto it2 = m_sortedStopsByName.begin();
        while (it2 != m_sortedStopsByName.end())
        {
            if (it2->second == id) it2 = m_sortedStopsByName.erase(it2);
            else it2++;
        }

        auto it3 = m_sortedStopsByCoord.begin();
        while (it3 != m_sortedStopsByCoord.end())
        {
            if (it3->second == id) it3 = m_sortedStopsByCoord.erase(it3);
            else it3++;
        }
        return true;
    }
    return false;
}

RegionID Datastructures::stops_common_region(StopID id1, StopID id2)
{
    auto it1 = m_container.find(id1);
    auto it2 = m_container.find(id2);
    if(it1 != m_container.end() && it1 != m_container.end()
            && it1->second.region != nullptr && it2->second.region != nullptr){
        std::vector<RegionID> id1Regions = {};
        std::vector<RegionID> id2Regions = {};
        findRegionParentsSecond(*it1->second.region, id1Regions);
        findRegionParentsSecond(*it2->second.region, id2Regions);
        RegionID commonReg = findCommonRegion(id1Regions, id2Regions);
        return commonReg;
    } else {
        return NO_REGION;
    }
}

double Datastructures::twoPointDistance(Coord coord1, Coord coord2)
{
    return sqrt(pow(coord1.x - coord2.x,2) + pow(coord1.y - coord2.y,2));
}

void Datastructures::findRegionParents(RegionID region, std::vector<RegionID> &regions)
{
    regions.push_back(region);
    auto it = m_regionContainer.find(region);
    if(it != m_regionContainer.end() && it->second.parent != nullptr){
        findRegionParents(*it->second.parent, regions);
    }
}

void Datastructures::findRegionParentsSecond(RegionID region, std::vector<RegionID> &regions)
{
    auto it = m_regionContainer.find(region);
    if(it != m_regionContainer.end() && it->second.parent != nullptr){
        findRegionParentsSecond(*it->second.parent, regions);
        regions.push_back(region);
    } else {
        regions.push_back(region);
    }
}

void Datastructures::findRegionChilds(RegionID region, std::vector<RegionID> &regions)
{
    auto it = m_regionContainer.find(region);
    if(it != m_regionContainer.end() && !it->second.children.empty()){
        auto childVec = it->second.children;
        for(auto region : childVec){
            regions.push_back(*region);
            findRegionChilds(*region, regions);
        }
    }
}

RegionID Datastructures::findCommonRegion(const std::vector<RegionID> &regions1, const std::vector<RegionID> &regions2)
{
    RegionID commonRegion = NO_REGION;
    std::vector<RegionID> result = {};
    std::set_intersection(regions1.begin(), regions1.end(), regions2.begin(), regions2.end(), std::back_inserter(result));

    if(!result.empty()){
        commonRegion = result.at(result.size()-1);
    }
    return commonRegion;
}

void Datastructures::sortStopsByCoord()
{
    std::random_shuffle(m_sortedStopsByCoord.begin(), m_sortedStopsByCoord.end());
    std::sort(m_sortedStopsByCoord.begin(), m_sortedStopsByCoord.end(), CompareDistance());
    m_isSortedStopsByCoord = true;
}

void Datastructures::sortStopsByName()
{
    std::random_shuffle(m_sortedStopsByName.begin(), m_sortedStopsByName.end());
    std::sort(m_sortedStopsByName.begin(), m_sortedStopsByName.end());
    m_isSortedStopsByName = true;
}

int Datastructures::getDistanceBetweenTwoStops(Coord source, Coord destination)
{
    double length = sqrt(pow(source.x - destination.x,2) + pow(source.y - destination.y,2));
    return floor(length);
}

bool Datastructures::journey_least_stops_helper(StopID source, StopID destination, std::unordered_map<StopID, Edge*> &pred)
{
    std::list<StopID> queue;
    std::unordered_map<StopID, bool> visited;
    std::unordered_map<StopID, int> dist;
    for(auto i : m_graph){
        visited.insert({i.first, false});
        dist.insert({i.first, INT_MAX});
        pred.insert({i.first, nullptr});
    }

    visited.at(source) = true;
    dist.at(source) = 0;
    queue.push_back(source);

    while(!queue.empty()){
        StopID stop = queue.front();
        queue.pop_front();
        auto it = m_graph.find(stop);
        if(it != m_graph.end()){
            for(auto &edge : it->second->edges){
                if(visited[edge->destination->stopID] == false){
                    visited.at(edge->destination->stopID) = true;
                    dist.at(edge->destination->stopID) = dist.at(stop) + 1;
                    pred.at(edge->destination->stopID) = edge.get();
                    queue.push_back(edge->destination->stopID);
                    if(edge->destination->stopID == destination){
                        return true;
                    }
                }
            }
        }
    }
    return false;

}

std::vector<RouteID> Datastructures::all_routes()
{
    if(!m_routeContainer.empty()){
        std::vector<RouteID> tempVector = {};
        std::for_each(m_routeContainer.begin(), m_routeContainer.end(), [&tempVector](const std::pair<RouteID, std::deque<RouteStopInfo>> &item){
           tempVector.push_back(item.first);
        });
        return tempVector;
    } else {
        return {NO_ROUTE};
    }
}

bool Datastructures::add_route(RouteID id, std::vector<StopID> stops)
{   
    auto it = m_routeContainer.find(id);
    if(it == m_routeContainer.end() && stops.size() > 1){
        std::deque<RouteStopInfo> tempContainer = {};
        std::for_each(stops.begin(), stops.end(), [&tempContainer](const StopID &item){
            RouteStopInfo tmp;
            tmp.stop = item;
            tempContainer.push_back(tmp);
        });

        for(unsigned int i = 0; i < tempContainer.size()-1; ++i){
            auto firstStop = m_graph.find(tempContainer.at(i).stop);
            auto secondStop = m_graph.find(tempContainer.at(i+1).stop);
            if(firstStop != m_graph.end() && secondStop != m_graph.end()){
                std::shared_ptr<Edge> edge = std::make_shared<Edge>();
                edge->source = firstStop->second;
                edge->destination = secondStop->second;
                edge->distance = getDistanceBetweenTwoStops(firstStop->second->coordinate, secondStop->second->coordinate);
                edge->route = id;
                firstStop->second->edges.push_back(edge);
            } else {
                return false;
            }
        }

        m_routeContainer.insert({id, tempContainer});
        return true;
    }

    return false;
}

std::vector<std::pair<RouteID, StopID>> Datastructures::routes_from(StopID stopid)
{
    auto it = m_graph.find(stopid);
    if(it != m_graph.end()){
        std::vector<std::pair<RouteID, StopID>> temp = {};
        std::for_each(it->second->edges.begin(), it->second->edges.end(), [&temp](const std::shared_ptr<Edge>& edge){
            temp.push_back({edge->route, edge->destination->stopID});
        });
        return temp;
    }

    return {{NO_ROUTE, NO_STOP}};
}

std::vector<StopID> Datastructures::route_stops(RouteID id)
{
    auto it = m_routeContainer.find(id);
    if(it != m_routeContainer.end()){
        std::vector<StopID> temp = {};
        std::for_each(it->second.begin(), it->second.end(), [&temp](const RouteStopInfo &item){
           temp.push_back(item.stop);
        });
        return temp;
    } else {
        return {NO_STOP};
    }
}

void Datastructures::clear_routes()
{
    m_routeContainer.clear();
    for(auto i : m_graph){
        i.second->edges.clear();
    }
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_any(StopID fromstop, StopID tostop)
{
    return journey_least_stops(fromstop, tostop);
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_least_stops(StopID fromstop, StopID tostop)
{
    std::unordered_map<StopID, Edge*> pred;
    if(journey_least_stops_helper(fromstop, tostop, pred)){

        std::vector<Edge*> path;
        StopID crawl = tostop;
        while(pred.at(crawl) != nullptr){
            path.push_back(pred.at(crawl));
            crawl = pred.at(crawl)->source->stopID;
        }

        std::reverse(path.begin(), path.end());

        std::vector<std::tuple<StopID, RouteID, Distance>> temp = {};
        Distance distance = 0;
        for(unsigned int i = 0; i < path.size(); i++){
            if(i == 0) temp.push_back({path.at(i)->source->stopID, path.at(i)->route, distance});
            else{
                distance = path.at(i-1)->distance + distance;
                temp.push_back({path.at(i)->source->stopID, path.at(i)->route, distance});
            }
        }
        distance = path.at(path.size()-1)->distance + distance;
        temp.push_back({path.at(path.size()-1)->destination->stopID, NO_ROUTE, distance});
        return temp;
    }

    return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_with_cycle(StopID fromstop)
{
    // Replace this comment and the line below with your implementation
    return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_shortest_distance(StopID fromstop, StopID tostop)
{
    // Replace this comment and the line below with your implementation
    return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
}

bool Datastructures::add_trip(RouteID routeid, std::vector<Time> const& stop_times)
{
    auto it = m_routeContainer.find(routeid);
    if(it != m_routeContainer.end()){
        for(unsigned int i = 0; i < it->second.size(); ++i){
                it->second.at(i).times.push_back(stop_times.at(i));
        }
        return true;
    }
    return false;

}

std::vector<std::pair<Time, Duration>> Datastructures::route_times_from(RouteID routeid, StopID stopid)
{
    auto it = m_routeContainer.find(routeid);
    if(it != m_routeContainer.end()){
        std::deque<Time> stopOneTimes;
        std::deque<Time> stopTwoTimes;
        for(unsigned int i = 0; i < it->second.size()-1; i++){
            if(it->second.at(i).stop == stopid){
                stopOneTimes = it->second.at(i).times;
                stopTwoTimes = it->second.at(i+1).times;
            }
        }
        if(!stopOneTimes.empty() && !stopTwoTimes.empty()){
            std::vector<std::pair<Time, Duration>> temp = {};
            for(unsigned int i = 0; i < stopOneTimes.size(); i++){
                Duration duration = stopTwoTimes.at(i) - stopOneTimes.at(i);
                temp.push_back({stopOneTimes.at(i), duration});
            }
            return temp;
        }
    }
    return {{NO_TIME, NO_DURATION}};
}

std::vector<std::tuple<StopID, RouteID, Time> > Datastructures::journey_earliest_arrival(StopID fromstop, StopID tostop, Time starttime)
{
    // Replace this comment and the line below with your implementation
    return {{NO_STOP, NO_ROUTE, NO_TIME}};
}

void Datastructures::add_walking_connections()
{
    // Replace this comment and the line below with your implementation
}
