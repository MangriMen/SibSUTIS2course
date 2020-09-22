﻿using Microsoft.Xna.Framework.Graphics;
using System;
using Vector2 = Microsoft.Xna.Framework.Vector2;
using Microsoft.Xna.Framework;
using System.Diagnostics;

namespace OOP_3S_Lab234
{
    class Enemy : Shuttle
    {
        public Enemy()
        {
            Speed = 0;
            position_ = new Vector2(0, 0);
            velocity_ = new Vector2(0, 0);
        }

        public Enemy(int speed, Vector2 spawnPoint)
        {
            Speed = speed;
            position_ = spawnPoint;
            velocity_ = new Vector2(0.5f, 0.5f);
        }

        public override void Update(GameTime gameTime, Vector2 resolution)
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