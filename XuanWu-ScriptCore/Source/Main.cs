using System;
using System.Runtime.CompilerServices;

namespace XuanWu
{
    public struct Vector3
    {
        public float X { get; set; }
        public float Y { get; set; }
        public float Z { get; set; }

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

    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string text, int parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog_Vector(ref Vector3 parameter, out Vector3 result);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float NativeLog_VectorDot(ref Vector3 parameter);
    }

    public class Main
    {
        public float FloatVar {get; set;}
        public Main()
        {
            Console.WriteLine("Main constructor!");

            InternalCalls.NativeLog("Hello C++!!!", 666);
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello World from C#!");
        }
        public void PrintInt(int value)
        {
            Console.WriteLine($"C# says: {value}");
        }
        public void PrintInts(int value1, int value2)
        {
            Console.WriteLine($"C# says: {value1} and {value2}");
        }
        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# says: {message}");
        }
        private void Log(string text, int parameter)
        {
            InternalCalls.NativeLog(text, parameter);
        }
    }
}