using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace BlueToothDummyClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private string[] PortNames;
        private SerialPort port;

        public MainWindow()
        {
            InitializeComponent();
            RefreshPortDropDown();
            dropdownPorts.SelectedIndex = PortNames.Length - 2;

            AppendLog("LST BlueTooth Dummy Client ready...");
        }

        private void refreshPortsBtn_Click(object sender, RoutedEventArgs e)
        {
            RefreshPortDropDown();
        }

        private void buttonConnect_Click(object sender, RoutedEventArgs e)
        {
            connectToPort(dropdownPorts.Text);
        }

        private void connectToPort(string portName)
        {
            port = new SerialPort(portName,
            9600, Parity.None, 8, StopBits.One);

            try
            {
                SerialPortProgram();
                AppendLog("Successfully connected to port " + portName);
            }
            catch (Exception ex)
            {
                AppendLog("Error while connecting to port " + portName + ":\n" + ex.Message);
            }
        }

        private void buttondisconnect_Click(object sender, RoutedEventArgs e)
        {
            disconnectFromPort();
        }

        private void disconnectFromPort()
        {
            try
            {
                port.Close();
                AppendLog("Successfully disconnected from port " + port.PortName);
            }
            catch (Exception ex)
            {
                AppendLog("Error while disconnecting from port:\n" + ex.Message);
            }
        }

        private void varTypesBtn_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                sendVarTypes();
                AppendLog("Sent var types");
            }
            catch (Exception ex)
            {
                AppendLog("Error while sending var types:\n" + ex.Message);
            }
        }

        private void sendVarTypes()
        {
            byte b = 5;
            byte[] bytes = new byte[1];
            bytes[0] = b;
            port.Write(bytes, 0, bytes.Length);
        }

        private void sendVarsBtn_Click(object sender, RoutedEventArgs e)
        {

        }

        private void sendVarStreamBtn_Click(object sender, RoutedEventArgs e)
        {

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

        private void port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int bytes = port.BytesToRead;
            byte[] buffer = new byte[bytes];
            port.Read(buffer, 0, bytes);
        }

        // helpers
        private void RefreshPortDropDown()
        {
            // get COM ports, set it to the dropdown
            PortNames = System.IO.Ports.SerialPort.GetPortNames();
            dropdownPorts.ItemsSource = PortNames;
        }

        private void AppendLog(string toAppend, bool newLine=true, bool timeStamp=true, bool scrollToEnd=true)
        {
            Application.Current.Dispatcher.Invoke(new Action(() => {
                string textNewLine = "";
                string textTimeStamp = "";
                if (newLine) textNewLine = "\n";
                if (timeStamp) textTimeStamp = DateTime.Now.ToString("HH:mm:ss.f", CultureInfo.InvariantCulture) + ": ";
                textBox.AppendText(textTimeStamp + toAppend + textNewLine);
                if (scrollToEnd) textBox.ScrollToEnd();
            }));
        }
    }
}
