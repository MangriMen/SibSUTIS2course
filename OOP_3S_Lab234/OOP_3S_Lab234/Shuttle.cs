using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace OOP_3S_Lab234
{
    class Shuttle
    {
        protected Vector<double> velocity;

        protected Vector<double> coords;

        protected int speed_ = new int();

        protected Texture2D texture_;

        protected Texture2D Texture
        {
            get { return texture_; }
            set { texture_ = value; }
        }
        public int Speed
        {
            get { return speed_; }
            set { speed_ = value > 0 ? value : 0; }
        }
    }
}
