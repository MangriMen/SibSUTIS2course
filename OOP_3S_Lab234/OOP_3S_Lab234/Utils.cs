using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;
using System.Collections.Generic;
using System.Text;

namespace OOP_3S_Lab234
{
    namespace Utils
    {
        public static class Kb
        {
            public static bool getDown(Keys key)
            {
                return Keyboard.GetState().IsKeyDown(key);
            }
        }
        public static class Sprite
        {
            public static void DrawCollider(SpriteBatch _spriteBatch, Texture2D ColliderTexture, Rectangle Collider)
            {
                for (int corner = 0; corner < 4; corner++)
                {
                    switch (corner)
                    {
                        case 0:
                            _spriteBatch.Draw(
                                ColliderTexture,
                                new Rectangle(Collider.X, Collider.Y, Collider.Width, 1),
                                null,
                                Color.White,
                                0f,
                                Vector2.Zero,
                                SpriteEffects.None,
                                0f
                                );
                            break;
                        case 1:
                            _spriteBatch.Draw(
                                ColliderTexture,
                                new Rectangle(Collider.X + Collider.Width, Collider.Y, 1, Collider.Height),
                                null,
                                Color.White,
                                0f,
                                Vector2.Zero,
                                SpriteEffects.None,
                                0f
                                );
                            break;
                        case 2:
                            _spriteBatch.Draw(
                                ColliderTexture,
                                new Rectangle(Collider.X, Collider.Y + Collider.Height, Collider.Width, 1),
                                null,
                                Color.White,
                                0f,
                                Vector2.Zero,
                                SpriteEffects.None,
                                0f
                                );
                            break;
                        case 3:
                            _spriteBatch.Draw(
                                ColliderTexture,
                                new Rectangle(Collider.X, Collider.Y, 1, Collider.Height),
                                null,
                                Color.White,
                                0f,
                                Vector2.Zero,
                                SpriteEffects.None,
                                0f
                                );
                            break;
                    };
                }
            }
        }
    }
}
