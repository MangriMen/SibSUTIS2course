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
            Jet.Load(Content, "Images/Shuttle/Jet/speedOrangeJet");
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

            bool noKeyPressed = !(Kb.getDown(Keys.Up) || Kb.getDown(Keys.Down) || Kb.getDown(Keys.Left) || Kb.getDown(Keys.Right) ||
                Kb.getDown(Keys.W) || Kb.getDown(Keys.S) || Kb.getDown(Keys.A) || Kb.getDown(Keys.D));

            Vector2 temp = Position;

            if (noKeyPressed)
            {
                if (Position.Y > 0 + Texture.Height / 2 && Position.Y < resolution.Y &&
                    Position.X > 0 + Texture.Width / 2 && Position.X < resolution.X)
                {
                    if (Math.Abs(velocity_.X) > 0.05f || Math.Abs(velocity_.Y) > 0.05f)
                        velocity_ *= 0.95f;
                    else
                        velocity_ *= 0;
                }
                else
                    velocity_ *= 0;
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
            }

            Vector2 offset = velocity_ * Jet.Speed * delta;
            BorderCollision(offset, resolution);

            Position += velocity_ * Jet.Speed * delta;

            if (Position - temp != Vector2.Zero)
                RotateAngle = (float)Math.Atan2(Position.Y - temp.Y, Position.X - temp.X) + (float)Math.PI / 2;
            //Debug.WriteLine(Position - temp);
        }
    }
}
