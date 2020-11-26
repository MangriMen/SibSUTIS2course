using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using OOP_3S_Lab234.Entities;
using OOP_3S_Lab234.Mode;
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

        public static Vector2 resolution;
        public static Vector2 WindowResolution;

        Texture2D loadingScreen;

        public static Dictionary<string, Texture2D> projectilesTexture = new Dictionary<string, Texture2D>();
        public static Dictionary<string, Texture2D> shuttleTexture = new Dictionary<string, Texture2D>();
        public static Dictionary<string, Texture2D> jetTexture = new Dictionary<string, Texture2D>();
        public static Dictionary<string, Texture2D> cabinTexture = new Dictionary<string, Texture2D>();

        public static Dictionary<string, Texture2D> jetAnimations = new Dictionary<string, Texture2D>();
        public static Dictionary<string, Dictionary<string, Texture2D>> projectilesAnimation = new Dictionary<string, Dictionary<string, Texture2D>>();


        private GraphicsDeviceManager _graphics;
        private SpriteBatch _spriteBatch;
        public static Texture2D white;

        RenderTarget2D viewport;

        Waves waves;

        public ASpaceOutside()
        {
            _graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            IsMouseVisible = true;
        }

        protected override void Initialize()
        {
            // Hight graphics profile
            _graphics.GraphicsProfile = GraphicsProfile.HiDef;

            // Changing window title
            Window.Title = "A Space Outside!";

            // Screen mode
            _graphics.IsFullScreen = true;

            // Turn on the multisampling and set it level to 8
            _graphics.PreferMultiSampling = true;
            GraphicsDevice.PresentationParameters.MultiSampleCount = 8;

            _graphics.SynchronizeWithVerticalRetrace = true; // vsync
            IsFixedTimeStep = false; // Don't force equal timestep updates

            // Auto resolution on fullscreen
            if (_graphics.IsFullScreen)
            {
                WindowResolution.X = GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Width;
                WindowResolution.Y = GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Height;
            }
            else
            {
                WindowResolution.X = 1280;
                WindowResolution.Y = 720;
            }

            // Screen resolution
            _graphics.PreferredBackBufferWidth = (int)WindowResolution.X;
            _graphics.PreferredBackBufferHeight = (int)WindowResolution.Y;

            // Set the viewport resolution
            resolution = new Vector2(1920, 1080);

            // Apply changes for _graphics
            _graphics.ApplyChanges();

            // Setting rectangle of the viewport
            viewport = new RenderTarget2D(GraphicsDevice, (int)resolution.X, (int)resolution.Y);

            base.Initialize();
        }

        protected override void LoadContent()
        {
            _spriteBatch = new SpriteBatch(GraphicsDevice);

            // Load texture for game initialize logo
            loadingScreen = Content.Load<Texture2D>("Images/Backgrounds/bootloader");

            // Create thread for parallel loading and displaying logo
            Thread bgLoad = new Thread(new ThreadStart(LoadGame));
            bgLoad.IsBackground = true;
            bgLoad.Start();
        }

        protected void LoadGame()
        {
            // Loading texture for debug collision polygons
            white = Content.Load<Texture2D>("Images/Backgrounds/white");

            // Loading textures for shuttles
            shuttleTexture.Add("Bug", Content.Load<Texture2D>("Images/Shuttle/Body/bugBody"));
            shuttleTexture.Add("Bat", Content.Load<Texture2D>("Images/Shuttle/Body/batBody"));
            shuttleTexture.Add("Lunar", Content.Load<Texture2D>("Images/Shuttle/Body/lunarBody"));
            shuttleTexture.Add("Massive", Content.Load<Texture2D>("Images/Shuttle/Body/massiveBody"));

            // Loading textures for cabins
            cabinTexture.Add("brickCabin", Content.Load<Texture2D>("Images/Shuttle/Cabin/brickCabin"));
            cabinTexture.Add("conusCabin", Content.Load<Texture2D>("Images/Shuttle/Cabin/conusCabinDouble"));
            cabinTexture.Add("ovalCabin", Content.Load<Texture2D>("Images/Shuttle/Cabin/ovalCabin"));
            cabinTexture.Add("raindropCabin", Content.Load<Texture2D>("Images/Shuttle/Cabin/raindropDoubleCabin"));

            // Loading textures for jets
            jetTexture.Add("SlideBlue", Content.Load<Texture2D>("Images/Shuttle/Jet/slideBlueJet"));
            jetTexture.Add("SlideGreen", Content.Load<Texture2D>("Images/Shuttle/Jet/slideBlueJet"));
            jetTexture.Add("SlideOrange", Content.Load<Texture2D>("Images/Shuttle/Jet/slideBlueJet"));

            jetTexture.Add("SpeedBlue", Content.Load<Texture2D>("Images/Shuttle/Jet/speedBlueJet"));
            jetTexture.Add("SpeedGreen", Content.Load<Texture2D>("Images/Shuttle/Jet/speedBlueJet"));
            jetTexture.Add("SpeedOrange", Content.Load<Texture2D>("Images/Shuttle/Jet/speedBlueJet"));

            jetTexture.Add("WarpBlue", Content.Load<Texture2D>("Images/Shuttle/Jet/speedOrangeJet"));
            jetTexture.Add("WarpGreen", Content.Load<Texture2D>("Images/Shuttle/Jet/speedOrangeJet"));
            jetTexture.Add("WarpOrange", Content.Load<Texture2D>("Images/Shuttle/Jet/speedOrangeJet"));

            // Loading animations for jets
            jetAnimations.Add("slide", Content.Load<Texture2D>("Images/Particles/slideParticles"));
            jetAnimations.Add("speed", Content.Load<Texture2D>("Images/Particles/speedParticles"));
            jetAnimations.Add("warp", Content.Load<Texture2D>("Images/Particles/warpParticles"));

            // Loading textures for projectiles
            projectilesTexture.Add("rocket", Content.Load<Texture2D>("Images/Projectales/rocketProj"));
            projectilesTexture.Add("pixel", Content.Load<Texture2D>("Images/Backgrounds/white"));

            // Loading animations for projectiles
            projectilesAnimation.Add(
                "rocket",
                new Dictionary<string, Texture2D> {
                    ["Working"] = Content.Load<Texture2D>("Images/Projectales/rocketProj"),
                    ["Exploding"] = Content.Load<Texture2D>("Images/Particles/projectileExplosionPariclesBig")
                });

            // Initialize the waves mode
            waves = new Waves();

            // Loading MainMenu
            MainMenu.Load(Content, resolution);

            // Loading Mode: waves
            waves.Load(Content, resolution);

            // Creating timer for end loading for (fixed + loading) time
            Timer timer = new Timer(new TimerCallback(GameLoaded), null, 800, Timeout.Infinite);
        }


        private void GameLoaded(object obj)
        {
            // Setting the target fps
            TargetElapsedTime = TimeSpan.FromSeconds(1 / 80.0f); // FPS lock (80 per second for now)

            // Changing state to MainMenu
            State = GameState.MainMenu;
        }

        protected override void Update(GameTime gameTime)
        {
            // Switching by State
            switch (State)
            {
                case GameState.LoadingStart:
                    break;
                case GameState.MainMenu:
                    MainMenu.Update(gameTime, this);
                    break;
                case GameState.Gameplay:
                    waves.Update(gameTime);
                    break;
            }

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            // Set render target to the viewport
            _graphics.GraphicsDevice.SetRenderTarget(viewport);

            // Switching by State
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
                    waves.Draw(gameTime, _spriteBatch, _graphics);
                    break;
            }

            // Set render target to the null for display it in the window
            _graphics.GraphicsDevice.SetRenderTarget(null);

            // Rendering the viewport
            _spriteBatch.Begin();

            _spriteBatch.Draw(
                viewport,
                GraphicsDevice.Viewport.Bounds,
                Color.White
                );

            _spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
