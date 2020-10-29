using System;
using System.Collections.Generic;
using System.Linq;
using System.Diagnostics;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework;
using Vector2 = Microsoft.Xna.Framework.Vector2;
using OOP_3S_Lab234.Models;
using OOP_3S_Lab234.Managers;

namespace OOP_3S_Lab234.Entities
{
    public enum ProjectileType
    {
        rocket
    }

    class Projectile
    {
        protected Vector2 velocity_ = Vector2.Zero;
        public string TypeOfProjectile { get; protected set; }
        public Vector2 Position { get; set; }
        public float Speed { get; set; }
        public float RotateAngle { get; set; }
        public Texture2D Texture { get; set; }
        public AnimationManager _animationManager { get; set; }
        public Dictionary<string, Animation> _animations { get; set; }

        public Rectangle HitBox
        {
            get
            {
                return new Rectangle((int)Position.X, (int)Position.Y, Texture.Width, Texture.Height);
            }
        }

        public Projectile(Vector2 spawnPoint, string proj)
        {
            Position = spawnPoint;
            //velocity_ = new Vector2(0.5f, 0.5f);
            TypeOfProjectile = proj;
        }

        public void Draw(SpriteBatch _spriteBatch)
        {
            //_animationManager.Pivot = new Vector2(
            //        Texture._animations["Working"].Texture.Width / Texture._animations["Working"].FrameCount / 2,
            //        ShuttleJetOffset[TypeOfShuttle] - (Texture._animations["Working"].Texture.Height / 2) + 2
            //        );

            //_animationManager.RotateAngle = RotateAngle;
            _animationManager.Draw(_spriteBatch);

            //_spriteBatch.Draw(
            //    Texture,
            //    Position,
            //    null,
            //    Color.White,
            //    RotateAngle,
            //    new Vector2(Texture.Width / 2, Texture.Height / 2),
            //    Vector2.One,
            //    SpriteEffects.None,
            //    0f
            //    );
        }

        public virtual void Load(ContentManager Content)
        {
            Random rand = new Random();
            Texture = Content.Load<Texture2D>("Images/Projectales/" + TypeOfProjectile.ToLower() + "Proj");
            _animations = new Dictionary<string, Animation> { ["Working"] = new Animation(Content.Load<Texture2D>("Images/Projectales/" + TypeOfProjectile.ToLower() + "Proj"), 5, 0.1f) };
            _animationManager = new AnimationManager(_animations.First().Value);

        }

        public void Update(GameTime gameTime, Vector2 resolution)
        {
            float delta = (float)gameTime.ElapsedGameTime.TotalSeconds;

            _animationManager.Play(_animations["Working"]);
            _animationManager.Update(gameTime);

            Vector2 prevPos = Position;

            if (velocity_ != Vector2.Zero) { velocity_.Normalize(); }

            Position += velocity_ * Speed * delta;

            _animationManager.Position = Position;

            //RotateAngle = (float)Math.Atan2(Position.Y - prevPos.Y, Position.X - prevPos.X) + (float)Math.PI / 2;
            //RotateAngle = 90.0f * ((float)Math.PI / 2.0f);
        }
    }
}

