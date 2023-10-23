#pragma once
#include "Entity.h"

namespace XuanWu
{
    class ScriptableEntity
    {
    public:
        friend class Scene;
        virtual ~ScriptableEntity() = default;
        template<typename T>
        T& GetComponent()
        {
            return m_Entity.GetComponent<T>();
        }
    protected:
        virtual void OnCreate() {}
        virtual void OnUpdate(Timestep ts) {}
        virtual void OnDestroy() {}
    private:
        Entity m_Entity;
    };
}