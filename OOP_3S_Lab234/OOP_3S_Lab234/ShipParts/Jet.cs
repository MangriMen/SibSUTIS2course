﻿using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using OOP_3S_Lab234.Managers;
using OOP_3S_Lab234.Models;
using System;
using System.Collections.Generic;
using System.Linq;
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
        public AnimationManager _animationManager { get; set; }
        public Dictionary<string, Animation> _animations { get; set; }
        public string Color { get; set; }
        public string TypeOfJet { get; set; }
        float Speed { get; set; }
        Texture2D Texture { get; set; }
        void Load(ContentManager Content, string texturePath, Dictionary<string, Animation> animations)
        {
            Texture = Content.Load<Texture2D>(texturePath);

            _animations = animations;
            _animationManager = new AnimationManager(_animations.First().Value);

            string temp = Texture.ToString().Split("/").Last();
            int firstUpper = 0;
            while (firstUpper < temp.Length && temp[firstUpper] != temp.ToUpper()[firstUpper]) firstUpper++;

            Color = temp.Substring(firstUpper, temp.Length - firstUpper);
        }
        void Boost();
    }
}
