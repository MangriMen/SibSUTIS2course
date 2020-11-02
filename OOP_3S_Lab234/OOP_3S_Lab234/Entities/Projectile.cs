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
        public Rectangle Collider
        {
            get
            {
                //return new Rectangle((int)Math.Round(Position.X) - (_animationManager.Current.FrameWidth / 2),
                //                     (int)Math.Round(Position.Y) - (_animationManager.Current.FrameHeight / 2),
                //                     _animationManager.Current.FrameWidth,
                //                     _animationManager.Current.FrameHeight);
                Point[] tmp = {
                    Utils.RotatedRectangle.Rotate(
                        new Vector2(
                            (int)Math.Round(Position.X),
                            (int)Math.Round(Position.Y)
                            ),
                        new Vector2(
                            (int)Math.Round(Position.X) - (_animationManager.Current.FrameWidth / 2),
                            (int)Math.Round(Position.Y) - (_animationManager.Current.FrameHeight / 2)
                            ),
                        RotateAngle).ToPoint(),
                    Utils.RotatedRectangle.Rotate(
                        new Vector2(
                            (int)Math.Round(Position.X),
                            (int)Math.Round(Position.Y)
                            ),
                        new Vector2(
                            (int)Math.Round(Position.X) + (_animationManager.Current.FrameWidth / 2),
                            (int)Math.Round(Position.Y) - (_animationManager.Current.FrameHeight / 2)
                            ),
                        RotateAngle).ToPoint(),
                    Utils.RotatedRectangle.Rotate(
                        new Vector2(
                            (int)Math.Round(Position.X),
                            (int)Math.Round(Position.Y)
                            ),
                        new Vector2(
                            (int)Math.Round(Position.X) + (_animationManager.Current.FrameWidth / 2),
                            (int)Math.Round(Position.Y) + (_animationManager.Current.FrameHeight / 2)
                            ),
                        RotateAngle).ToPoint(),
                    Utils.RotatedRectangle.Rotate(
                        new Vector2(
                            (int)Math.Round(Position.X),
                            (int)Math.Round(Position.Y)
                            ),
                        new Vector2(
                            (int)Math.Round(Position.X) - (_animationManager.Current.FrameWidth / 2),
                            (int)Math.Round(Position.Y) + (_animationManager.Current.FrameHeight / 2)
                            ),
                        RotateAngle).ToPoint(),
                };

                return new Rectangle(
                    (int)Utils.RotatedRectangle.Smallest(tmp).X,
                    (int)Utils.RotatedRectangle.Smallest(tmp).Y,
                    (int)Utils.RotatedRectangle.Biggest(tmp).X - (int)Utils.RotatedRectangle.Smallest(tmp).X,
                    (int)Utils.RotatedRectangle.Biggest(tmp).Y - (int)Utils.RotatedRectangle.Smallest(tmp).Y
                    );
            }
        }

        public Projectile(Vector2 spawnPoint, string proj)
        {
            Position = spawnPoint;
            //Speed = 50f;
            TypeOfProjectile = proj;
            velocity_ = new Vector2(0.5f, 0.5f);
        }

        public void Draw(SpriteBatch _spriteBatch)
        {
            _animationManager.Pivot = new Vector2(
                _animationManager.Current.FrameWidth / 2,
                _animationManager.Current.FrameHeight / 2
                );
            _animationManager.RotateAngle = RotateAngle;

            _animationManager.Draw(_spriteBatch);

            Utils.Sprite.DrawCollider(_spriteBatch, ColliderTexture, Collider);
        }

        public virtual void Load(ContentManager Content)
        {
            Texture = Content.Load<Texture2D>("Images/Projectales/" + TypeOfProjectile.ToLower() + "Proj");
            ColliderTexture = Content.Load<Texture2D>("Images/Backgrounds/white");
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
            if (RotateAngle > 3.14f * 2)
            {
                RotateAngle = 0;
            }

            RotateAngle += 0.012f;
        }
    }
}

