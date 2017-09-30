using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BlueToothDesktop.Models
{
    public enum MessageTypeEnum : byte
    {
        Error = 0,
        Ok,
        NotOk,
        StatusRequest,
        VarList,
        VarValues,
        SetSpeed,
        SetSteering,
        SetStateMachine
    }
}
