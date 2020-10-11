using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;
using System.Diagnostics;
using System.Text.Json;
using System.Threading;
using System.Threading.Tasks;
using OOP_3S_Lab234.Entities;
using System.Drawing;
using Color = Microsoft.Xna.Framework.Color;

namespace OOP_3S_Lab234
{
    public class ASpaceOutside : Game
    {
        public enum GameState
        {
            LoadingStart,
            MainMenu,
            Gameplay,
        }

        public static GameState State { get; set; }


        Vector2 resolution;

        Texture2D backgoundTexture;
        Texture2D loadingScreen;
        Player player;
        Bug[] clonesBug = new Bug[6];
        Bat[] clonesBat = new Bat[3];
        Massive[] clonesMassive = new Massive[2];
        Lunar[] clonesLunar = new Lunar[3];

        private GraphicsDeviceManager _graphics;
        private SpriteBatch _spriteBatch;
        Random random = new Random();

        public ASpaceOutside()
        {
            _graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            IsMouseVisible = true;
        }

        protected override void Initialize()
        {
            _graphics.GraphicsProfile = GraphicsProfile.HiDef;

            Window.Title = "A Space Outside!";

            // Screen mode
            _graphics.IsFullScreen = false;

            _graphics.SynchronizeWithVerticalRetrace = false; // vsync
            IsFixedTimeStep = false; // Don't force equal timestep updates

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

            for (int i = 0; i < clonesBug.Length; ++i)
            {
                clonesBug[i] = new Bug(new Vector2(
                    random.Next(65, _graphics.PreferredBackBufferWidth - 100),
                    random.Next(65, _graphics.PreferredBackBufferHeight - 100)
                ));
            }
            for (int i = 0; i < clonesBat.Length; ++i)
            {
                clonesBat[i] = new Bat(new Vector2(
                    random.Next(_graphics.PreferredBackBufferWidth - 300, _graphics.PreferredBackBufferWidth - 100),
                    random.Next(65, _graphics.PreferredBackBufferHeight - 100)
                ));
            }
            for (int i = 0; i < clonesMassive.Length; ++i)
            {
                clonesMassive[i] = new Massive(new Vector2(
                    random.Next(65, 350),
                    random.Next(65, _graphics.PreferredBackBufferHeight / 2)
                ));
            }
            for (int i = 0; i < clonesLunar.Length; ++i)
            {
                clonesLunar[i] = new Lunar(new Vector2(
                    random.Next(65, 350),
                    random.Next(_graphics.PreferredBackBufferHeight / 2, _graphics.PreferredBackBufferHeight - 65)
                ));
            }

            base.Initialize();
        }

        protected override void LoadContent()
        {
            _spriteBatch = new SpriteBatch(GraphicsDevice);

            loadingScreen = Content.Load<Texture2D>("Images/Backgrounds/bootloader");

            Thread bgLoad = new Thread(new ThreadStart(LoadGame));
            bgLoad.IsBackground = true;
            bgLoad.Start();
        }

        protected void LoadGame()
        {
            MainMenu.Load(Content);

            backgoundTexture = Content.Load<Texture2D>("Images/Backgrounds/background");

            player.Load(Content);

            //String[] bodies = new String[4] { "bugBody", "gamepadBody", "lunarBody", "massiveBody" };
            String[] cabins = new String[4] { "brickCabin", "conusCabinDouble", "ovalCabin", "raindropDoubleCabin" };
            String[] jets = new String[6] { "doubleBlueJet", "doubleGreenJet", "doubleOrangeJet", "monoBlueJet", "monoGreenJet", "monoOrangeJet" };


            for (int i = 0; i < clonesBug.Length; ++i)
            {
                clonesBug[i].Load(Content,
                    "bugBody",
                    cabins[1],
                    jets[random.Next(3, 5)]
                    );
            }
            for (int i = 0; i < clonesBat.Length; ++i)
            {
                clonesBat[i].Load(Content,
                    "batBody",
                    cabins[random.Next(0, 2) * 2],
                    "doubleGreenJet"
                    );
            }
            for (int i = 0; i < clonesMassive.Length; ++i)
            {
                clonesMassive[i].Load(Content,
                    "massiveBody",
                    cabins[random.Next(0, 2) == 0 ? 0 : cabins.Length - 1],
                    jets[random.Next(0, 4)]
                    );
            }
            for (int i = 0; i < clonesLunar.Length; ++i)
            {
                clonesLunar[i].Load(Content,
                    "lunarBody",
                    cabins[random.Next(0, 2) * 2 + 1],
                    jets[random.Next(0, 4)]
                    );
            }

            Timer timer = new Timer(new TimerCallback(GameLoaded), null, 800, Timeout.Infinite);
        }

        private void GameLoaded(object obj)
        {
            State = GameState.MainMenu;
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                Exit();

            switch (State)
            {
                case GameState.LoadingStart:
                    break;
                case GameState.MainMenu:
                    MainMenu.Update(gameTime);
                    break;
                case GameState.Gameplay:
                    UpdateGameplay(gameTime);
                    break;
            }

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            switch (State)
            {
                case GameState.LoadingStart:
                    _spriteBatch.Begin();
                    _spriteBatch.Draw(loadingScreen, GraphicsDevice.Viewport.Bounds, Color.White);
                    _spriteBatch.End();
                    break;
                case GameState.MainMenu:
                    MainMenu.Draw(_spriteBatch, GraphicsDevice);
                    break;
                case GameState.Gameplay:
                    DrawGameplay();
                    break;
            }
            base.Draw(gameTime);
        }

        protected void UpdateGameplay(GameTime gameTime)
        {
            player.Update(gameTime, resolution);
           
            for (int i = 0; i < clonesBug.Length; ++i)
            {
                clonesBug[i].Update(gameTime, resolution);
            }
            for (int i = 0; i < clonesBat.Length; ++i)
            {
                clonesBat[i].Update(gameTime, resolution);
            }
            for (int i = 0; i < clonesMassive.Length; ++i)
            {
                clonesMassive[i].Update(gameTime, resolution);
            }
            for (int i = 0; i < clonesLunar.Length; ++i)
            {
                clonesLunar[i].Update(gameTime, resolution);
            }
        }
        protected void DrawGameplay()
        {
            _spriteBatch.Begin();
            _spriteBatch.Draw(backgoundTexture, GraphicsDevice.Viewport.Bounds, Color.White);

            for (int i = 0; i < clonesBug.Length; ++i)
            {
                clonesBug[i].Draw(_spriteBatch);
            }
            for (int i = 0; i < clonesBat.Length; ++i)
            {
                clonesBat[i].Draw(_spriteBatch);
            }
            for (int i = 0; i < clonesMassive.Length; ++i)
            {
                clonesMassive[i].Draw(_spriteBatch);
            }
            for (int i = 0; i < clonesLunar.Length; ++i)
            {
                clonesLunar[i].Draw(_spriteBatch);
            }

            player.Draw(_spriteBatch);

            _spriteBatch.End();
        }
    }
}
