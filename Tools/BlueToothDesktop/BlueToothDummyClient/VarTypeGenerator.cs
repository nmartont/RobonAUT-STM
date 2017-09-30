using BlueToothDesktop.Enums;
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
        public static VarTypeListModel GetVarTypes()
        {
            VarTypeListModel VarTypesList = new VarTypeListModel();

            VarTypesList.VarTypes.Add(new VarTypeModel
            {
                Name = "DummyVar",
                VarType = VarTypeEnum.uint16
            });

            VarTypesList.VarTypes.Add(new VarTypeModel
            {
                Name = "AnotherVar",
                VarType = VarTypeEnum.uint8
            });

            return VarTypesList;
        }
    }
}
