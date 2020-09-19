using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace OOP_3S_Lab234
{
    public class Game1 : Game
    {
        Vector2 resolution;
        Texture2D backgoundTexture;
        Mini_Shuttle test;

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
            // TODO: Add your initialization logic here

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

            _graphics.ApplyChanges();

            test = new Mini_Shuttle(new Vector2(_graphics.PreferredBackBufferWidth / 2, 
                _graphics.PreferredBackBufferHeight / 2));

            base.Initialize();
        }

        protected override void LoadContent()
        {
            _spriteBatch = new SpriteBatch(GraphicsDevice);

            // TODO: use this.Content to load your game content here
            backgoundTexture = Content.Load<Texture2D>("Images/Backgrounds/background");

            test.Texture = Content.Load<Texture2D>("Images/Shuttle/Body/massiveBody");
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                Exit();

            // TODO: Add your update logic here
            test.Update(gameTime, resolution);

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {

            // TODO: Add your drawing code here
            _spriteBatch.Begin();
            _spriteBatch.Draw(backgoundTexture, GraphicsDevice.Viewport.Bounds, Color.White);
            _spriteBatch.Draw(test.Texture,
                test.Coords,
                null,
                Color.White,
                test.RotateAngle,
                new Vector2(test.Texture.Width / 2, test.Texture.Height / 2),
                Vector2.One,
                SpriteEffects.None,
                0f);
            _spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
