#pragma once
#include "Entity.h"

namespace XuanWu
{
    class ScriptableEntity
    {
    public:
        friend class Scene;

        template<typename T>
        T& GetComponent()
        {
            return m_Entity.GetComponent<T>();
        }
    private:
        Entity m_Entity;
    };
}