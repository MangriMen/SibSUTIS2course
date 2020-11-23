using System;
using Vector2 = Microsoft.Xna.Framework.Vector2;
using Microsoft.Xna.Framework;
using OOP_3S_Lab234.ShipParts;

namespace OOP_3S_Lab234.Entities
{
    class Enemy : Shuttle
    {
        public Enemy(Vector2 spawnPoint, ShuttleType shuttle, JetType jet, Vector2 _velocity, bool _isAbleToAttack = true, float _attackDelay = 0.2f)
        {
            Position = spawnPoint;
            velocity_ = _velocity;
            isExist = true;
            isAbleToAttack = _isAbleToAttack;
            attackDelay = _attackDelay;

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
        }
        public override void Update(GameTime gameTime, Vector2 resolution)
        {
            float delta = (float)gameTime.ElapsedGameTime.TotalSeconds;

            projectilesDestroyTimer += delta;

            if (projectilesDestroyTimer >= 0.5f)
            {
                for (int i = 0; i < projectiles.Count; i++)
                {
                    if (!projectiles[i].IsExists) { projectiles[i] = null; projectiles.Remove(projectiles[i]); }
                }
                projectilesDestroyTimer = 0f;
            }

            attackTimer += delta;

            if (attackTimer >= attackDelay) { isAbleToAttack = true; attackTimer = 0; }

            if (isAbleToAttack)
                Attack();

            foreach (var projectile in projectiles)
                projectile.Update(gameTime, resolution);

            Jet._animationManager.Play(Jet._animations["Working"]);
            Jet._animationManager.Update(gameTime);

            Vector2 prevPos = Position;

            if (velocity_ != Vector2.Zero) { velocity_.Normalize(); }

            BorderCollision(velocity_ * Jet.Speed * delta, resolution);

            Position += velocity_ * Jet.Speed * delta;

            Jet._animationManager.Position = Position;

            Collider.Position = Position;

            if (Position - prevPos != Vector2.Zero)
            {
                RotateAngle = (float)Math.Atan2(Position.Y - prevPos.Y, Position.X - prevPos.X) + (float)Math.PI / 2;
            }

            Collider.Rotate(Collider.Position, RotateAngle);
        }
    }
}
