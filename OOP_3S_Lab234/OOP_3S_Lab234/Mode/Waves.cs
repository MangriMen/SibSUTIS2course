using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using OOP_3S_Lab234.Control;
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
        Texture2D playerShieldTexture;
        GuiText gameoverText;
        GuiText wavesText;
        GuiText livesText;
        Player player;
        List<Enemy> clones;
        int wave;
        int attempts;
        float hitTime;
        float waveDelay;

        public Waves()
        {
            Init();
        }

        public void Init()
        {
            wave = 0;
            attempts = 3;
            hitTime = 0.0f;

            clones = new List<Enemy>();

            player = new Player(new Vector2(ASpaceOutside.resolution.X / 2,
                ASpaceOutside.resolution.Y / 2));
        }

        public void Load(ContentManager Content, Vector2 resolution)
        {
            backgroundTexture = Content.Load<Texture2D>("Images/Backgrounds/background");
            playerShieldTexture = Content.Load<Texture2D>("Images/Particles/shield");

            wavesText = new GuiText(Content.Load<Texture2D>("Images/Controls/guiElementRight"), Content.Load<SpriteFont>("Fonts/default"))
            {
                Position = new Vector2(resolution.X - 256, 0),
                Text = "Wave: 1",
            };

            livesText = new GuiText(Content.Load<Texture2D>("Images/Controls/guiElementLeft"), Content.Load<SpriteFont>("Fonts/default"))
            {
                Position = new Vector2(0, 0),
                Text = "Health: 3",
            };

            gameoverText = new GuiText(Content.Load<Texture2D>("Images/Backgrounds/gameover"), Content.Load<SpriteFont>("Fonts/default"))
            {
                Position = new Vector2(0, 0),
                Text = "",
                PenColour = Color.White
            };

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
                    {
                        if (hitTime > 2)
                        {
                            player.isDamaged = true;
                            attempts--;
                            hitTime = 1;
                        }
                    }

                    foreach (var projectile in clones[i].projectiles)
                        if (projectile.IsExists && projectile.Collider.Intersects(player.Collider))
                        {
                            if (hitTime > 2)
                            {
                                player.isDamaged = true;
                                attempts--;
                                hitTime = 1;
                            }
                            projectile.Blow();
                        };
                    foreach (var projectile in player.projectiles)
                    {
                        if (projectile.IsExists && projectile.Collider.Intersects(clones[i].Collider))
                        {
                            clones[i].isDamaged = true;
                            projectile.Blow();
                            clones[i].isExist = false;
                        };
                        foreach (var cloneProjectile in clones[i].projectiles)
                            if (projectile.IsExists && projectile.Collider.Intersects(cloneProjectile.Collider))
                            {
                                projectile.Blow();
                                cloneProjectile.Blow();
                            };
                    }
                }
            }
        }

        void SwitchWave()
        {
            Random random = new Random();
            hitTime = -1;
            float numberOfEnemy = (wave + 6) * ((random.Next(18, 22) / (float)10));

            player.Position = new Vector2((int)ASpaceOutside.resolution.X * 0.5f - 32, (int)ASpaceOutside.resolution.Y * 0.5f - 32);

            for (int i = 0; i < numberOfEnemy; i++)
            {
                if (!Convert.ToBoolean((wave + 1) % 3))
                {
                    clones.Add(new Enemy(
                        new Vector2(random.Next(65, (int)ASpaceOutside.resolution.X - 65),
                                    random.Next(65, (int)ASpaceOutside.resolution.Y - 65)),
                        ShuttleType.Bug,
                        JetType.Speed,
                        new Vector2(0.5f,0.5f),
                        false,
                        1000000.0f)
                    );
                }
                else
                {
                    if (i <= Math.Ceiling((numberOfEnemy * 4) / 10))
                    {
                        clones.Add(new Enemy(
                            new Vector2(random.Next((int)ASpaceOutside.resolution.X - 300, (int)ASpaceOutside.resolution.X - 65),
                                        random.Next(65, (int)ASpaceOutside.resolution.Y - 65)),
                            ShuttleType.Bat,
                            JetType.Warp,
                            Convert.ToBoolean(i & 1) ? new Vector2(-0.2f, 0.75f) : new Vector2(-0.2f, -0.75f),
                            false,
                            0.2f)
                            );
                    }
                    else
                    {
                        clones.Add(new Enemy(
                            new Vector2(random.Next(65, 300),
                                        random.Next(65, (int)ASpaceOutside.resolution.Y - 65)),
                            ShuttleType.Lunar,
                            JetType.Warp,
                            Convert.ToBoolean(i & 1) ? new Vector2(0.75f, -0.2f) : new Vector2(0.75f, 0.2f),
                            false,
                            0.2f)
                            );
                    }
                }
            }

            string[] cabins = new string[4] { "brickCabin", "conusCabinDouble", "ovalCabin", "raindropDoubleCabin" };

            for (int i = 0; i < clones.Count; i++)
            {
                clones[i].Load(cabins[2]);
            }
            wave++;
            gameoverText.Text = "Game Over!\nYou made it to the wave #" + wave + " !\nPress an R key to restart.";
        }

        override public void Update(GameTime gameTime)
        {
            bool isGameEnded = attempts <= 0 && Keyboard.GetState().IsKeyDown(Keys.R);
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
            {
                isGameEnded = true;
                ASpaceOutside.State = ASpaceOutside.GameState.MainMenu;
            }

            float delta = (float)gameTime.ElapsedGameTime.TotalSeconds;
            hitTime += delta;
            waveDelay += delta;


            bool exists = false;
            for (int i = 0; i < clones.Count; i++)
                if (clones[i].isExist)
                    exists = true;

            if (!exists || isGameEnded)
            {
                for (int i = 0; i < clones.Count; i++)
                    clones[i] = null;
                clones.Clear();
                if (isGameEnded)
                {
                    wave = 0;
                    attempts = 3;
                }
                SwitchWave();
                foreach(var playerProjToClear in player.projectiles)
                {
                    playerProjToClear.IsExists = false;
                }
                player.projectiles.Clear();
                waveDelay = 0;
                wavesText.Text = wavesText.Text.Substring(0, 6) + wave;
            }

            livesText.Text = livesText.Text.Substring(0, 8) + attempts;

            if (waveDelay > 1.0f || waveDelay < 0.007f)
            {
                if (attempts > 0) player.Update(gameTime, ASpaceOutside.resolution);

                for (int i = 0; i < clones.Count; i++)
                {
                    if (clones[i].isExist)
                        clones[i].Update(gameTime, ASpaceOutside.resolution);
                }

                CollisionCheck();
            }
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

            if (hitTime < 2) {
                _spriteBatch.Draw(
                    playerShieldTexture,
                    new Vector2(player.Position.X, player.Position.Y),
                    null,
                    Color.White,
                    player.RotateAngle,
                    new Vector2(playerShieldTexture.Width * 0.5f, playerShieldTexture.Height * 0.5f),
                    1.0f,
                    SpriteEffects.None,
                    0f
                    );
            }

            wavesText.Draw(gameTime, _spriteBatch);
            
            livesText.Draw(gameTime, _spriteBatch);
            
            if (attempts <= 0)
                gameoverText.Draw(gameTime, _spriteBatch);

            _spriteBatch.End();
        }
    }
}
