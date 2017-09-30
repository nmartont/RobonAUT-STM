using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BlueToothDesktop.Serial;
using BlueToothDesktop;
using BlueToothDesktop.Models;

namespace BlueToothDummyClient.Serial
{
    class DummySerialHandler: SerialHandler
    {
        public DummySerialHandler(WindowCallback cb) : base(cb) { }

        // productive functions
        public void SendVarTypes()
        {
            // message type
            byte msgType = (byte)MessageTypeEnum.VarList;
            // get var types
            var VarTypes = VarTypeGenerator.GetVarTypes();
            // get byte list
            List<byte[]> Bytes = new List<byte[]>();
            Bytes.Add(new byte[] { msgType });
            foreach (VarTypeModel Var in VarTypes)
            {
                Bytes = Bytes.Concat(Var.ByteList).ToList();
            }

            // send bytes
            SendBytes(Bytes);

            Callback.AppendLog("Sent var types");
        }
    }
}
