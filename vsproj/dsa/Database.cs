using System;
using System.Diagnostics; /* contains the Debug.Assert method. */

namespace dsa
{
    class Database
    {
        public string Name { get; }
        public UInt64 ID { get; }

        public Database(string Name, UInt64 ID)
        {
            this.Name = Name;
            this.ID = ID;
        }

        // why is this declared override?
        public override string ToString()
        {
            // what does base do?
            // return Name;
            return base.ToString();
        }

    }

    /* Testing Class features in C#
     */

    public class TestDatabase
    {
        public void Test()
        {
            string Name = "Info Database";
            UInt64 ID = 12345;
            Database mydb = new Database(Name, ID);
            Debug.Assert(mydb.Name == Name);
            Debug.Assert(mydb.ID == ID);
        }
    }
}
