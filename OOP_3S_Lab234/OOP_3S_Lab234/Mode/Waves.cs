using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using OOP_3S_Lab234.Entities;
using OOP_3S_Lab234.ShipParts;
using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace OOP_3S_Lab234.Mode
{
    class Waves : GameMode
    {
        Texture2D backgroundTexture;

        Player player;
        List<Enemy> clones;

        int wave;

        public Waves()
        {
            Init();
        }

        public void Init()
        {
            wave = 0;

            clones = new List<Enemy>();

            player = new Player(new Vector2(ASpaceOutside.resolution.X / 2,
                ASpaceOutside.resolution.Y / 2));

            SwitchWave();
        }

        public void Load(ContentManager Content)
        {
            backgroundTexture = Content.Load<Texture2D>("Images/Backgrounds/background");

            player.Load(Content);

            string[] cabins = new string[4] { "brickCabin", "conusCabinDouble", "ovalCabin", "raindropDoubleCabin" };

            for (int i = 0; i < clones.Count; i++)
            {
                clones[i].Load(cabins[2]);
            }
        }

        protected void CollisionCheck()
        {
            player.isDamaged = false;
            for (int i = 0; i < clones.Count; i++)
            {
                if (clones[i].isExist)
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
                            clones[i].isExist = false;
                        };
                }
            }
        }

        void SwitchWave()
        {
            Random random = new Random();

            for (int i = 0; i < (wave + 6) * ((random.Next(18, 22) / (float)10)); i++)
            {
                if (i >= 0 && i < 4)
                    clones.Add(new Enemy(
                        new Vector2(random.Next((int)ASpaceOutside.resolution.X - 300, (int)ASpaceOutside.resolution.X - 100),
                                    random.Next(65, (int)ASpaceOutside.resolution.Y - 100)),
                        ShuttleType.Bat,
                        JetType.Warp)
                        );
                if (i >= 4 && i < 7)
                    clones.Add(new Enemy(
                        new Vector2(random.Next(65, (int)ASpaceOutside.resolution.X - 100),
                                    random.Next(65, (int)ASpaceOutside.resolution.Y - 100)),
                        ShuttleType.Bug,
                        JetType.Speed)
                        );
                if (i >= 7)
                    clones.Add(new Enemy(
                        new Vector2(random.Next(65, 350),
                                    random.Next((int)ASpaceOutside.resolution.Y / 2, (int)ASpaceOutside.resolution.Y - 65)),
                        ShuttleType.Lunar,
                        JetType.Warp)
                        );
            }

            string[] cabins = new string[4] { "brickCabin", "conusCabinDouble", "ovalCabin", "raindropDoubleCabin" };

            for (int i = 0; i < clones.Count; i++)
            {
                clones[i].Load(cabins[2]);
            }
            wave++;
        }

        override public void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                ASpaceOutside.State = ASpaceOutside.GameState.MainMenu;

            bool exists = false;
            for (int i = 0; i < clones.Count; i++)
                if (clones[i].isExist)
                    exists = true;

            if (!exists)
            {
                for (int i = 0; i < clones.Count; i++)
                        clones[i] = null;
                clones.Clear();
                SwitchWave();
                Debug.WriteLine(clones.Count);
            }

            player.Update(gameTime, ASpaceOutside.resolution);

            for (int i = 0; i < clones.Count; i++)
            {
                if (clones[i].isExist)
                    clones[i].Update(gameTime, ASpaceOutside.resolution);
            }

            CollisionCheck();
        }

        override public void Draw(GameTime gameTime, SpriteBatch _spriteBatch, GraphicsDeviceManager _graphics)
        {
            _graphics.GraphicsDevice.Clear(Color.Black);
            _spriteBatch.Begin();

            _spriteBatch.Draw(
                backgroundTexture,
                _graphics.GraphicsDevice.Viewport.Bounds,
                null,
                Color.White,
                0f,
                Vector2.Zero,
                SpriteEffects.None,
                0f
                );

            foreach (var clone in clones)
                if (clone.isExist)
                    clone.Draw(_spriteBatch);

            player.Draw(_spriteBatch);

            _spriteBatch.End();
        }
    }
}
