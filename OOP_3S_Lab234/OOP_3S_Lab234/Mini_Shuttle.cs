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
            if (coords_.Y > 0 + Texture.Height * 4 / 2 && Keyboard.GetState().IsKeyDown(Keys.Up))
            {
                velocity_.Y -= 1.0f;
            }
            if (coords_.Y < resolution.Y - Texture.Height * 4 / 2 && Keyboard.GetState().IsKeyDown(Keys.Down) )
            {
                velocity_.Y += 1.0f;
            }
            if (coords_.X > 0 + Texture.Width * 4 / 2 && Keyboard.GetState().IsKeyDown(Keys.Left))
            {
                velocity_.X -= 1.0f;
            }
            if (coords_.X < resolution.X - Texture.Width * 4 / 2 && Keyboard.GetState().IsKeyDown(Keys.Right))
            {
                velocity_.X += 1.0f;
            }

            if (!Keyboard.GetState().IsKeyDown(Keys.Up) &&
                !Keyboard.GetState().IsKeyDown(Keys.Down) &&
                !Keyboard.GetState().IsKeyDown(Keys.Left) &&
                !Keyboard.GetState().IsKeyDown(Keys.Right))
            {
                while (velocity_ != Vector2.Zero)
                {
                    int counter = 1;
                    int limit = 10;
                    float countDuration = 2f;
                    float currentTime = 0f;

                    currentTime += (float)gameTime.ElapsedGameTime.TotalSeconds;

                    if (currentTime >= countDuration)
                    {
                        //if (velocity_.X > 0)
                        //{
                        //    velocity_.X *= 0.1F;
                        //}

                        //if (velocity_.Y > 0)
                        //{
                        //    velocity_.Y *= 0.1F;
                        //}

                        counter++;
                        currentTime -= countDuration;
                        Debug.WriteLine("Kek");
                    }
                    if (counter >= limit)
                    {
                        counter = 0;
                        velocity_ = Vector2.Zero;
                    }
                }
            }

            if (velocity_ != Vector2.Zero)
            {
                velocity_.Normalize();
            }

            coords_ += velocity_ * Speed * (float)gameTime.ElapsedGameTime.TotalSeconds;
        }
    }
}
