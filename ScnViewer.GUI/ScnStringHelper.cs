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

namespace ScnViewer.GUI
{
    public class ScnStringHelper
    {
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
                    _item.Items.Add(new TreeViewItem { Header = _source[i].Speaker + "：" + _source[i].Content[j]._Content });
                }
                
                _result.Add(_item);
            }
            return _result;
        }
    }
}
