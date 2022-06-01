using System;
using System.Collections.Generic;
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
using ScnViewer;

namespace ScnViewer.GUI
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }
        private void Button_Confirm_Click(object sender, RoutedEventArgs e)
        {
            Parser parser = new Parser();
            StringBuilder _address = new StringBuilder();
            _address.Append(TextBox_Address.Text);
            try
            {
                parser.Init(_address);
                var _result = parser.Parse();
                var _items = ScnStringHelper.ToTreeViewItems(_result);
                for(int i = 0; i < _items.Count; i++)
                {
                    TreeView_ScnStrings.Items.Add(_items[i]);
                }
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            
        }
    }
}
