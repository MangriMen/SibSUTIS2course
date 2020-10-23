using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;
using OOP_3S_Lab234.Utils;
using Vector2 = Microsoft.Xna.Framework.Vector2;
using Microsoft.Xna.Framework.Content;
using OOP_3S_Lab234.ShipParts;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Threading;
using System.Collections.Generic;
using OOP_3S_Lab234.Models;

namespace OOP_3S_Lab234.Entities
{
    class Player : Shuttle
    {
        public Player(Vector2 spawnPoint)
        {
            Jet = new SpeedJet();
            Jet.Speed = 250;
            Position = spawnPoint;
        }
        public void Load(ContentManager Content)
        {
            Texture = Content.Load<Texture2D>("Images/Shuttle/Body/massiveBody");
            Cabin = Content.Load<Texture2D>("Images/Shuttle/Cabin/brickCabin");
            TypeOfShuttle = "Massive";
            Jet.Load(Content,
                "Images/Shuttle/Jet/slideBlueJet",
                new Dictionary<string, Animation> { ["Working"] = new Animation(Content.Load<Texture2D>("Images/Particles/slideParticles"), 10, 0.1f) }
                );
        }
        protected override void BorderCollision(Vector2 offset, Vector2 resolution)
        {
            if (Position.Y + offset.Y < Texture.Height / 2 ||
                Position.Y + offset.Y > resolution.Y - Texture.Height / 2)
            {
                velocity_.Y *= 0;
            }

            if (Position.X + offset.X < Texture.Height / 2 ||
                Position.X + offset.X > resolution.X - Texture.Height / 2)
            {
                velocity_.X *= 0;
            }
        }
        public override void Update(GameTime gameTime, Vector2 resolution)
        {
            float delta = (float)gameTime.ElapsedGameTime.TotalSeconds;

            Jet._animationManager.Play(Jet._animations["Working"]);
            Jet._animationManager.Update(gameTime);

            bool noKeyPressed = !(Kb.getDown(Keys.Up) || Kb.getDown(Keys.Down) || Kb.getDown(Keys.Left) || Kb.getDown(Keys.Right) ||
                Kb.getDown(Keys.W) || Kb.getDown(Keys.S) || Kb.getDown(Keys.A) || Kb.getDown(Keys.D));

            Vector2 temp = Position;

            if (noKeyPressed)
            {
                if (!isStoped)
                {
                    if (clearStep)
                    {
                        tmpVel = velocity_;
                        step = 0;
                    }
                    if (step <= 1)
                    {
                        step += 0.007f;
                        clearStep = false;
                        t = 1 - SmoothStep(step);
                        velocity_ = tmpVel * t;
                    }
                    else
                    {
                        clearStep = true;
                        isStoped = true;
                    }
                }   
            }
            else
            {
                if (Kb.getDown(Keys.Up) || Kb.getDown(Keys.W))
                    velocity_.Y = -1e10f;
                if (Kb.getDown(Keys.Down) || Kb.getDown(Keys.S))
                    velocity_.Y = 1e10f;
                if (Kb.getDown(Keys.Left) || Kb.getDown(Keys.A))
                    velocity_.X = -1e10f;
                if (Kb.getDown(Keys.Right) || Kb.getDown(Keys.D))
                    velocity_.X = 1e10f;

                if(velocity_ != Vector2.Zero)
                {
                    velocity_.Normalize();

                    if (double.IsNaN(velocity_.X) || double.IsInfinity(velocity_.X))
                        velocity_.X = 0;
                    if (double.IsNaN(velocity_.Y) || double.IsInfinity(velocity_.Y))
                        velocity_.Y = 0;
                }

                clearStep = true;
                isStoped = false;
            }

            Vector2 offset = velocity_ * Jet.Speed * delta;
            BorderCollision(offset, resolution);

            Position += velocity_ * Jet.Speed * delta;

            Jet._animationManager.Position = Position;

            if (Position - temp != Vector2.Zero)
                RotateAngle = (float)Math.Atan2(Position.Y - temp.Y, Position.X - temp.X) + (float)Math.PI / 2;
        }
    }
}
