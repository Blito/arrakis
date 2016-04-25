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

    Document frame;
    frame.SetObject();

    // Create array with objects with a Rendering component
    Value objects_array(kArrayType);
    auto& allocator = frame.GetAllocator();
    entities.each<Position, Rendering>([dt, &objects_array, &allocator, this](entityx::Entity entity, Position & position, Rendering & rendering)
    {
        if (!rendering.enabled)
        {
            return;
        }

        Value object; // [id, x, y]
        object.SetObject();
        object.AddMember("id", entity.id().id(), allocator);
        object.AddMember("x", position.x, allocator);
        object.AddMember("y", position.y, allocator);

        objects_array.PushBack(object, allocator);
    });

    // Add array to output JSON
    frame.AddMember("to_render", objects_array, allocator);

    StringBuffer sb;
    Writer<StringBuffer> writer(sb);
    frame.Accept(writer);

    // Send JSON to output clients
    m_server.sendMessage({core::MessageType::Output, sb.GetString()});
}
