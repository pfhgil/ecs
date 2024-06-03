//
// Created by Ilya on 03.06.2024.
//

#ifndef ECS_COMPONENTS_ALLOCATOR_H
#define ECS_COMPONENTS_ALLOCATOR_H

#include "traits.h"

template<typename EntityIDT>
requires(std::is_integral_v<EntityIDT>)
struct basic_component_holder
{
    EntityIDT entity_id = -1;
    component_type_id_t component_type_id = -1;
    size_t component_place_id = -1;
    void* component_mem = nullptr;
};

using component_holder = basic_component_holder<entity_t>;

template<typename EntityIDT>
requires(std::is_integral_v<EntityIDT>)
struct components_allocator
{
    // do your pool allocator logic

    template<typename ComponentT, typename... Args>
    basic_component_holder<EntityIDT>* allocate(entity_t forEntity, Args&&... args) noexcept
    {
        auto* holder = new basic_component_holder<EntityIDT>();
        holder->component_mem = new ComponentT(std::forward<Args>(args)...);
        holder->entity_id = forEntity;
        holder->component_type_id = get_type_id<ComponentT>();
        holder->component_place_id = buffer.size();

        buffer.push_back(holder);

        // do allocate logic and push new component to entity
        return holder;
    }

    template<typename ComponentT>
    basic_component_holder<EntityIDT>* deallocate(entity_t forEntity) noexcept
    {
        // do deallocate logic
        // make sure you notify the forEntity, that component is no more exist
        return nullptr;
    }

    basic_component_holder<EntityIDT>* get(size_t idx) noexcept
    {
        // do your indexing logic
        return reinterpret_cast<basic_component_holder<EntityIDT>*>(buffer[idx]);
    }

    [[nodiscard]] size_t size() const noexcept
    {
        return buffer.size();
    }

private:
    std::vector<basic_component_holder<EntityIDT>*> buffer;
    // void* buffer = nullptr;
};

#endif //ECS_COMPONENTS_ALLOCATOR_H
