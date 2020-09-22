using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using OOP_3S_Lab234.Utils;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

namespace OOP_3S_Lab234
{
    static class MainMenu
    {
        public static Texture2D Texture { get; set; }

        public static void Update(GameTime gameTime)
        {
            if (Kb.getDown(Keys.Space) || Kb.getDown(Keys.Enter))
            {
                ASpaceOutside.State = ASpaceOutside.GameState.Gameplay;
            }
        }
        public static void Draw(SpriteBatch _spriteBatch, GraphicsDevice GraphicsDevice)
        {
            _spriteBatch.Begin();
            _spriteBatch.Draw(Texture, GraphicsDevice.Viewport.Bounds, Color.White);
            _spriteBatch.End();
        }
    }
}
