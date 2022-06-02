using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using ScnViewer;
using Panuon.UI;
using Panuon.UI.Silver;
using Panuon.UI.Silver.Controls;
using System.Runtime.InteropServices;

namespace ScnViewer.GUI
{
    public class ScnStringHelper
    {
        [DllImport("user32.dll", EntryPoint = "MessageBox", CharSet = CharSet.Auto)]
        public static extern int MsgBox(IntPtr hWnd, String lpText, String lpCaption, int uType);
        public static List<TreeViewItem> ToTreeViewItems(List<Parser.ScnString> _source)
        {
            var _size = _source.Count;
            List<TreeViewItem> _result = new List<TreeViewItem>();
            for (int i = 0; i < _size; i++)
            {
                TreeViewItem _item = new TreeViewItem
                {
                    Header = _source[i].Content[0]._beginPos.ToString() + '-' + _source[i].Content[0]._endPos.ToString()
                };
                for (int j = 0; j < _source[i].Content.Count; j++)
                {
                    if (_source[i].Speaker.Count() != 0)
                        _item.Items.Add(new TreeViewItem { Header = _source[i].Speaker + "：" + _source[i].Content[j]._Content, DataContext = _source[i].Content[j] });
                    else
                        _item.Items.Add(new TreeViewItem { Header = "旁白" + "：" + _source[i].Content[j]._Content });
                }

                _result.Add(_item);
            }
            return _result;
        }
        public static bool BindScnStringWithTextBlock(ref List<TreeViewItem> _source, TextBlock _dest)
        {
            //_dest.Text = "test";
            IntPtr _ptr = (IntPtr)null;
            Parallel.ForEach(_source, node =>
             {
                 foreach(TreeViewItem _item in node.Items)
                 {
                     _item.MouseLeftButtonUp += (o, j) =>
                     {
                         _dest.Text = "Source: \n" + ((Parser.ScnSingleString)_item.DataContext)._Content;
                         _dest.DataContext = (Parser.ScnSingleString)_item.DataContext;
                     };
                     _item.MouseDoubleClick += (o, j) =>
                     {
                         _dest.Text = "Source: \n" + ((Parser.ScnSingleString)_item.DataContext)._Content;
                         _dest.DataContext = (Parser.ScnSingleString)_item.DataContext;
                     };
                     
                 };
             }
            );
            return true;
        }

    }
}
