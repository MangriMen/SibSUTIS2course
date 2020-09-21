using Microsoft.Xna.Framework.Input;
using System;
using System.Collections.Generic;
using System.Text;

namespace OOP_3S_Lab234
{
    namespace Utils
    {
        public static class Kb
        {
            public static bool getDown(Keys key)
            {
                return Keyboard.GetState().IsKeyDown(key);
            }
        }
    }
}
