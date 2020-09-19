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
            Coords = spawnPoint;
        }

        public void Update(GameTime gameTime, Vector2 resolution)
        {
            float delta = (float)gameTime.ElapsedGameTime.TotalSeconds;
            bool noKeyPressed = !Keyboard.GetState().IsKeyDown(Keys.Up) &&
                !Keyboard.GetState().IsKeyDown(Keys.Down) &&
                !Keyboard.GetState().IsKeyDown(Keys.Left) &&
                !Keyboard.GetState().IsKeyDown(Keys.Right);

            if (noKeyPressed)
            {
                velocity_ *= 0.95f;
            }
            else
            {
                if (coords_.Y > 0 + Texture.Height / 2 && Keyboard.GetState().IsKeyDown(Keys.Up))
                {
                    rotateAngle_ = 0.0f;
                    velocity_.Y -= 1.0f;
                }
                if (coords_.Y < resolution.Y - Texture.Height / 2 && Keyboard.GetState().IsKeyDown(Keys.Down) )
                {
                    rotateAngle_ = (float)Math.PI;
                    velocity_.Y += 1.0f;
                }
                if (coords_.X > 0 + Texture.Width / 2 && Keyboard.GetState().IsKeyDown(Keys.Left))
                {
                    rotateAngle_ = -(float)Math.PI / 2.0f;
                    velocity_.X -= 1.0f;
                }
                if (coords_.X < resolution.X - Texture.Width / 2 && Keyboard.GetState().IsKeyDown(Keys.Right))
                {
                    rotateAngle_ = (float)Math.PI / 2.0f;
                    velocity_.X += 1.0f;
                }
                if (velocity_ != Vector2.Zero)
                {
                    velocity_.Normalize();
                }
            }
            coords_ += velocity_ * Speed * delta;
        }
    }
}
