using System;
using System.Collections.Generic;
using System.Text;

namespace OOP_3S_Lab234.ShipParts
{

    class SlideJet : Jet
    {
        protected float Speed { get; set; }

        void Jet.boost()
        {
            throw new NotImplementedException();
        }
    }
}
