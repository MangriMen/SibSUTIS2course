using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;
using System.Collections.Generic;
using System.Data;
using System.Diagnostics;
using System.Numerics;
using System.Text;
using Vector2 = Microsoft.Xna.Framework.Vector2;

namespace OOP_3S_Lab234
{
    class Mini_Shuttle : Shuttle
    {
        public Mini_Shuttle(Vector2 spawnPoint)
        {
            Speed = 200;
            Position = spawnPoint;
        }

        public void Update(GameTime gameTime, Vector2 resolution)
        {
            float delta = (float)gameTime.ElapsedGameTime.TotalSeconds;

            bool noKeyPressed = !Keyboard.GetState().IsKeyDown(Keys.Up) &&
                !Keyboard.GetState().IsKeyDown(Keys.Down) &&
                !Keyboard.GetState().IsKeyDown(Keys.Left) &&
                !Keyboard.GetState().IsKeyDown(Keys.Right);

            Vector2 temp = velocity_;

            if (noKeyPressed)
            {
                if (Position.Y > 0 + Texture.Height / 2 && Position.Y < resolution.Y &&
                    Position.X > 0 + Texture.Width / 2 && Position.X < resolution.X)
                {
                    velocity_ *= 0.95f;
                }
                else
                {
                    velocity_ *= 0;
                }
            }
            else
            {
                if (Keyboard.GetState().IsKeyDown(Keys.Up))
                {
                    velocity_.Y -= 1.0f;
                }
                if (Keyboard.GetState().IsKeyDown(Keys.Down) )
                {
                    velocity_.Y += 1.0f;
                }
                if (Keyboard.GetState().IsKeyDown(Keys.Left))
                {
                    velocity_.X -= 1.0f;
                }
                if (Keyboard.GetState().IsKeyDown(Keys.Right))
                {
                    velocity_.X += 1.0f;
                }
                rotateAngle_ = (float)Math.Atan2(velocity_.Y - temp.Y, velocity_.X - temp.X) + (float)Math.PI / 2;
                if(velocity_ != Vector2.Zero)
                {
                    velocity_.Normalize();
                }
                Vector2 offset = velocity_ * Speed * delta;
                if (Position.Y + offset.Y < Texture.Height / 2 ||
                    Position.Y + offset.Y > resolution.Y - Texture.Height / 2) { velocity_.Y = 0; }
                if (Position.X + offset.X < Texture.Height / 2 ||
                    Position.X + offset.X > resolution.X - Texture.Height / 2) { velocity_.X = 0; }
            }
            Position += velocity_ * Speed * delta;
        }
    }
}
