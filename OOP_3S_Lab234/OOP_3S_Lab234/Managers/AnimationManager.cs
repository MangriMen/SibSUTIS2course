using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using OOP_3S_Lab234.Models;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Numerics;
using System.Text;
using Vector2 = Microsoft.Xna.Framework.Vector2;

namespace OOP_3S_Lab234.Managers
{
    public class AnimationManager
    {
        private Animation _animation;

        private float _timer;
        public bool isFinished = false;
        public Color Color { get; set; } = Color.White;
        public Vector2 Position { get; set; }
        public Vector2 Pivot { get; set; } = Vector2.Zero;
        public float RotateAngle { get; set; } = 0;
        public Animation Current
        {
            get
            {
                return _animation;
            }
        }

        public AnimationManager(Animation animation)
        {
            _animation = animation;
        }

        public void Draw(SpriteBatch spriteBatch)
        {
            spriteBatch.Draw(
                _animation.Texture,
                Position,
                new Rectangle(_animation.CurrentFrame * _animation.FrameWidth, 0, _animation.FrameWidth, _animation.FrameHeight),
                Color,
                RotateAngle,
                Pivot,
                Vector2.One,
                SpriteEffects.None,
                0f
                );
        }

        public void Play(Animation animation)
        {
            if (_animation == animation)
                return;

            _animation = animation;

            _animation.CurrentFrame = 0;

            _timer = 0;
        }

        public void Stop()
        {
            _timer = 0f;

            _animation.CurrentFrame = 0;

            isFinished = true;
        }

        public void Update(GameTime gameTime)
        {
            _timer += (float)gameTime.ElapsedGameTime.TotalSeconds;

            if (!isFinished)
                if (_timer > _animation.FrameSpeed)
                {
                    _timer = 0f;

                    _animation.CurrentFrame++;

                    if (!_animation.IsLooping && _animation.CurrentFrame == _animation.FrameCount) Stop();
                    
                    if (_animation.CurrentFrame >= _animation.FrameCount)
                        _animation.CurrentFrame = 0;
                }
        }
    }
}
