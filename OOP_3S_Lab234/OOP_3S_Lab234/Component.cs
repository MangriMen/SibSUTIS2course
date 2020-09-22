using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System;
using System.Collections.Generic;
using System.Text;

namespace OOP_3S_Lab234
{
    public abstract class Component
    {
        public abstract void Draw(GameTime gameTime, SpriteBatch _spriteBatch);
        public abstract void Update(GameTime gameTime);
    }
}
