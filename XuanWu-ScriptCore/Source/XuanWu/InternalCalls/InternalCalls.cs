using System;
using System.Runtime.CompilerServices;

namespace XuanWu
{
    public static class InternalCalls
    {
        #region NativeLog
                [MethodImplAttribute(MethodImplOptions.InternalCall)]
                internal extern static void NativeLog(string text, int parameter);

                [MethodImplAttribute(MethodImplOptions.InternalCall)]
                internal extern static void NativeLog_Vector(ref Vector3 parameter, out Vector3 result);

                [MethodImplAttribute(MethodImplOptions.InternalCall)]
                internal extern static float NativeLog_VectorDot(ref Vector3 parameter);
        #endregion

        #region Entity
                [MethodImplAttribute(MethodImplOptions.InternalCall)]
                internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);
        #endregion

        #region TransformComponent
                [MethodImplAttribute(MethodImplOptions.InternalCall)]
                internal extern static void TransformComponent_GetTranslation(ulong entityID, out Vector3 translation);

                [MethodImplAttribute(MethodImplOptions.InternalCall)]
                internal extern static void TransformComponent_SetTranslation(ulong entityID, ref Vector3 translation);
        #endregion

        #region Rigidbody2DComponent
                [MethodImplAttribute(MethodImplOptions.InternalCall)]
                internal extern static void Rigidbody2DComponent_ApplyLinearImpulse(ulong entityID, ref Vector2 impluse, ref Vector2 worldPoint, bool wake);

                [MethodImplAttribute(MethodImplOptions.InternalCall)]
                internal extern static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(ulong entityID, ref Vector2 implues, bool wake);
        #endregion

        #region Input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keyCode);
        #endregion
    }
}