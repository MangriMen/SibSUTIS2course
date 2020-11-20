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
using OOP_3S_Lab234.Utils;
using System.Runtime.CompilerServices;

namespace OOP_3S_Lab234.Entities
{
    public enum ProjectileType
    {
        Rocket
    }

    class Projectile
    {
        protected Vector2 velocity_ = Vector2.Zero;
        public string TypeOfProjectile { get; protected set; }
        public Vector2 Position { get; set; }
        public float Speed { get; set; }
        public float RotateAngle { get; set; }
        public Texture2D Texture { get; set; }
        public Texture2D ColliderTexture { get; set; }
        public AnimationManager _animationManager { get; set; }
        public Dictionary<string, Animation> _animations { get; set; }
        public bool IsExists { get; set; }

        public bool IsDestroyed { get; set; }

        public PolygonCollider Collider;

        public Projectile(Vector2 spawnPoint, string proj, Vector2 velocity)
        {
            Position = spawnPoint;
            Speed = 600f;
            TypeOfProjectile = proj;
            velocity_ = velocity;
            IsExists = true;
        }

        public void Draw(SpriteBatch _spriteBatch)
        {
            if (!_animationManager.isFinished)
            {
                _animationManager.Pivot = new Vector2(
                _animationManager.Current.FrameWidth / 2,
                _animationManager.Current.FrameHeight / 2
                );
                _animationManager.RotateAngle = RotateAngle;

                _animationManager.Draw(_spriteBatch);

                Collider.Rotate(Collider.Position, RotateAngle);

                //Collider.Draw(_spriteBatch, ColliderTexture);
            }
        }

        public void Load(ContentManager Content)
        {
            Texture = Content.Load<Texture2D>("Images/Projectales/" + TypeOfProjectile.ToLower() + "Proj");

            ColliderTexture = Content.Load<Texture2D>("Images/Backgrounds/white");

            _animations = new Dictionary<string, Animation> {
                ["Working"] = new Animation(Content.Load<Texture2D>("Images/Projectales/" + TypeOfProjectile.ToLower() + "Proj"), 5, 0.1f),
                ["Exploding"] = new Animation(Content.Load<Texture2D>("Images/Particles/projectileExplosionPariclesBig"), 8, 0.08f, false)
            };

            _animationManager = new AnimationManager(_animations.First().Value);
            _animationManager.Play(_animations["Working"]);

            Vector2[] colliderPoints =
            {
                new Vector2(Texture.Bounds.X, Texture.Bounds.Y),
                new Vector2(Texture.Bounds.Width / _animationManager.Current.FrameCount, Texture.Bounds.Y),
                new Vector2(Texture.Bounds.Width / _animationManager.Current.FrameCount, Texture.Bounds.Height),
                new Vector2(Texture.Bounds.X, Texture.Bounds.Height),
            };

            Collider = new PolygonCollider(colliderPoints);
        }

        public void Load(Texture2D texture, Texture2D pixel, Dictionary<string, Animation> animations)
        {
            Texture = texture;

            ColliderTexture = pixel;

            _animations = animations;

            //_animations = animations.Keys.ToDictionary(_ => _, _ => animations[_]);

            _animationManager = new AnimationManager(_animations.First().Value);

            _animationManager.Play(_animations["Working"]);

            Vector2[] colliderPoints =
            {
                new Vector2(Texture.Bounds.X, Texture.Bounds.Y),
                new Vector2(Texture.Bounds.Width / _animationManager.Current.FrameCount, Texture.Bounds.Y),
                new Vector2(Texture.Bounds.Width / _animationManager.Current.FrameCount, Texture.Bounds.Height),
                new Vector2(Texture.Bounds.X, Texture.Bounds.Height),
            };

            Collider = new PolygonCollider(colliderPoints);
        }

        public void Blow()
        {
            _animationManager.Play(_animations["Exploding"]);
            IsExists = false;
        }

        public void Update(GameTime gameTime, Vector2 resolution)
        {
            float delta = (float)gameTime.ElapsedGameTime.TotalSeconds;

            if (Position.X > resolution.X || Position.X < 0 || Position.Y < 0 || Position.Y > resolution.Y) IsExists = false;

            _animationManager.Update(gameTime);

            Vector2 prevPos = Position;

            if (velocity_ != Vector2.Zero) { velocity_.Normalize(); }

            Position += velocity_ * Speed * delta;

            _animationManager.Position = Position;

            Collider.Position = Position;

            RotateAngle = (float)Math.Atan2(Position.Y - prevPos.Y, Position.X - prevPos.X) + (float)Math.PI / 2;

            Collider.Rotate(Collider.Position, RotateAngle / 100f);
        }
    }
}

