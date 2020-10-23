using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;
using System;
using Vector2 = Microsoft.Xna.Framework.Vector2;
using Microsoft.Xna.Framework;
using OOP_3S_Lab234.ShipParts;
using System.Collections.Generic;
using System.Threading;
using System.Diagnostics;

namespace OOP_3S_Lab234.Entities
{
    public enum ShuttleType
    {
        Bug,
        Bat,
        Lunar,
        Massive
    }


    abstract class Shuttle
    {
        public readonly Dictionary<string, int> ShuttleJetOffset = new Dictionary<string, int>
        {
            ["Bug"] = -30,
            ["Bat"] = -10,
            ["Lunar"] = -30,
            ["Massive"] = -20
        };

        protected Vector2 velocity_ = Vector2.Zero;
        public string TypeOfShuttle { get; protected set; }
        public Vector2 Position { get; set; }
        public float RotateAngle { get; set; }
        public Texture2D Texture { get; set; }
        public Texture2D Cabin { get; set; }
        public bool isDamaged { get; set; }
        public float timer;
        public int frameWidth = 16;
        public int frameHeight = 16;
        public int framesCount = 4;
        public int currentFrame = 0;
        public void animationPlaying(GameTime gameTime)
        {
            timer += (float)gameTime.ElapsedGameTime.TotalSeconds;
            if (timer > 0.2)
            {
                timer = 0;
                currentFrame++;
                if (currentFrame >= framesCount)
                {
                    currentFrame = 0;
                }
            }
        }

        public Rectangle HitBox
        {
            get
            {
                return new Rectangle((int)Position.X, (int)Position.Y, Texture.Width, Texture.Height);
            }
        }
        protected IJet Jet = new SpeedJet();
        public void Draw(SpriteBatch _spriteBatch)
        {
            _spriteBatch.Draw(
                Texture,
                Position,
                null,
                isDamaged ? Color.Red : Color.White,
                RotateAngle,
                new Vector2(Texture.Width / 2, Texture.Height / 2),
                Vector2.One,
                SpriteEffects.None,
                0f
                );

            _spriteBatch.Draw(
                Cabin,
                Position,
                null,
                isDamaged ? Color.Red : Color.White,
                RotateAngle,
                new Vector2(Texture.Width / 2, Texture.Height / 2),
                Vector2.One,
                SpriteEffects.None,
                0f
                );

            _spriteBatch.Draw(
                Jet.Texture,
                Position,
                null,
                isDamaged ? Color.Red : Color.White,
                RotateAngle,
                new Vector2(Jet.Texture.Width / 2, ShuttleJetOffset[TypeOfShuttle]),
                Vector2.One,
                SpriteEffects.None,
                0f
                );

            _spriteBatch.Draw(
                Jet.Particles,
                Position,
                new Rectangle(currentFrame * frameWidth, 0, frameWidth, frameHeight),
                Color.White,
                RotateAngle,
                new Vector2(Jet.Particles.Width / 4 / 2, ShuttleJetOffset[TypeOfShuttle] - (Jet.Particles.Height / 2) + 6),
                Vector2.One,
                SpriteEffects.None,
                0f
                );
        }
        public virtual void Load(ContentManager Content, string cabin)
        {
            Random rand = new Random();
            Texture = Content.Load<Texture2D>("Images/Shuttle/Body/" + TypeOfShuttle.ToLower() + "Body");
            Cabin = Content.Load<Texture2D>("Images/Shuttle/Cabin/" + cabin);
            Jet.Load(Content, "Images/Shuttle/Jet/" + Jet.TypeOfJet.ToLower() + (rand.Next(0, 2) == 0 ? "Green" : "Blue") + "Jet");
        }
        protected virtual void BorderCollision(Vector2 offset, Vector2 resolution)
        {
            if (Position.Y + offset.Y < Texture.Height / 2 ||
                Position.Y + offset.Y > resolution.Y - Texture.Height / 2)
            {
                velocity_.Y = -velocity_.Y;
            }

            if (Position.X + offset.X < Texture.Height / 2 ||
                Position.X + offset.X > resolution.X - Texture.Height / 2)
            {
                velocity_.X = -velocity_.X;
            }
        }

        public abstract void Update(GameTime gameTime, Vector2 resolution);

        protected float t = 0;
        protected float step = 1;
        protected bool isStoped = false;
        protected bool clearStep = true;
        protected Vector2 tmpVel;
        protected float SmoothStep(float x)
        {
            return (float)Math.Sqrt(1 - Math.Pow(x - 1, 2));
        }
    }
}
