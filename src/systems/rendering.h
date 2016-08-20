#ifndef RENDERINGSYSTEM_H
#define RENDERINGSYSTEM_H

#include <entityx/System.h>

#include <rapidjson/document.h>

#include <components/playercontrolled.h>

#include <string>
#include <unordered_map>

namespace arrakis
{

namespace systems
{

class Networking;

// The Rendering system calculates what should the output clients see and
// sends that to them through the Networking system.
class Rendering : public entityx::System<Rendering>
{
public:
    Rendering(Networking & networking_system);

    void update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt) override;

protected:
    void drawPlayers(rapidjson::Value & objects_array, rapidjson::Document::AllocatorType & allocator, entityx::EntityManager & entities) const;

    Networking & networking_system;

    // We need this to store enum classes as unordered_map keys.
    struct EnumClassHash
    {
        template <typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };
    const std::unordered_map<components::PlayerControlled::Status, std::string, EnumClassHash> status_text
    {
        { components::PlayerControlled::Status::IDLE,    "IDLE" },
        { components::PlayerControlled::Status::WALKING, "WALKING" },
        { components::PlayerControlled::Status::DUCKING, "DUCKING" },
        { components::PlayerControlled::Status::AIRBORN, "AIRBORN" },
    };
    const std::unordered_map<components::PlayerControlled::Direction, std::string, EnumClassHash> direction_text
    {
        { components::PlayerControlled::Direction::NONE, "NONE" },
        { components::PlayerControlled::Direction::N,    "N" },
        { components::PlayerControlled::Direction::NE,   "NE" },
        { components::PlayerControlled::Direction::E,    "E" },
        { components::PlayerControlled::Direction::SE,   "SE" },
        { components::PlayerControlled::Direction::S,    "S" },
        { components::PlayerControlled::Direction::SW,   "SW" },
        { components::PlayerControlled::Direction::W,    "W" },
        { components::PlayerControlled::Direction::NW,   "NW" },
    };
};

} // end systems
} // end arrakis

#endif // RENDERINGSYSTEM_H
