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
        public class Line
        {
            Vector2 start;
            Vector2 end;

            Vector2 startNR;
            Vector2 endNR;
            float angle;
            Vector2 origin;
            
            public float lenght;
            public Vector2 Start
            {
                get
                {
                    return start;
                }
                set
                {
                    start = value;
                    startNR = value;
                }
            }

            public Vector2 End
            {
                get
                {
                    return end;
                }
                set
                {
                    end = value;
                    endNR = value;
                }
            }

            public Vector2 Center
            {
                get
                {
                    return new Vector2(lenght, lenght) / 2;
                }
            }

            public Line (Vector2 start_, Vector2 end_)
            {
                Start = start_;
                End = end_;
                lenght = (float)Math.Sqrt(Math.Pow(end.X - start.X, 2) + Math.Pow(end.Y - start.Y, 2));
            }
            public void SetPosition(Vector2 position_)
            {
                End = position_ + (End - Start);
                Start = position_;
            }
            public void Move(Vector2 offset)
            {
                Start += offset;
                End += offset;
            }
            public void Draw(SpriteBatch _spriteBatch, Texture2D pixel)
            {
                for (int i = 0; i < lenght; i++)
                {
                    _spriteBatch.Draw(
                        pixel,
                        Start + (End - Start) * i/lenght,
                        Color.White
                        );
                }
            }
            public void Rotate(Vector2 origin_, float angle_)
            {
                start = RotatedRectangle.Rotate(origin_, startNR, angle_);
                end = RotatedRectangle.Rotate(origin_, endNR, angle_);
                angle = angle_;
                origin = origin_;
            }
        }
        public class PolygonCollider
        {
            public Line[] lines;
            Line[] linesLocal;
            Vector2 position_;
            public Vector2 Position 
            { 
                get
                {
                    return position_;
                }
                set
                {
                    position_ = value;
                    for (int i = 0; i < lines.Length; i++)
                    {
                        lines[i].SetPosition(value + linesLocal[i].Start - Pivot);
                    }
                } 
            }
            public Vector2 Pivot { get; set; }

            public Line[] Lines
            {
                get
                {
                    return lines;
                }
            }

            public PolygonCollider(Vector2[] points)
            {
                Init(points);
            }

            public void Init(Vector2[] points)
            {
                if (points == null) { return; }
                lines = new Line[points.Length];
                linesLocal = new Line[points.Length];

                for (int i = 0; i < points.Length - 1; i++)
                {
                    lines[i] = new Line(points[i], points[i + 1]);
                    linesLocal[i] = new Line(points[i], points[i + 1]);
                }
                lines[points.Length - 1] = new Line(points[points.Length - 1], points[0]);
                linesLocal[points.Length - 1] = new Line(points[points.Length - 1], points[0]);
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
                int i = 0;
                for (i = 0; i < lines.Length; i++)
                {
                    x0 = lines[i].Start.X;
                    y0 = lines[i].Start.Y;
                    x1 = lines[(i + 1) % lines.Length].Start.X;
                    y1 = lines[(i + 1) % lines.Length].Start.Y;
                    a = x0* y1 - x1* y0;
                    signedArea += a;
                    centroid.X += (x0 + x1)*a;
                    centroid.Y += (y0 + y1)*a;
                }

                signedArea *= 0.5f;
                centroid.X /= (6.0f * signedArea);
                centroid.Y /= (6.0f * signedArea);

                Pivot = centroid;
            }

            public static float UnderOrOverTheLine(Vector2 start, Vector2 end, Vector2 point)
            {
                return (end.X - start.X) * (point.Y - start.Y) - (end.Y - start.Y) * (point.X - start.X);
            }

            public bool Intersects(PolygonCollider collider)
            {
                bool isIntersect = false;

                int i;
                for (i = 0; i < lines.Length; i++)
                {
                    int atLeastOne = 0;
                    for (int j = 0; j < collider.lines.Length; j++)
                    {
                        if (UnderOrOverTheLine(lines[i].Start, lines[i].End, collider.lines[j].Start) > 0) { atLeastOne++; }
                    }
                    if (atLeastOne == 0)
                    {
                        break;
                    }
                }

                if (i == lines.Length) { isIntersect = true; }

                return isIntersect;
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
