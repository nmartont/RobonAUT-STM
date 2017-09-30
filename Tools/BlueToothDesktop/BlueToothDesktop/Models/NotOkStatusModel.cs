using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BlueToothDesktop.Models
{
    public class NotOkStatusModel
    {
        public string Text { get; set; }

        public byte[] GetByteArray()
        {
            // get string bytes
            byte[] str = Encoding.ASCII.GetBytes(Text);
            return str;
        }

        public static NotOkStatusModel DecodeByteArray(byte[] bytes)
        {
            // get text from bytes
            string str = Encoding.ASCII.GetString(bytes);
            return new NotOkStatusModel { Text = str };
        }
    }
}
