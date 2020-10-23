using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;
using System.Threading;
using OOP_3S_Lab234.Entities;
using Color = Microsoft.Xna.Framework.Color;
using Vector2 = Microsoft.Xna.Framework.Vector2;
using OOP_3S_Lab234.ShipParts;
using System.Diagnostics;

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
        Enemy[] clones = new Enemy[9];
 
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

            _graphics.SynchronizeWithVerticalRetrace = true; // vsync
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

            for (int i = 0; i < clones.Length; i++)
            {
                if (i >= 0 && i < 4)
                    clones[i] = new Enemy(
                        new Vector2(random.Next((int)resolution.X - 300, (int)resolution.X - 100),
                                    random.Next(65, (int)resolution.Y - 100)),
                        ShuttleType.Bat,
                        JetType.Warp);
                if (i >= 4 && i < 7)
                    clones[i] = new Enemy(
                        new Vector2(random.Next(65, (int)resolution.X - 100),
                                    random.Next(65, (int)resolution.Y - 100)),
                        ShuttleType.Bug,
                        JetType.Speed);
                if (i >= 7 && i < clones.Length)
                    clones[i] = new Enemy(
                        new Vector2(random.Next(65, 350),
                                    random.Next((int)resolution.Y / 2, (int)resolution.Y - 65)),
                        ShuttleType.Lunar,
                        JetType.Warp);
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

            for (int i = 0; i < clones.Length; i++)
            {
                clones[i].Load(Content, cabins[2]);
            }

            Timer timer = new Timer(new TimerCallback(GameLoaded), null, 800, Timeout.Infinite);
        }


        private void GameLoaded(object obj)
        {
            State = GameState.MainMenu;
        }

        protected void CollisionCheck()
        {
            player.isDamaged = false;
            for (int i = 0; i < clones.Length; i++)
            {
                if (clones[i].HitBox.Intersects(player.HitBox))
                {
                    player.isDamaged = true;
                }
            }
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
           
            for (int i = 0; i < clones.Length; i++)
            {
                clones[i].Update(gameTime, resolution);
            }

            CollisionCheck();
        }
        protected void DrawGameplay()
        {
            _spriteBatch.Begin();
            _spriteBatch.Draw(backgoundTexture, GraphicsDevice.Viewport.Bounds, Color.White);

            for (int i = 0; i < clones.Length; i++)
            {
                clones[i].Draw(_spriteBatch);
            }

            player.Draw(_spriteBatch);

            _spriteBatch.End();
        }
    }
}
