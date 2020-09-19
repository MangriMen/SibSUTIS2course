using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;
using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;
using Vector2 = Microsoft.Xna.Framework.Vector2;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework;
using System.Diagnostics;
using Microsoft.VisualBasic.CompilerServices;

namespace OOP_3S_Lab234
{
    class Shuttle
    {
        protected Vector2 velocity_ = Vector2.Zero;

        protected Vector2 position_ = Vector2.Zero;

        protected int speed_ = new int();

        protected float rotateAngle_ = new float();

        protected Texture2D texture_;

        protected Texture2D cabin_;

        protected Texture2D jet_;

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
        public Texture2D Cabin
        {
            get { return cabin_; }
            set { cabin_ = value; }
        }
        public Texture2D Jet
        {
            get { return jet_; }
            set { jet_ = value; }
        }

        public Shuttle()
        {
            Speed = 0;
            position_ = new Vector2(0, 0);
            velocity_ = new Vector2(0, 0);
        }

        public Shuttle(int speed, Vector2 spawnPoint)
        {
            Speed = speed;
            position_ = spawnPoint;
            velocity_ = new Vector2(0.5f, 0.5f);
        }

        protected void BorderCollision(Vector2 offset, Vector2 resolution, String type)
        {
            bool typeB = type == "player" ? false : true;
            if (Position.Y + offset.Y < Texture.Height / 2 ||
                Position.Y + offset.Y > resolution.Y - Texture.Height / 2)
            {
                velocity_.Y = Convert.ToInt32(typeB) * -velocity_.Y;
            }

            if (Position.X + offset.X < Texture.Height / 2 ||
                Position.X + offset.X > resolution.X - Texture.Height / 2)
            {
                velocity_.X = Convert.ToInt32(typeB) * -velocity_.X;
            }
        }

        public virtual void Update(GameTime gameTime, Vector2 resolution)
        {
            float delta = (float)gameTime.ElapsedGameTime.TotalSeconds;

            Vector2 prevPos = Position;

            if (velocity_ != Vector2.Zero) { velocity_.Normalize(); }

            BorderCollision(velocity_ * Speed * delta, resolution, "bot");

            position_ += velocity_ * Speed * delta;

            rotateAngle_ = (float)Math.Atan2(Position.Y - prevPos.Y, Position.X - prevPos.X) + (float)Math.PI / 2;
        }
    }
}
