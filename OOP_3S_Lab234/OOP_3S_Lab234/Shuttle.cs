using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;
using Vector2 = Microsoft.Xna.Framework.Vector2;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework;

namespace OOP_3S_Lab234
{
    class Shuttle
    {
        protected Vector2 velocity_ = Vector2.Zero;

        protected Vector2 coords_ = Vector2.Zero;

        protected int speed_ = new int();

        protected Texture2D texture_;

        protected float smoothTimer = 0;

        public Vector2 Coords
        {
            get { return coords_; }
            set { coords_ = value; }
        }
        public int Speed
        {
            get { return speed_; }
            set { speed_ = value > 0 ? value : 0; }
        }
        public Texture2D Texture
        {
            get { return texture_; }
            set { texture_ = value; }
        }

        public void Update(GameTime gameTime)
        {
            if (Keyboard.GetState().IsKeyDown(Keys.Up))
            {
                coords_.Y -= Speed * (float)gameTime.ElapsedGameTime.TotalSeconds;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.Down))
            {
                coords_.Y += Speed * (float)gameTime.ElapsedGameTime.TotalSeconds;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.Left))
            {
                coords_.X -= Speed * (float)gameTime.ElapsedGameTime.TotalSeconds;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.Right))
            {
                coords_.X += Speed * (float)gameTime.ElapsedGameTime.TotalSeconds;
            }
        }
    }
}
