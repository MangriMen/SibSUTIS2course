using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using OOP_3S_Lab234.Control;
using OOP_3S_Lab234.Utils;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

namespace OOP_3S_Lab234
{
    static class MainMenu
    {
        public static List<Component> Controls;
        public static SpriteFont Font { get; set; }
        public static SpriteFont ShadowFont { get; set; }

        public static bool isGame = true;

        readonly static Color Purple = new Color(127, 110, 155);
        readonly static Color DarkPurple = new Color(72, 54, 102);

        public static Texture2D Texture { get; set; }

        public static void Load(ContentManager Content, Vector2 resolution)
        {
            Font = Content.Load<SpriteFont>("Fonts/default");
            ShadowFont = Content.Load<SpriteFont>("Fonts/shadow");
            Texture = Content.Load<Texture2D>("Images/Backgrounds/menu");
            Button PlayButton = new Button(Content.Load<Texture2D>("Images/Controls/button"), Content.Load<SpriteFont>("Fonts/default"))
            {
                Position = new Vector2(resolution.X * 0.44f, resolution.Y * 0.55f),
                Text = "Play",
            };
            PlayButton.Click += PlayButton_Click;
            Button SettingsButton = new Button(Content.Load<Texture2D>("Images/Controls/button"), Content.Load<SpriteFont>("Fonts/default"))
            {
                Position = new Vector2(resolution.X * 0.80f, resolution.Y * 0.85f),
                Text = "Settings",
            };
            Button ExitButton = new Button(Content.Load<Texture2D>("Images/Controls/button"), Content.Load<SpriteFont>("Fonts/default"))
            {
                Position = new Vector2(resolution.X * 0.03f, resolution.Y * 0.85f),
                Text = "Exit",
            };
            ExitButton.Click += ExitButton_Click;
            Controls = new List<Component>
            {
                PlayButton,
                SettingsButton,
                ExitButton,
            };
        }

        private static void PlayButton_Click(object sender, EventArgs e)
        {
            ASpaceOutside.State = ASpaceOutside.GameState.Gameplay;
        }
        private static void ExitButton_Click(object sender, System.EventArgs e)
        {
            isGame = false;
        }
        public static void Update(GameTime gameTime, Game game)
        {
            if (!isGame)
            {
                game.Exit();
            }

            foreach (var component in Controls)
                component.Update(gameTime);
        }
        public static void Draw(GameTime gameTime, SpriteBatch _spriteBatch, GraphicsDevice GraphicsDevice)
        {
            _spriteBatch.Begin();

            _spriteBatch.Draw(Texture, GraphicsDevice.Viewport.Bounds, Color.White);

            foreach (var component in Controls)
                component.Draw(gameTime, _spriteBatch);

            _spriteBatch.End();
        }
    }
}
