using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BlueToothDesktop.Serial;
using BlueToothDesktop;
using BlueToothDesktop.Models;
using BlueToothDesktop.Enums;

namespace BlueToothDummyClient.Serial
{
    class DummySerialHandler: SerialHandler
    {
        public DummySerialHandler(WindowCallback cb) : base(cb) { }

        public override void HandleIncomingMessageModel(MessageTypeEnum msgType, dynamic messageModel)
        {
            throw new NotImplementedException();
        }

        // productive functions
        public void SendVarTypes()
        {
            // message type
            var msgType = MessageTypeEnum.VarList;
            // get var types
            var VarTypes = VarTypeGenerator.GetVarTypes();
            // get bytes
            var bytes = VarTypes.GetByteArray();

            // send bytes
            var succ = SendBytes(msgType, bytes);

            if (succ) Callback.AppendLog("Sent var types");
            else Callback.AppendLog("Failed to send var types");
        }
    }
}
