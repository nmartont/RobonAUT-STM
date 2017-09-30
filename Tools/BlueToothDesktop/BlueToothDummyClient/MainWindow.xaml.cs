using BlueToothDesktop;
using BlueToothDesktop.Models;
using BlueToothDummyClient;
using BlueToothDummyClient.Serial;
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
    public partial class MainWindow : Window, WindowCallback
    {
        private string[] PortNames;
        private DummySerialHandler SerHandler;

        public MainWindow()
        {
            InitializeComponent();
            RefreshPortDropDown();
            dropdownPorts.SelectedIndex = PortNames.Length - 2;
            
            AppendLog("LST BlueTooth Dummy Client ready...");
            
            // create SerialHandler
            SerHandler = new DummySerialHandler(this);
            SetBindings();
        }

        private void SetBindings()
        {
            // set bindings to UI elements
            Binding NotConnectedBinding = new Binding();
            NotConnectedBinding.Source = SerHandler;
            NotConnectedBinding.Path = new PropertyPath("NotConnected");
            NotConnectedBinding.Mode = BindingMode.OneWay;
            NotConnectedBinding.UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged;

            Binding ConnectedBinding = new Binding();
            ConnectedBinding.Source = SerHandler;
            ConnectedBinding.Path = new PropertyPath("IsConnected");
            ConnectedBinding.Mode = BindingMode.OneWay;
            ConnectedBinding.UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged;

            dropdownPorts.SetBinding(IsEnabledProperty, NotConnectedBinding);
            refreshPortsBtn.SetBinding(IsEnabledProperty, NotConnectedBinding);
            buttonConnect.SetBinding(IsEnabledProperty, NotConnectedBinding);
            buttonDisconnect.SetBinding(IsEnabledProperty, ConnectedBinding);
            sendVarsBtn.SetBinding(IsEnabledProperty, ConnectedBinding);
            sendVarStreamBtn.SetBinding(IsEnabledProperty, ConnectedBinding);
            varTypesBtn.SetBinding(IsEnabledProperty, ConnectedBinding);
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
            SerHandler.Connect(portName);
        }

        private void buttondisconnect_Click(object sender, RoutedEventArgs e)
        {
            disconnectFromPort();
        }

        private void disconnectFromPort()
        {
            SerHandler.Disconnect();
        }

        private void varTypesBtn_Click(object sender, RoutedEventArgs e)
        {
            SerHandler.SendVarTypes();
        }
        
        private void sendVarsBtn_Click(object sender, RoutedEventArgs e)
        {

        }

        private void sendVarStreamBtn_Click(object sender, RoutedEventArgs e)
        {

        }
        
        // helpers
        private void RefreshPortDropDown()
        {
            // get COM ports, set it to the dropdown
            PortNames = System.IO.Ports.SerialPort.GetPortNames();
            dropdownPorts.ItemsSource = PortNames;
        }
        
        // interface functions
        public void AppendLog(string toAppend, bool newLine = true, bool timeStamp = true, bool scrollToEnd = true)
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
