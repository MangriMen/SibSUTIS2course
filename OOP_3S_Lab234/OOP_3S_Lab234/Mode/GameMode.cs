using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System;
using System.Collections.Generic;
using System.Text;

namespace OOP_3S_Lab234.Mode
{
    abstract class GameMode
    {
        public abstract void Update(GameTime gameTime);
        public abstract void Draw(GameTime gameTime, SpriteBatch _spriteBatch, GraphicsDeviceManager _graphics);
    }
}
