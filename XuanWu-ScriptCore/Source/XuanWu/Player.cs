using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using XuanWu;

public class Player : Entity
{
    public Player()
    {
        Console.WriteLine($"Contructor");
    }

    void OnCreate() 
    {
        Console.WriteLine($"Player.OnCreate -UUID: {ID}");
    }

    void OnUpdate(float ts)
    {
        Console.WriteLine($"OnUpdate- {ts}");

        float speed = 10.0f;
        Vector3 velocity = Vector3.Zero;

        if (Input.IsKeyDown(KeyCode.W))
            velocity.Y = 1.0f;
        else if (Input.IsKeyDown(KeyCode.S))
            velocity.Y = -1.0f;
        if (Input.IsKeyDown(KeyCode.A))
            velocity.X = -1.0f;
        else if (Input.IsKeyDown(KeyCode.D))
            velocity.X = 1.0f;

        velocity *= speed;
        Vector3 translation = Translation;
        translation += velocity * ts;
        Translation = translation;
    }
}
