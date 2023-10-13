namespace XuanWu
{
    public abstract class Component
    {
        public Entity entity { get; set; }
    }

    public class TransformComponent : Component
    {
        public Vector3 Translation
        { 
            get
            {
                InternalCalls.TransformComponent_GetTranslation(entity.ID, out Vector3 outTranslation);
                return outTranslation;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(entity.ID, ref value);
            }
        }
    }

    public class Rigidbody2DComponent : Component
    {
        public void ApplyLinearImpulse(Vector2 impluse, Vector2 worldPoint, bool wake = true)
        {
            InternalCalls.Rigidbody2DComponent_ApplyLinearImpulse(entity.ID, ref impluse, ref worldPoint, wake);
        }

        public void ApplyLinearImpulseToCenter(Vector2 impluse, bool wake = true)
        {
            InternalCalls.Rigidbody2DComponent_ApplyLinearImpulseToCenter(entity.ID, ref impluse, wake);
        }
    }
}
