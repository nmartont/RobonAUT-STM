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
                case MessageTypeEnum.Error:
                    return ErrorStatusModel.DecodeByteArray(msgBytes);
                case MessageTypeEnum.Ok:
                    return OkStatusModel.DecodeByteArray(msgBytes);
                case MessageTypeEnum.NotOk:
                    return NotOkStatusModel.DecodeByteArray(msgBytes);
                default:
                    return null;
            }
        }
    }
}
