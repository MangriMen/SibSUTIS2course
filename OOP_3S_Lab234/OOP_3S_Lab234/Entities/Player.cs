﻿using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;
using System.Collections.Generic;
using System.Data;
using System.Diagnostics;
using System.Numerics;
using System.Text;
using OOP_3S_Lab234.Utils;
using Vector2 = Microsoft.Xna.Framework.Vector2;

namespace OOP_3S_Lab234.Entities
{
    class Player : Shuttle
    {
        public Player(Vector2 spawnPoint)
        {
            Speed = 200;
            Position = spawnPoint;
        }

        public override void Update(GameTime gameTime, Vector2 resolution)
        {
            float delta = (float)gameTime.ElapsedGameTime.TotalSeconds;

            bool noKeyPressed = !(Kb.getDown(Keys.Up) || Kb.getDown(Keys.Down) || Kb.getDown(Keys.Left) || Kb.getDown(Keys.Right) ||
                Kb.getDown(Keys.W) || Kb.getDown(Keys.S) || Kb.getDown(Keys.A) || Kb.getDown(Keys.D));

            Vector2 temp = Position;

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
                if (Kb.getDown(Keys.Up) || Kb.getDown(Keys.W))
                {
                    velocity_.Y -= 1.0f;
                }
                if (Kb.getDown(Keys.Down) || Kb.getDown(Keys.S))
                {
                    velocity_.Y += 1.0f;
                }
                if (Kb.getDown(Keys.Left) || Kb.getDown(Keys.A))
                {
                    velocity_.X -= 1.0f;
                }
                if (Kb.getDown(Keys.Right) || Kb.getDown(Keys.D))
                {
                    velocity_.X += 1.0f;
                }
                if(velocity_ != Vector2.Zero)
                {
                    velocity_.Normalize();
                }
                Vector2 offset = velocity_ * Speed * delta;
                BorderCollision(offset, resolution, "player");
            }

            Position += velocity_ * Speed * delta;

            rotateAngle_ = (float)Math.Atan2(Position.Y - temp.Y, Position.X - temp.X) + (float)Math.PI / 2;
        }
    }
}
