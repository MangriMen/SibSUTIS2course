﻿using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;
using System.Diagnostics;
using System.Text.Json;

namespace OOP_3S_Lab234
{
    public class Game1 : Game
    {
        Vector2 resolution;
        Texture2D backgoundTexture;
        Player player;
        static uint numberOfClones = 5;
        Shuttle[] clones = new Shuttle[numberOfClones];

        private GraphicsDeviceManager _graphics;
        private SpriteBatch _spriteBatch;

        public Game1()
        {
            _graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            IsMouseVisible = true;
        }

        protected override void Initialize()
        {
            _graphics.GraphicsProfile = GraphicsProfile.HiDef;

            // Screen mode
            _graphics.IsFullScreen = false;

            // Auto resolution on fullscreen
            if (_graphics.IsFullScreen)
            {
                resolution.X = GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Width;
                resolution.Y = GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Height;
            } else
            {
                resolution.X = 1280;
                resolution.Y = 720;
            }

            // Screen resolution
            _graphics.PreferredBackBufferWidth = (int)resolution.X;
            _graphics.PreferredBackBufferHeight = (int)resolution.Y;

            _graphics.PreferMultiSampling = true;
            GraphicsDevice.PresentationParameters.MultiSampleCount = 8;

            _graphics.ApplyChanges();

            player = new Player(new Vector2(_graphics.PreferredBackBufferWidth / 2, 
                _graphics.PreferredBackBufferHeight / 2));

            Random random = new Random();
            for (int i = 0; i < numberOfClones; i++)
            {
                clones[i] = new Shuttle(random.Next(100, 300), new Vector2(random.Next(100, (int)resolution.X - 100), random.Next(100, (int)(resolution.Y - 100))));
            }

            base.Initialize();
        }

        protected override void LoadContent()
        {
            _spriteBatch = new SpriteBatch(GraphicsDevice);

            backgoundTexture = Content.Load<Texture2D>("Images/Backgrounds/background");

            player.Texture = Content.Load<Texture2D>("Images/Shuttle/Body/massiveBody");
            player.Cabin = Content.Load<Texture2D>("Images/Shuttle/Cabin/brickCabin");
            player.Jet = Content.Load<Texture2D>("Images/Shuttle/Jet/doubleOrangeJet");

            String[] bodies = new String[4] { "bugBody", "gamepadBody", "horseshoeBody", "massiveBody" };
            String[] cabins = new String[4] { "brickCabin", "conusCabinDouble", "ovalCabin", "raindropDoubleCabin" };
            String[] jets = new String[6] { "doubleBlueJet", "doubleGreenJet", "doubleOrangeJet", "monoBlueJet", "monoGreenJet", "monoOrangeJet" };
            Random random = new Random();
            for (int i = 0; i < numberOfClones; i++)
            {
                clones[i].Texture = Content.Load<Texture2D>("Images/Shuttle/Body/" + bodies[random.Next(0, 4)]);
                clones[i].Cabin = Content.Load<Texture2D>("Images/Shuttle/Cabin/" + cabins[random.Next(0, 4)]);
                clones[i].Jet = Content.Load<Texture2D>("Images/Shuttle/Jet/" + jets[random.Next(0, 6)]);
            }
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                Exit();

            player.Update(gameTime, resolution);
            
            for (int i = 0; i < numberOfClones; i++)
            {
                clones[i].Update(gameTime, resolution);
            }

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            bool jetOffset = false;

            _spriteBatch.Begin();
            _spriteBatch.Draw(backgoundTexture, GraphicsDevice.Viewport.Bounds, Color.White);
            for (int i = 0; i < numberOfClones; i++)
            {
                _spriteBatch.Draw(
                clones[i].Texture,
                clones[i].Position,
                null,
                Color.White,
                clones[i].RotateAngle,
                new Vector2(clones[i].Texture.Width / 2, clones[i].Texture.Height / 2),
                Vector2.One,
                SpriteEffects.None,
                0f
                );

                _spriteBatch.Draw(
                clones[i].Cabin,
                clones[i].Position,
                null,
                Color.White,
                clones[i].RotateAngle,
                new Vector2(clones[i].Texture.Width / 2, clones[i].Texture.Height / 2),
                Vector2.One,
                SpriteEffects.None,
                0f
                );

                jetOffset = clones[i].Texture.ToString() == "Images/Shuttle/Body/massiveBody" ||
                    clones[i].Texture.ToString() == "Images/Shuttle/Body/gamepadBody";

                _spriteBatch.Draw(
                clones[i].Jet,
                clones[i].Position,
                null,
                Color.White,
                clones[i].RotateAngle,
                new Vector2(clones[i].Texture.Width / 2, jetOffset ? 10 : 0),
                Vector2.One,
                SpriteEffects.None,
                0f
                );
            }
            _spriteBatch.Draw(
                player.Texture,
                player.Position,
                null,
                Color.White,
                player.RotateAngle,
                new Vector2(player.Texture.Width / 2, player.Texture.Height / 2),
                Vector2.One,
                SpriteEffects.None,
                1f
            );
            _spriteBatch.Draw(
                player.Cabin,
                player.Position,
                null,
                Color.White,
                player.RotateAngle,
                new Vector2(player.Texture.Width / 2, player.Texture.Height / 2),
                Vector2.One,
                SpriteEffects.None,
                1f
            );

            jetOffset = player.Texture.ToString() == "Images/Shuttle/Body/massiveBody" ||
                    player.Texture.ToString() == "Images/Shuttle/Body/gamepadBody";

            _spriteBatch.Draw(
                player.Jet,
                player.Position,
                null,
                Color.White,
                player.RotateAngle,
                new Vector2(player.Texture.Width / 2, jetOffset ? 10 : 0),
                Vector2.One,
                SpriteEffects.None,
                1f
            );
            _spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
