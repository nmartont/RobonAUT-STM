using BlueToothDesktop.Serial;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BlueToothDesktop.Utils
{
    public class ByteArrayHandler
    {
        public static byte[] ConstructByteArray(List<byte[]> Bytes)
        {
            // get how many byes we need
            var byteNo = Bytes.Select(t => t.Length).Sum();

            // create byte array
            byte[] bytes = new byte[byteNo];

            int offset = 0;

            foreach (byte[] Byte in Bytes)
            {
                // swap endian if needed
                if (SerialHandler.SwapEndian) Array.Reverse(Byte);
                Buffer.BlockCopy(Byte, 0, bytes, offset, Byte.Length);
                offset += Byte.Length;
            }

            return bytes;
        }

        public static byte[] GetBytesFromArray(byte[] array, int offset, int length)
        {
            // create destination array
            byte[] dest = new byte[length];

            // copy array section
            Buffer.BlockCopy(array, offset, dest, 0, length);

            // swap endian if needed
            if (SerialHandler.SwapEndian) Array.Reverse(dest);

            return dest;
        }
    }
}
