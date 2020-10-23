using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System;
using System.Collections.Generic;
using System.Text;

namespace OOP_3S_Lab234.ShipParts
{
    enum JetType
    {
        Slide,
        Speed,
        Warp
    }
    public interface IJet
    {
        public string TypeOfJet { get; set; }
        float Speed { get; set; }
        Texture2D Texture { get; set; }
        Texture2D Particles { get; set; }
        void Load(ContentManager Content, string texturePath)
        {
            Texture = Content.Load<Texture2D>(texturePath);
            Particles = Content.Load<Texture2D>("Images/Particles/JetParticles");
        }
        void Boost();
    }
}
