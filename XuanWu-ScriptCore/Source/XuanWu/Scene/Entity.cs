using System;

namespace XuanWu
{
    public class Entity
    {
        public readonly ulong ID;
        protected Entity() 
        { 
            ID = 0;
        }

        internal Entity(ulong id) 
        {
            ID = id;
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(ID, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if(!HasComponent<T>())
                return null;

            T component = new T() { entity= this };
            return component;
        }
    }
}
