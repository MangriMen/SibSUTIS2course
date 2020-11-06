using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;
using System;
using Vector2 = Microsoft.Xna.Framework.Vector2;
using Microsoft.Xna.Framework;
using OOP_3S_Lab234.ShipParts;
using System.Collections.Generic;
using System.Threading;
using System.Diagnostics;
using OOP_3S_Lab234.Models;
using System.Linq;
using OOP_3S_Lab234.Utils;

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
            ["Bug"] = -28,
            ["Bat"] = -7,
            ["Lunar"] = -28,
            ["Massive"] = -20
        };

        public Dictionary<string, Color> JetColors = new Dictionary<string, Color>
        {
            ["BlueJet"] = new Color(51, 147, 212),
            ["GreenJet"] = new Color(83, 255, 0),
            ["OrangeJet"] = new Color(238, 120, 26)
        };

        protected Vector2 velocity_ = Vector2.Zero;
        public string TypeOfShuttle { get; protected set; }
        public Vector2 Position { get; set; }
        public float RotateAngle { get; set; }
        public Texture2D Texture { get; set; }
        public Texture2D Cabin { get; set; }
        public bool isDamaged { get; set; }
        public Texture2D ColliderTexture { get; set; }
        public PolygonCollider Collider = new PolygonCollider(null);
        public IJet Jet = new SpeedJet();

        public void Draw(SpriteBatch _spriteBatch)
        {
            Jet._animationManager.Color = JetColors[Jet.Color];
            Jet._animationManager.Pivot = new Vector2(
                    Jet._animations["Working"].Texture.Width / Jet._animations["Working"].FrameCount / 2,
                    ShuttleJetOffset[TypeOfShuttle] - (Jet._animations["Working"].Texture.Height / 2) + 2
                    );
            Jet._animationManager.RotateAngle = RotateAngle;
            Jet._animationManager.Draw(_spriteBatch);

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


            for (int i = 0; i < Collider.Lines.Length; i++)
                Collider.Lines[i].Draw(_spriteBatch, ColliderTexture);

            //Utils.Sprite.DrawCollider(_spriteBatch, ColliderTexture, Collider);
        }
        public virtual void Load(ContentManager Content, string cabin)
        {
            Random rand = new Random();
            Texture = Content.Load<Texture2D>("Images/Shuttle/Body/" + TypeOfShuttle.ToLower() + "Body");
            Cabin = Content.Load<Texture2D>("Images/Shuttle/Cabin/" + cabin);
            ColliderTexture = Content.Load<Texture2D>("Images/Backgrounds/white");
            Jet.Load(
                Content,
                "Images/Shuttle/Jet/" + Jet.TypeOfJet.ToLower() + (rand.Next(0, 2) == 0 ? "Green" : "Blue") + "Jet",
                new Dictionary<string, Animation> { ["Working"] = new Animation(Content.Load<Texture2D>("Images/Particles/" + Jet.TypeOfJet.ToLower() + "Particles"), 10, 0.1f) }
                );

            Vector2[] tmp =
            {
                new Vector2(Texture.Bounds.X, Texture.Bounds.Y),
                new Vector2(Texture.Bounds.Width, Texture.Bounds.Y),
                new Vector2(Texture.Bounds.Width, Texture.Bounds.Height),
                new Vector2(Texture.Bounds.X, Texture.Bounds.Height),
            };

            Collider = new PolygonCollider(tmp);
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
