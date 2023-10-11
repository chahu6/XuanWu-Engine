using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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

        public Vector3 Translation 
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(ID, out Vector3 result);
                return result;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(ID, ref value);
            }
        }
    }
}
