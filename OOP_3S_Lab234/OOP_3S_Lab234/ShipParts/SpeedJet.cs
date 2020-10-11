using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System;
using System.Collections.Generic;
using System.Text;

namespace OOP_3S_Lab234.ShipParts
{
    public class SpeedJet : IJet
    {
        public float Speed { get; set; }
        public Texture2D Texture { get; set; }
        public SpeedJet()
        {
            Speed = 200f;
        }
        void IJet.Boost()
        {
        }
    }
}
