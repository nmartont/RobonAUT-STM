using BlueToothDesktop.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BlueToothDummyClient
{
    public class VarTypeGenerator
    {
        public static List<VarTypeModel> GetVarTypes()
        {
            List<VarTypeModel> VarTypes = new List<VarTypeModel>();

            VarTypes.Add(new VarTypeModel
            {
                Name = "DummyVar",
                VarType = VarTypeEnum.uint16
            });

            VarTypes.Add(new VarTypeModel
            {
                Name = "AnotherVar",
                VarType = VarTypeEnum.uint8
            });

            return VarTypes;
        }
    }
}
