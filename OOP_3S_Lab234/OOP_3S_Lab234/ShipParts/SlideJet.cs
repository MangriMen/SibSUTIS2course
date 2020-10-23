using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System;
using System.Collections.Generic;
using System.Text;

namespace OOP_3S_Lab234.ShipParts
{

    public class SlideJet : IJet
    {
        public string TypeOfJet { get; set; }
        public float Speed { get; set; }
        public Texture2D Texture { get; set; }
        public Texture2D Particles { get; set; }
        public SlideJet()
        {
            Speed = 200f;
        }
        void IJet.Boost()
        {
        }
    }
}
