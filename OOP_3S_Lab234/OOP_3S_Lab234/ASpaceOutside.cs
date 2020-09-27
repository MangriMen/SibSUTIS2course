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

        SpriteFont usually;

        Vector2 resolution;

        Texture2D backgoundTexture;
        Texture2D loadingScreen;
        Player player;
        BugEnemy[] clonesBug = new BugEnemy[6];
        GamepadEnemy[] clonesGamepad = new GamepadEnemy[3];
        MassiveEnemy[] clonesMassive = new MassiveEnemy[2];
        LunarEnemy[] clonesLunar = new LunarEnemy[3];

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

            //State = GameState.MainMenu;

            player = new Player(new Vector2(_graphics.PreferredBackBufferWidth / 2, 
                _graphics.PreferredBackBufferHeight / 2));

            for (int i = 0; i < clonesBug.Length; ++i)
            {
                clonesBug[i] = new BugEnemy(new Vector2(
                    random.Next(65, _graphics.PreferredBackBufferWidth - 100),
                    random.Next(65, _graphics.PreferredBackBufferHeight - 100)
                ));
            }
            for (int i = 0; i < clonesGamepad.Length; ++i)
            {
                clonesGamepad[i] = new GamepadEnemy(new Vector2(
                    random.Next(_graphics.PreferredBackBufferWidth - 300, _graphics.PreferredBackBufferWidth - 100),
                    random.Next(65, _graphics.PreferredBackBufferHeight - 100)
                ));
            }
            for (int i = 0; i < clonesMassive.Length; ++i)
            {
                clonesMassive[i] = new MassiveEnemy(new Vector2(
                    random.Next(65, 350),
                    random.Next(65, _graphics.PreferredBackBufferHeight / 2)
                ));
            }
            for (int i = 0; i < clonesLunar.Length; ++i)
            {
                clonesLunar[i] = new LunarEnemy(new Vector2(
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
            usually = Content.Load<SpriteFont>("Fonts/default");
            MainMenu.Texture = Content.Load<Texture2D>("Images/Backgrounds/menu");
            backgoundTexture = Content.Load<Texture2D>("Images/Backgrounds/background");

            player.Texture = Content.Load<Texture2D>("Images/Shuttle/Body/massiveBody");
            player.Cabin = Content.Load<Texture2D>("Images/Shuttle/Cabin/brickCabin");
            player.Jet = Content.Load<Texture2D>("Images/Shuttle/Jet/doubleOrangeJet");

            //String[] bodies = new String[4] { "bugBody", "gamepadBody", "lunarBody", "massiveBody" };
            String[] cabins = new String[4] { "brickCabin", "conusCabinDouble", "ovalCabin", "raindropDoubleCabin" };
            String[] jets = new String[6] { "doubleBlueJet", "doubleGreenJet", "doubleOrangeJet", "monoBlueJet", "monoGreenJet", "monoOrangeJet" };


            for (int i = 0; i < clonesBug.Length; ++i)
            {
                clonesBug[i].Texture = Content.Load<Texture2D>("Images/Shuttle/Body/bugBody");
                clonesBug[i].Cabin = Content.Load<Texture2D>("Images/Shuttle/Cabin/" + cabins[1]);
                clonesBug[i].Jet = Content.Load<Texture2D>("Images/Shuttle/Jet/" + jets[random.Next(3, 5)]);
            }
            for (int i = 0; i < clonesGamepad.Length; ++i)
            {
                clonesGamepad[i].Texture = Content.Load<Texture2D>("Images/Shuttle/Body/gamepadBody");
                clonesGamepad[i].Cabin = Content.Load<Texture2D>("Images/Shuttle/Cabin/" + cabins[random.Next(0,2) * 2]);
                clonesGamepad[i].Jet = Content.Load<Texture2D>("Images/Shuttle/Jet/doubleGreenJet");
            }
            for (int i = 0; i < clonesMassive.Length; ++i)
            {
                clonesMassive[i].Texture = Content.Load<Texture2D>("Images/Shuttle/Body/massiveBody");
                clonesMassive[i].Cabin = Content.Load<Texture2D>("Images/Shuttle/Cabin/" + cabins[random.Next(0, 2) == 0 ? 0 : cabins.Length - 1]);
                clonesMassive[i].Jet = Content.Load<Texture2D>("Images/Shuttle/Jet/" + jets[random.Next(0, 4)]);
            }
            for (int i = 0; i < clonesLunar.Length; ++i)
            {
                clonesLunar[i].Texture = Content.Load<Texture2D>("Images/Shuttle/Body/lunarBody");
                clonesLunar[i].Cabin = Content.Load<Texture2D>("Images/Shuttle/Cabin/" + cabins[random.Next(0, 2) * 2 + 1]);
                clonesLunar[i].Jet = Content.Load<Texture2D>("Images/Shuttle/Jet/" + jets[random.Next(0, 4)]);
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
            for (int i = 0; i < clonesGamepad.Length; ++i)
            {
                clonesGamepad[i].Update(gameTime, resolution);
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
            for (int i = 0; i < clonesGamepad.Length; ++i)
            {
                clonesGamepad[i].Draw(_spriteBatch);
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
            Debug.WriteLine(player.Position);

            _spriteBatch.End();
        }
    }
}
