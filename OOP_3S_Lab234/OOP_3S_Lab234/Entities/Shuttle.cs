using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;
using System;
using Vector2 = Microsoft.Xna.Framework.Vector2;
using Microsoft.Xna.Framework;
using OOP_3S_Lab234.ShipParts;

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
        protected Vector2 velocity_ = Vector2.Zero;
        public string TypeOfShuttle { get; protected set; }
        public Vector2 Position { get; set; }
        public float RotateAngle { get; set; }
        public Texture2D Texture { get; set; }
        public Texture2D Cabin { get; set; }
        public bool isDamaged { get; set; }
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
            bool jetOffset = false;
            
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

            jetOffset = Texture.ToString() == "Images/Shuttle/Body/massiveBody" ||
                Texture.ToString() == "Images/Shuttle/Body/gamepadBody";

            _spriteBatch.Draw(
                Jet.Texture,
                Position,
                null,
                isDamaged ? Color.Red : Color.White,
                RotateAngle,
                new Vector2(Texture.Width / 2, jetOffset ? 10 : 0),
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
