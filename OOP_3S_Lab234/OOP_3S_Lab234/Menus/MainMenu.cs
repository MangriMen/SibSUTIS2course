using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
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
        public static SpriteFont Font { get; set; }
        public static SpriteFont ShadowFont { get; set; }

        readonly static Color Purple = new Color(127, 110, 155);
        readonly static Color DarkPurple = new Color(72, 54, 102);

        public static Texture2D Texture { get; set; }

        public static void Load(ContentManager Content)
        {
            Font = Content.Load<SpriteFont>("Fonts/default");
            ShadowFont = Content.Load<SpriteFont>("Fonts/shadow");
            Texture = Content.Load<Texture2D>("Images/Backgrounds/menu");
        }
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
            Vector2 pressEnter = new Vector2(GraphicsDevice.Viewport.Width / 2 - 60, GraphicsDevice.Viewport.Height - 150);
            int offset = 2;
            _spriteBatch.DrawString(
                Font,
                "Press Enter to start the game",
                pressEnter + new Vector2(0, -offset),
                Purple
            );
            _spriteBatch.DrawString(
                Font,
                "Press Enter to start the game",
                pressEnter + new Vector2(offset, 0),
                Purple
            );
            _spriteBatch.DrawString(
                Font,
                "Press Enter to start the game",
                pressEnter + new Vector2(0, offset),
                Purple
            );
            _spriteBatch.DrawString(
                Font,
                "Press Enter to start the game",
                pressEnter + new Vector2(-offset, 0),
                Purple
            );
            _spriteBatch.DrawString(
                Font,
                "Press Enter to start the game",
                pressEnter,
                DarkPurple
            );
            _spriteBatch.End();
        }
    }
}
