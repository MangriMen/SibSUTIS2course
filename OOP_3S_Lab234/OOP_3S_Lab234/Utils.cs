using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks.Dataflow;
using System.Threading.Tasks.Sources;

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
        public class Kvadr
        {
            Point[] corners = new Point[4];
            Vector2[] points;
            int els = 0;
            public Kvadr(Point[] corners_)
            {
                corners = corners_;
                points = new Vector2[500];
            }

            public void Build(Point[] corners_ = null)
            {
                if (corners_ != null)
                {
                    corners = corners_;
                }
                int i = 0;
                int k = 1;
                while (i < 500)
                {
                    points[i] = corners[0].ToVector2() + (corners[1].ToVector2() - corners[0].ToVector2()) * k/100;
                    if (points[i] == corners[1].ToVector2())
                    {
                        break;
                    }
                    k++;
                    i++;
                }
                k = 1;
                while (i < 500)
                {
                    points[i] = corners[1].ToVector2() + (corners[2].ToVector2() - corners[1].ToVector2()) * k / 100;
                    if (points[i] == corners[2].ToVector2())
                    {
                        break;
                    }
                    k++;
                    i++;
                }
                k = 1;
                while (i < 500)
                {
                    points[i] = corners[2].ToVector2() + (corners[3].ToVector2() - corners[2].ToVector2()) * k / 100;
                    if (points[i] == corners[3].ToVector2())
                    {
                        break;
                    }
                    k++;
                    i++;
                }
                k = 1;
                while (i < 500)
                {
                    points[i] = corners[3].ToVector2() + (corners[0].ToVector2() - corners[3].ToVector2()) * k / 100;
                    if (points[i] == corners[0].ToVector2())
                    {
                        break;
                    }
                    k++;
                    i++;
                }
                els = i;
            }
            public void Draw(SpriteBatch _spriteBatch, Texture2D white)
            {
                for (int i = 0; i < els; i++)
                    _spriteBatch.Draw(
                        white,
                        points[i],
                        Color.White
                        );
            }
        }
        public static class RotatedRectangle {
            public static Vector2 Rotate(Vector2 origin, Vector2 point, float angle) {
                float ox = origin.X;
                float oy = origin.Y;
                float px = point.X;
                float py = point.Y;

                float qx = ox + (float)Math.Cos(angle) * (px - ox) - (float)Math.Sin(angle) * (py - oy);
                float qy = oy + (float)Math.Sin(angle) * (px - ox) + (float)Math.Cos(angle) * (py - oy);
                return new Vector2(qx, qy);
            }
            public static Vector2 Smallest(Point[] corners)
            {
                Vector2 smallest = Vector2.Zero;
                Vector2 sm1;
                Vector2 sm2;

                sm1.X = Math.Min(corners[0].X, corners[1].X);
                sm1.Y = Math.Min(corners[0].Y, corners[1].Y);

                sm2.X = Math.Min(corners[2].X, corners[3].X);
                sm2.Y = Math.Min(corners[2].Y, corners[3].Y);

                smallest.X = Math.Min(sm1.X, sm2.X);
                smallest.Y = Math.Min(sm1.Y, sm2.Y);

                return smallest;
            }
            public static Vector2 Biggest(Point[] corners)
            {
                Vector2 biggest = Vector2.Zero;
                Vector2 bg1;
                Vector2 bg2;

                bg1.X = Math.Max(corners[0].X, corners[1].X);
                bg1.Y = Math.Max(corners[0].Y, corners[1].Y);

                bg2.X = Math.Max(corners[2].X, corners[3].X);
                bg2.Y = Math.Max(corners[2].Y, corners[3].Y);

                biggest.X = Math.Max(bg1.X, bg2.X);
                biggest.Y = Math.Max(bg1.Y, bg2.Y);

                return biggest;
            }
        }
    }
}
