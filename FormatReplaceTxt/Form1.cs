using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FormatReplaceTxt
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void buttonFormat_Click(object sender, EventArgs e)
        {
            string discardFrom = textBoxDiscardFrom.Text.Trim();
            List<string> dstLines = new List<string>();
            foreach (var iter in textBoxSrc.Lines)
            {
                var srcLine = iter.Trim();
                if (string.IsNullOrWhiteSpace(srcLine))
                {
                    continue;
                }
                //从这个字符串开始抛弃
                if (string.IsNullOrWhiteSpace(discardFrom)==false)
                {
                    srcLine = srcLine.Substring(0, srcLine.IndexOf(discardFrom));
                }
                dstLines.Add(string.Format(textBoxRule.Text, srcLine));
            }
            textBoxDst.Lines = dstLines.ToArray();
        }
    }
}
