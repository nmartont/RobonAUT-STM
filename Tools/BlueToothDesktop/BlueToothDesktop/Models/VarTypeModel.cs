using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BlueToothDesktop.Models
{
    public class VarTypeModel
    {
        public string Name { get; set; }
        public VarTypeEnum VarType { get; set; }
        public List<byte[]> ByteList {
            get{
                List<byte[]> bList = new List<byte[]>();

                // get string bytes
                byte[] str = Encoding.ASCII.GetBytes(Name);

                // get length
                byte len = Convert.ToByte(str.Length);

                // create byte array list
                bList.Add(new byte[] { len });
                bList.Add(str);
                bList.Add(new byte[] { (byte)VarType });
                
                return  bList;
            }
        }
    }
}
