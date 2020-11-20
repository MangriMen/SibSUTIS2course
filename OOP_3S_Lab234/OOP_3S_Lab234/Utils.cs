using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;
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
        public class PolygonCollider
        {
            Vector2[] pointsO_;
            public Vector2[] points_;
            Vector2 pivot_;
            Vector2 position_;
            public Vector2 Pivot
            {
                get
                {
                    return pivot_;
                }
            }
            public Vector2 Position
            {
                get
                {
                    return position_;
                }
                set
                {
                    for (int i = 0; i < points_.Length; i++)
                    {
                        points_[i] = value - (pointsO_[i] - pivot_);
                    }
                    position_ = value;
                }
            }
            public PolygonCollider(Vector2[] points)
            {
                Init(points);
            }
            public void Init(Vector2[] points)
            {
                pointsO_ = (Vector2[])points.Clone();
                points_ = (Vector2[])points.Clone();

                compute2DPolygonCentroid();
            }
            public void compute2DPolygonCentroid()
            {
                Vector2 centroid = Vector2.Zero;
                float signedArea = 0f;
                float x0 = 0f; // Current vertex X
                float y0 = 0f; // Current vertex Y
                float x1 = 0f; // Next vertex X
                float y1 = 0f; // Next vertex Y
                float a = 0f;  // Partial signed area

                // For all vertices
                for (int i = 0; i < points_.Length; i++)
                {
                    x0 = points_[i].X;
                    y0 = points_[i].Y;
                    x1 = points_[(i + 1) % points_.Length].X;
                    y1 = points_[(i + 1) % points_.Length].Y;
                    a = x0 * y1 - x1 * y0;
                    signedArea += a;
                    centroid.X += (x0 + x1) * a;
                    centroid.Y += (y0 + y1) * a;
                }

                signedArea *= 0.5f;
                centroid.X /= (6.0f * signedArea);
                centroid.Y /= (6.0f * signedArea);

                pivot_ = centroid - points_[0];

                pivot_.X = (float)Math.Abs(pivot_.X);
                pivot_.Y = (float)Math.Abs(pivot_.Y);
            }
            public void Draw(SpriteBatch _spriteBatch, Texture2D pixel)
            {
                for (int i = 0; i < points_.Length; i++)
                {
                    float lenght = (float)Math.Sqrt(
                        Math.Pow(points_[(i + 1) % points_.Length].X - points_[i].X, 2) +
                        Math.Pow(points_[(i + 1) % points_.Length].Y - points_[i].Y, 2)
                        );
                    for (int h = 0; h < lenght; h++)
                    {
                        _spriteBatch.Draw(
                            pixel,
                            points_[i] + (points_[(i+1) % points_.Length] - points_[i]) * h / lenght,
                            Color.White
                            );
                    }

                }

            }

            public void Rotate(Vector2 origin_, float angle)
            {
                for (int i = 0; i < points_.Length; i++)
                {
                    points_[i] = Point.Rotate(origin_, points_[i], angle);
                }
            }

            public static float UnderOrOverTheLine(Vector2 start, Vector2 end, Vector2 point)
            {
                return ((end.X - start.X) * (point.Y - start.Y) - (end.Y - start.Y) * (point.X - start.X));
            }

            public bool Intersects(PolygonCollider collider)
            {
                int j = 0;

                for (int i = 0; i < points_.Length; i++)
                {
                    for (j = 0; j < collider.points_.Length; j++)
                        if (UnderOrOverTheLine(collider.points_[j], collider.points_[(j + 1) % collider.points_.Length], points_[i]) < 0) break;

                    if (j == collider.points_.Length) break;
                }

                return (j == collider.points_.Length);
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
        public static class Point {
            public static Vector2 Rotate(Vector2 origin, Vector2 point, float angle) {
                float cosFromAngle = (float)Math.Cos(angle);
                float sinFromAngle = (float)Math.Sin(angle);

                float ox = origin.X;
                float oy = origin.Y;
                float px = point.X;
                float py = point.Y;

                float qx = ox + cosFromAngle * (px - ox) - sinFromAngle * (py - oy);
                float qy = oy + sinFromAngle * (px - ox) + cosFromAngle * (py - oy);

                return new Vector2(qx, qy);
            }
        }
    }
}
