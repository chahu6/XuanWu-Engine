using System;

namespace XuanWu
{
    public struct Vector3
    {
        public float X { get; set; }
        public float Y { get; set; }
        public float Z { get; set; }

        public Vector2 XY
        {
            get
            {
                return new Vector2(X, Y);
            }
        }

        public static Vector3 Zero => new Vector3(0.0f);

        public  Vector3(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
        }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }

        public static Vector3 operator -(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        }

        public static Vector3 operator *(Vector3 v, float scalar)
        {
            return new Vector3(v.X * scalar, v.Y * scalar, v.Z * scalar);
        }

        public static Vector3 operator /(Vector3 v, float scalar)
        {
            if (scalar == 0)
                throw new DivideByZeroException("Vector division by zero");

            return new Vector3(v.X / scalar, v.Y / scalar, v.Z / scalar);
        }

        public override string ToString()
        {
            return $"({X}, {Y}, {Z})";
        }
    }

    public struct Vector2
    {

        public float X { get; set; }
        public float Y { get; set; }

        public static Vector2 Zero => new Vector2(0.0f);

        public Vector2(float scalar)
        {
            X = scalar;
            Y = scalar;
        }

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public static Vector2 operator +(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X + b.X, a.Y + b.Y);
        }

        public static Vector2 operator -(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X - b.X, a.Y - b.Y);
        }

        public static Vector2 operator *(Vector2 v, float scalar)
        {
            return new Vector2(v.X * scalar, v.Y * scalar);
        }

        public static Vector2 operator /(Vector2 v, float scalar)
        {
            if (scalar == 0)
                throw new DivideByZeroException("Vector division by zero");

            return new Vector2(v.X / scalar, v.Y / scalar);
        }

        public override string ToString()
        {
            return $"({X}, {Y})";
        }
    }
}
