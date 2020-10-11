using System;
using Vector2 = Microsoft.Xna.Framework.Vector2;
using Microsoft.Xna.Framework;
using OOP_3S_Lab234.ShipParts;

namespace OOP_3S_Lab234.Entities
{
    class Enemy : Shuttle
    {
        public Enemy(Vector2 spawnPoint, ShuttleType shuttle, JetType jet)
        {
            switch (shuttle)
            {
                case ShuttleType.Bat:
                    TypeOfShuttle = "Bat";
                    break;
                case ShuttleType.Bug:
                    TypeOfShuttle = "Bug";
                    break;
                case ShuttleType.Lunar:
                    TypeOfShuttle = "Lunar";
                    break;
                case ShuttleType.Massive:
                    TypeOfShuttle = "Massive";
                    break;
            }
            switch (jet)
            {
                case JetType.Slide:
                    Jet = new SlideJet();
                    Jet.TypeOfJet = "Slide";
                    break;
                case JetType.Speed:
                    Jet = new SpeedJet();
                    Jet.TypeOfJet = "Speed";
                    break;
                case JetType.Warp:
                    Jet = new WarpJet();
                    Jet.TypeOfJet = "Warp";
                    break;
            }
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
