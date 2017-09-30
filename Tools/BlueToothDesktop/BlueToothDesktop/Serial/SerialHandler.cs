using BlueToothDesktop.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BlueToothDesktop.Serial
{
    public class SerialHandler : INotifyPropertyChanged
    {
        public WindowCallback Callback;
        private SerialPort port;
        private bool desiredLittleEndian = true;
        private bool converterLittleEndian = BitConverter.IsLittleEndian;

        public SerialHandler(WindowCallback cb)
        {
            Callback = cb;
        }

        // event handling for binding
        protected void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            PropertyChanged?.Invoke(this, e);
        }

        protected void OnPropertyChanged(string propertyName)
        {
            OnPropertyChanged(new PropertyChangedEventArgs(propertyName));
        }

        public event PropertyChangedEventHandler PropertyChanged;

        // properties of being connected
        public bool _is_connected = false;
        public bool NotConnected
        {
            get { return !_is_connected; }
            set
            {
                OnPropertyChanged("NotConnected");
            }
        }
        public bool IsConnected
        {
            get { return _is_connected; }
            set
            {
                _is_connected = value;
                NotConnected = !value; // just to trigger event
                OnPropertyChanged("IsConnected");
            }
        }

        // property for endian swap
        public bool SwapEndian
        {
            get { return desiredLittleEndian != converterLittleEndian; }
        }

        // Connection handling functions
        public void Connect(string portName)
        {

            port = new SerialPort(portName, 9600, Parity.None, 8, StopBits.One);
            port.WriteTimeout = 1000;

            try
            {
                SerialPortProgram();
                IsConnected = true;
                Callback.AppendLog("Successfully connected to port " + portName);
            }
            catch (Exception ex)
            {
                Callback.AppendLog("Error while connecting to port " + portName + ":\n" + ex.Message);
            }
        }

        public void Disconnect()
        {
            try
            {
                port.Close();
                IsConnected = false;
                Callback.AppendLog("Successfully disconnected from port " + port.PortName);
            }
            catch (Exception ex)
            {
                Callback.AppendLog("Error while disconnecting from port:\n" + ex.Message);
            }
        }

        // COM port handling
        private void SerialPortProgram()
        {
            // Attach a method to be called when there
            // is data waiting in the port's buffer
            port.DataReceived += new
              SerialDataReceivedEventHandler(port_DataReceived);

            // Begin communications
            port.Open();
            // Empty buffer so stuck data is discarded
            port.DiscardInBuffer();
        }

        // COM message callback
        private void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int bytes = port.BytesToRead;
            byte[] buffer = new byte[bytes];
            port.Read(buffer, 0, bytes);
        }
        
        // helpers
        public void SendBytes(List<byte[]> Bytes)
        {
            // construct byte array
            byte[] bytes = ConstructByteArray(Bytes);

            // send bytes to port
            port.Write(bytes, 0, bytes.Length);
        }

        public byte[] ConstructByteArray(List<byte[]> Bytes)
        {
            // get how many byes we need
            var byteNo = Bytes.Select(t => t.Length).Sum();
            
            // create byte array
            byte[] bytes = new byte[byteNo];

            int offset = 0;
            
            foreach (byte[] Byte in Bytes)
            {
                // swap endian
                if (SwapEndian) Array.Reverse(Byte);
                Buffer.BlockCopy(Byte, 0, bytes, offset, Byte.Length);
                offset += Byte.Length;
            }

            return bytes;
        }
    }
}
