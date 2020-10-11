using Microsoft.Xna.Framework.Graphics;
using System;
using Vector2 = Microsoft.Xna.Framework.Vector2;
using Microsoft.Xna.Framework;
using System.Diagnostics;
using Microsoft.Xna.Framework.Content;

namespace OOP_3S_Lab234.Entities
{
    class BugEnemy : Enemy
    {
        public BugEnemy()
        {
            Random random = new Random();
            Jet.Speed = random.Next(300, 401);
            Position = new Vector2(0, 0);
            velocity_ = new Vector2(0, 0);
        }

        public BugEnemy(Vector2 spawnPoint)
        {
            Random random = new Random();
            Jet.Speed = random.Next(300, 401);
            Position = spawnPoint;
            velocity_ = new Vector2(0.5f, 0.5f);
        }
        public override void Update(GameTime gameTime, Vector2 resolution)
        {
            float delta = (float)gameTime.ElapsedGameTime.TotalSeconds;

            Vector2 prevPos = Position;

            if (velocity_ != Vector2.Zero) { velocity_.Normalize(); }

            BorderCollision(velocity_ * Jet.Speed * delta, resolution, "bot");

            Position += velocity_ * Jet.Speed * delta;

            RotateAngle = (float)Math.Atan2(Position.Y - prevPos.Y, Position.X - prevPos.X) + (float)Math.PI / 2;
        }

    }
}
