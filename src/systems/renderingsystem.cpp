#include "renderingsystem.h"

#include <components/position.h>
#include <components/rendering.h>

#include <core/message.h>
#include <core/server.h>

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace arrakis::systems;

RenderingSystem::RenderingSystem(arrakis::core::Server & server) :
    m_server(server)
{

}

void RenderingSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    using namespace arrakis::components;
    using namespace rapidjson;

    StringBuffer sb;
    Writer<StringBuffer> writer(sb);

    entities.each<Position, Rendering>([dt, &writer, &sb, this](entityx::Entity entity, Position & position, Rendering & rendering)
    {
        if (!rendering.enabled)
        {
            return;
        }

        writer.StartObject();
        writer.Key("id", 2); writer.String(std::to_string(entity.id().id()).c_str());
        writer.Key("position_x", 10); writer.Int(position.x);
        writer.Key("position_y", 10); writer.Int(position.y);
        writer.EndObject();
        m_server.sendMessage({core::MessageType::Output, sb.GetString()});
    });
}
