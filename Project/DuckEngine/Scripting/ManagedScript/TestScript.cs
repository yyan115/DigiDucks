using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ManagedScript
{
    internal class TestScript : ScriptAPI.Script
    {
        public override void Update()
        {
            Console.WriteLine("Test!");
        }
    }
}
