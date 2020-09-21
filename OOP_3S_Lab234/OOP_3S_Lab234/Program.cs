using System;

namespace OOP_3S_Lab234
{
    public static class Program
    {
        [STAThread]
        static void Main()
        {
            using (var game = new ASpaceOutside())
                game.Run();
        }
    }
}
