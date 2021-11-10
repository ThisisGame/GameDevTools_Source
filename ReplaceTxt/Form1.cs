using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ReplaceTxt
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void buttonReplace_Click(object sender, EventArgs e)
        {
            string replaceStart = textBoxStart.Text;
            string replaceEnd = textBoxEnd.Text;

            List<string> dstLines = new List<string>();
            foreach (var iter in textBoxSrc.Lines)
            {
                var srcLine = iter;

                int begin = srcLine.IndexOf(replaceStart);
                if (begin < 0)
                {
                    dstLines.Add(srcLine);
                    continue;
                }
                int end = srcLine.Substring(begin).IndexOf(replaceEnd);
                if(end < 0)
                {
                    dstLines.Add(srcLine);
                    continue;
                }
                int length = end + replaceEnd.Length;

                string subString = srcLine.Substring(begin, length);
                string newLine = srcLine.Replace(subString, textBoxReplace.Text);
                dstLines.Add(newLine);
            }
            textBoxDst.Lines = dstLines.ToArray();
        }
    }
}
