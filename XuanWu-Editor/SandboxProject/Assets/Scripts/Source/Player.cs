using System;
using XuanWu;

public class Player : Entity
{
    private Rigidbody2DComponent m_Rigidbody2D;

    public float speed = 20.0f;

    public Player()
    {
    }

    void OnCreate() 
    {
        m_Rigidbody2D = GetComponent<Rigidbody2DComponent>();
    }

    void OnUpdate(float ts)
    {
        Vector3 velocity = Vector3.Zero;

        if (Input.IsKeyDown(KeyCode.W))
            velocity.Y = 1.0f;
        else if (Input.IsKeyDown(KeyCode.S))
            velocity.Y = -1.0f;
        if (Input.IsKeyDown(KeyCode.A))
            velocity.X = -1.0f;
        else if (Input.IsKeyDown(KeyCode.D))
            velocity.X = 1.0f;

        velocity *= speed * ts;

        if(m_Rigidbody2D != null)
            m_Rigidbody2D.ApplyLinearImpulseToCenter(velocity.XY);
    }
}
