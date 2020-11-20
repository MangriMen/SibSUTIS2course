using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using OOP_3S_Lab234.Entities;
using OOP_3S_Lab234.Models;
using OOP_3S_Lab234.ShipParts;
using OOP_3S_Lab234.Utils;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
using Color = Microsoft.Xna.Framework.Color;
using Vector2 = Microsoft.Xna.Framework.Vector2;

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

        Texture2D backgroundTexture;
        Texture2D loadingScreen;
        Player player;
        Enemy[] clones = new Enemy[9];

        public static Dictionary<string, Texture2D> projectilesTexture = new Dictionary<string, Texture2D>();
        public static Dictionary<string, Dictionary<string, Texture2D>> projectilesAnimation = new Dictionary<string, Dictionary<string, Texture2D>>();

        private GraphicsDeviceManager _graphics;
        private SpriteBatch _spriteBatch;
        Random random = new Random();
        Texture2D white;

        RenderTarget2D viewport;

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
            _graphics.IsFullScreen = true;

            _graphics.SynchronizeWithVerticalRetrace = false; // vsync
            IsFixedTimeStep = true; // Don't force equal timestep updates

            // Auto resolution on fullscreen
            if (_graphics.IsFullScreen)
            {
                resolution.X = GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Width;
                resolution.Y = GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Height;
            }
            else
            {
                resolution.X = 1280;
                resolution.Y = 720;
            }

            // Screen resolution
            _graphics.PreferredBackBufferWidth = (int)resolution.X;
            _graphics.PreferredBackBufferHeight = (int)resolution.Y;

            resolution = new Vector2(1920, 1080);

            _graphics.PreferMultiSampling = true;
            GraphicsDevice.PresentationParameters.MultiSampleCount = 8;

            _graphics.ApplyChanges();

            player = new Player(new Vector2(resolution.X / 2,
                resolution.Y / 2));

            viewport = new RenderTarget2D(GraphicsDevice, (int)resolution.X, (int)resolution.Y);

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
            MainMenu.Load(Content, GraphicsDevice);

            backgroundTexture = Content.Load<Texture2D>("Images/Backgrounds/background");

            player.Load(Content);

            white = Content.Load<Texture2D>("Images/Backgrounds/white");

            projectilesTexture.Add("rocket", Content.Load<Texture2D>("Images/Projectales/rocketProj"));
            projectilesTexture.Add("pixel", Content.Load<Texture2D>("Images/Backgrounds/white"));

            projectilesAnimation.Add(
                "rocket",
                new Dictionary<string, Texture2D> {
                    ["Working"] = Content.Load<Texture2D>("Images/Projectales/rocketProj"),
                    ["Exploding"] = Content.Load<Texture2D>("Images/Particles/projectileExplosionPariclesBig")
                });

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
            TargetElapsedTime = TimeSpan.FromSeconds(1 / 80.0f); // FPS lock (80 per second for now)

            State = GameState.MainMenu;
        }

        protected void CollisionCheck()
        {
            player.isDamaged = false;
            for (int i = 0; i < clones.Length; i++)
            {
                if (player.Collider.Intersects(clones[i].Collider))
                    player.isDamaged = true;

                foreach (var projectile in clones[i].projectiles)
                    if (projectile.IsExists && projectile.Collider.Intersects(player.Collider))
                    {
                        player.isDamaged = true;
                        projectile.Blow();
                    };
                foreach (var projectile in player.projectiles)
                    if (projectile.IsExists && projectile.Collider.Intersects(clones[i].Collider))
                    {
                        clones[i].isDamaged = true;
                        projectile.Blow();
                    };
            }
        }

        protected override void Update(GameTime gameTime)
        {
            //if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
            //    Exit();

            switch (State)
            {
                case GameState.LoadingStart:
                    break;
                case GameState.MainMenu:
                    MainMenu.Update(gameTime, this);
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
                    MainMenu.Draw(gameTime, _spriteBatch, GraphicsDevice);
                    break;
                case GameState.Gameplay:
                    DrawGameplay();
                    break;
            }
            base.Draw(gameTime);
        }

        protected void UpdateGameplay(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                State = GameState.MainMenu;

            player.Update(gameTime, resolution);

            for (int i = 0; i < clones.Length; i++)
            {
                clones[i].Update(gameTime, resolution);
            }

            CollisionCheck();
        }

        protected void DrawGameplay()
        {

            _graphics.GraphicsDevice.SetRenderTarget(viewport);
            _graphics.GraphicsDevice.Clear(Color.Black);
            _spriteBatch.Begin();

            _spriteBatch.Draw(
                backgroundTexture,
                GraphicsDevice.Viewport.Bounds,
                null,
                Color.White,
                0f,
                Vector2.Zero,
                SpriteEffects.None,
                0f
                );

            foreach (var clone in clones)
                clone.Draw(_spriteBatch);

            player.Draw(_spriteBatch);

            _spriteBatch.End();

            _graphics.GraphicsDevice.SetRenderTarget(null);

            _spriteBatch.Begin();

            _spriteBatch.Draw(
                viewport,
                GraphicsDevice.Viewport.Bounds,
                Color.White
                );
            
            _spriteBatch.End();
        }
    }
}
