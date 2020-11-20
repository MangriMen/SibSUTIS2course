using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using OOP_3S_Lab234.Managers;
using OOP_3S_Lab234.Models;
using System;
using System.Collections.Generic;
using System.Text;

namespace OOP_3S_Lab234.ShipParts
{
    public class WarpJet : IJet
    {
        public AnimationManager _animationManager { get; set; }
        public Dictionary<string, Animation> _animations { get; set; }
        public string Color { get; set; }
        public string TypeOfJet { get; set; }
        public float Speed { get; set; }
        public Texture2D Texture { get; set; }
        public Texture2D Particles { get; set; }
        public WarpJet()
        {
            Speed = 300f;
        }
        void IJet.Boost()
        {
        }
    }
}
