#include <iostream>
#include <unordered_map>
#include <set>

#include "components_allocator.h"
#include "traits.h"

template<typename EntityIDT>
requires(std::is_integral_v<EntityIDT>)
struct entity
{
    EntityIDT entity_id = -1;
    std::set<component_type_id_t> components;
};

template<typename EntityIDT>
requires(std::is_integral_v<EntityIDT>)
struct basic_registry;

template<typename EntityIDT, typename... ComponentsT>
requires(std::is_integral_v<EntityIDT>)
struct basic_view
{
    friend struct basic_registry<EntityIDT>;

    template<typename FuncT>
    void each(FuncT&& func)
    {
        for(size_t i = 0; i < std::numeric_limits<size_t>::max(); ++i)
        {
            bool breakFor = false;
            for_types<ComponentsT...>([&](auto typeHolder) {
                using component_t = typename decltype(typeHolder)::type;

                if(allocators[get_type_id<component_t>()]->size() >= i)
                {
                    breakFor = true;
                }
            });

            if(breakFor) return;

            func((*(ComponentsT*) ((*allocators[get_type_id<ComponentsT>()])[i]->component_mem))...);
        }
    }

private:
    std::unordered_map<component_type_id_t, components_allocator<EntityIDT>*> allocators;
};

template<typename EntityIDT>
requires(std::is_integral_v<EntityIDT>)
struct basic_registry
{
    template<typename... ComponentsT>
    auto view()
    {
        basic_view<EntityIDT, ComponentsT...> view;
        view.allocators = {
                {get_type_id<ComponentsT>(), allocators[get_type_id<ComponentsT>()] }
                ...
        };

        return view;
    }

    entity_t create() noexcept
    {
        return ++max_entity;
    }

    template<typename ComponentT, typename... Args>
    ComponentT& emplace(entity_t forEntity, Args&&... args)
    {
        auto* allocator = allocators[get_type_id<ComponentT>()];
        basic_component_holder<EntityIDT>* holder =
                allocator->template allocate<ComponentT, Args...>(forEntity, std::forward<Args>(args)...);
        return (*(ComponentT*) holder->component_mem);
    }

    template<typename ComponentT>
    void remove(entity_t forEntity)
    {
        auto* allocator = allocators[get_type_id<ComponentT>()];
        allocator->template deallocate<ComponentT>(forEntity);
    }

private:
    std::unordered_map<std::uint32_t, components_allocator<EntityIDT>*> allocators;
    std::vector<entity<EntityIDT>> entities;
    entity_t max_entity = 0;
};

using registry = basic_registry<entity_t>;
using view = basic_view<entity_t >;

struct position
{
    float x = 0;
    float y = 0;
    float z = 0;
};

struct rotation
{
    float x = 0;
    float y = 0;
    float z = 0;
};

int main()
{
    registry reg;
    entity_t e0 = reg.create();
    auto& pos = reg.emplace<position>(e0);
    auto& rot = reg.emplace<rotation>(e0);

    auto v = reg.view<position, rotation>();
    v.each([](const position& p, const rotation& r) {
        // do smth
    });

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
