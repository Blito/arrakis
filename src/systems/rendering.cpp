#include "rendering.h"

#include <components/playercontrolled.h>
#include <components/position.h>
#include <components/rendering.h>

#include <core/message.h>
#include <systems/networking.h>

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace arrakis::systems;

Rendering::Rendering(Networking & networking_system) :
    networking_system(networking_system)
{

}

void Rendering::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    using namespace rapidjson;

    Document frame;
    frame.SetObject();

    // Create array with objects with a Rendering component
    Value objects_array(kArrayType);
    auto& allocator = frame.GetAllocator();
    drawPlayers(objects_array, allocator, entities);

    // Add array to output JSON
    frame.AddMember("to_render", objects_array, allocator);

    StringBuffer sb;
    Writer<StringBuffer> writer(sb);
    frame.Accept(writer);

    // Send JSON to output clients
    networking_system.send_message({core::MessageType::Output, sb.GetString()});
}

void Rendering::drawPlayers(rapidjson::Value & objects_array, rapidjson::Document::AllocatorType & allocator, entityx::EntityManager & entities) const
{
    using namespace arrakis::components;
    using namespace rapidjson;

    const std::unordered_map<components::PlayerControlled::Status, std::string, EnumClassHash> & status = this->status_text;
    const std::unordered_map<components::PlayerControlled::Direction, std::string, EnumClassHash> & direction = this->direction_text;

    entities.each<Position, PlayerControlled, components::Rendering>(
    [&objects_array, &allocator, &status, &direction](entityx::Entity entity, Position & position, PlayerControlled & player, components::Rendering & rendering)
    {
        if (!rendering.enabled)
        {
            return;
        }

        Value object; // [id, x, y]
        object.SetObject();
        object.AddMember("id", player.controlled_by, allocator);

        object.AddMember("x", position.x, allocator);
        object.AddMember("y", position.y, allocator);

        rapidjson::Document::StringRefType status_value(status.at(player.status).c_str());
        object.AddMember("status", status_value, allocator);

        rapidjson::Document::StringRefType direction_value(direction.at(player.aim_direction).c_str());
        object.AddMember("aiming-to", direction_value, allocator);

        objects_array.PushBack(object, allocator);
    });
}
