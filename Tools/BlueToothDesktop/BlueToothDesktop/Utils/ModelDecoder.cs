using BlueToothDesktop.Enums;
using BlueToothDesktop.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BlueToothDesktop.Utils
{
    public class ModelDecoder
    {
        public static dynamic DecodeMessage(MessageTypeEnum msgType, byte[] msgBytes)
        {
            switch (msgType)
            {
                case MessageTypeEnum.VarList:
                    return VarTypeListModel.DecodeByteArray(msgBytes);
            }

            return null;
        }
    }
}
