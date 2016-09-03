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

    // Create empty arrays for characters and arrows.
    Value players_array(kArrayType);
    Value arrows_array(kArrayType);

    auto& allocator = frame.GetAllocator();

    entities.each<components::Rendering>(
    [&players_array, &arrows_array, &allocator, this](entityx::Entity entity, components::Rendering & rendering)
    {
        if (rendering.enabled)
        {
            switch (rendering.tag)
            {
            case components::Rendering::Tag::PLAYER:
                drawPlayer(entity, players_array, allocator);
                break;
            case components::Rendering::Tag::ARROW:
                drawArrow(entity, arrows_array, allocator);
                break;
            }
        }
    });

    frame.AddMember("players", players_array, allocator);
    frame.AddMember("arrows", arrows_array, allocator);

    StringBuffer sb;
    Writer<StringBuffer> writer(sb);
    frame.Accept(writer);

    // Send JSON to output clients
    networking_system.send_message({core::MessageType::Output, sb.GetString()});
}

void Rendering::drawPlayer(entityx::Entity & entity, rapidjson::Value & players_array, rapidjson::Document::AllocatorType & allocator) const
{
    using namespace arrakis::components;
    using namespace rapidjson;

    const std::unordered_map<PlayerControlled::Status, std::string, EnumClassHash> & status = this->status_text;
    const std::unordered_map<PlayerControlled::Direction, std::string, EnumClassHash> & direction = this->direction_text;

    auto position = entity.component<Position>();
    auto player = entity.component<PlayerControlled>();

    Value object; // [id, x, y]
    object.SetObject();
    object.AddMember("id", player->controlled_by, allocator);

    object.AddMember("x", position->x, allocator);
    object.AddMember("y", position->y, allocator);

    Document::StringRefType status_value(status.at(player->status).c_str());
    object.AddMember("status", status_value, allocator);

    Document::StringRefType direction_value(direction.at(player->aim_direction).c_str());
    object.AddMember("aiming-to", direction_value, allocator);

    object.AddMember("ammo", player->ammo, allocator);

    players_array.PushBack(object, allocator);
}

void Rendering::drawArrow(entityx::Entity & entity, rapidjson::Value & arrows_array, rapidjson::Document::AllocatorType & allocator) const
{
    using namespace arrakis::components;
    using namespace rapidjson;

    auto position = entity.component<Position>();

    Value object; // [id, x, y]
    object.SetObject();

    object.AddMember("x", position->x, allocator);
    object.AddMember("y", position->y, allocator);

    arrows_array.PushBack(object, allocator);
}
