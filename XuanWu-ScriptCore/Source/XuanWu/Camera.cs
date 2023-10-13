using System;
using XuanWu;

public class Camera : Entity
{
    private TransformComponent m_Transform;

    void OnCreate() 
    {
        m_Transform = GetComponent<TransformComponent>();
    }

    void OnUpdate(float ts)
    {
        float speed = 10.0f;
        Vector3 velocity = Vector3.Zero;

        if (Input.IsKeyDown(KeyCode.Up))
            velocity.Y = 1.0f;
        else if (Input.IsKeyDown(KeyCode.Down))
            velocity.Y = -1.0f;
        if (Input.IsKeyDown(KeyCode.Left))
            velocity.X = -1.0f;
        else if (Input.IsKeyDown(KeyCode.Right))
            velocity.X = 1.0f;

        velocity *= speed;

        Vector3 translation = m_Transform.Translation;
        translation += velocity * ts;
        m_Transform.Translation = translation;
    }
}
