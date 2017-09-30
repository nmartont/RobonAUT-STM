using BlueToothDesktop.Enums;
using BlueToothDesktop.Models;
using BlueToothDesktop.Utils;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace BlueToothDesktop.Serial
{
    public abstract class SerialHandler : INotifyPropertyChanged
    {
        public WindowCallback Callback;
        private SerialPort port;
        private static bool desiredLittleEndian = true;
        private static bool converterLittleEndian = BitConverter.IsLittleEndian;
        private char newLine = '\n';

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
        public static bool SwapEndian
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

            // check if there are multiple messages in the same buffer


            // handle the bytes
            HandleReceivedBytes(buffer);
        }

        public void HandleReceivedBytes(byte[] buffer)
        {
            // check message type first
            MessageTypeEnum MsgType = MessageTypeEnum.Error;

            try
            {
                MsgType = (MessageTypeEnum)buffer[0];
            }
            catch (Exception ex)
            {
                Callback.AppendLog("Error while parsing message type:\n" + ex.Message);
                return;
            }

            // decode message into c# model
            byte[] msgBytes = new byte[buffer.Length - 1];
            Buffer.BlockCopy(buffer, 1, msgBytes, 0, buffer.Length - 1);
            dynamic MessageModel = ModelDecoder.DecodeMessage(MsgType, msgBytes);

            if (MessageModel == null)
            {
                Callback.AppendLog("Unknown message type, cannot decode.");
                return;
            }

            // handle message in implementation
            HandleIncomingMessageModel(MsgType, MessageModel);
        }

        public abstract void HandleIncomingMessageModel(MessageTypeEnum msgType, dynamic messageModel);
        
        public bool SendBytes(byte[] bytes)
        {
            // add new line character
            //byte[] b = new byte[bytes.Length + 1];
            //b[b.Length - 1] = Convert.ToByte(newLine);
            //Buffer.BlockCopy(bytes, 0, b, 0, bytes.Length);

            try
            {
                // send bytes to port
                port.Write(bytes, 0, bytes.Length);
                return true;
            }
            catch (Exception ex)
            {
                Callback.AppendLog("Error while sending bytes:\n" + ex.Message);
                return false;
            }
        }

        public bool SendBytes(MessageTypeEnum msgType, byte[] bytes)
        {
            // concat bytes
            byte[] b = new byte[bytes.Length + 1];
            b[0] = (byte)msgType;
            Buffer.BlockCopy(bytes, 0, b, 1, bytes.Length);

            // send message
            return SendBytes(b);
        }
    }
}
