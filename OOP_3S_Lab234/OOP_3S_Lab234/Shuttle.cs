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

        protected Vector2 position_ = Vector2.Zero;

        protected int speed_ = new int();

        protected float rotateAngle_ = new float();

        protected Texture2D texture_;

        public Vector2 Position
        {
            get { return position_; }
            set { position_ = value; }
        }
        public float RotateAngle
        {
            get { return rotateAngle_; }
            set { rotateAngle_ = value; }
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
                position_.Y -= Speed * (float)gameTime.ElapsedGameTime.TotalSeconds;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.Down))
            {
                position_.Y += Speed * (float)gameTime.ElapsedGameTime.TotalSeconds;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.Left))
            {
                position_.X -= Speed * (float)gameTime.ElapsedGameTime.TotalSeconds;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.Right))
            {
                position_.X += Speed * (float)gameTime.ElapsedGameTime.TotalSeconds;
            }
        }
    }
}
