using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BlueToothDesktop
{
    public interface WindowCallback
    {
        void AppendLog(string toAppend, bool newLine = true, bool timeStamp = true, bool scrollToEnd = true);
    }
}
