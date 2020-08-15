using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace FileListSortPrint
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private FileInfo GetFileInfo(Dictionary<string, FileInfo> keyValuePairs,string filePath)
        {
            FileInfo fileInfo = null;
            if (keyValuePairs.ContainsKey(filePath) == false)
            {
                fileInfo = new FileInfo(filePath);
                keyValuePairs.Add(filePath, fileInfo);
            }
            else
            {
                fileInfo = keyValuePairs[filePath];
            }
            return fileInfo;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string savePath = null;
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.Description = "请选择文件夹";

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                savePath = dialog.SelectedPath;
            }
            if(savePath==null)
            {
                return;
            }

            List<string> fileList = new List<string>();
            Dictionary<string, FileInfo> keyValuePairs = new Dictionary<string, FileInfo>();

            string[] files= Directory.GetFiles(savePath, "*.*", SearchOption.AllDirectories);

            //排序
            for (int i = 0; i < files.Length; i++)
            {
                for (int j = 0; j < files.Length-i-1; j++)//每次从左边开始，一次内循环挑出最大的放到最右边
                {

                    FileInfo fileInfo= GetFileInfo(keyValuePairs, files[j]);
                    FileInfo fileInfoRight = GetFileInfo(keyValuePairs, files[j + 1]);

                    if (fileInfo.Length>fileInfoRight.Length)
                    {
                        string tmp = files[j + 1];
                        files[j + 1] = files[j];
                        files[j] = tmp;
                    }
                }
            }

            //保存到txt
            StreamWriter streamWriter = new StreamWriter("./files.txt");
            for (int i = files.Length-1; i >=0; i--)
            {
                FileInfo fileInfo = GetFileInfo(keyValuePairs, files[i]);
                streamWriter.WriteLine(fileInfo.Length+" "+files[i]);
            }
            streamWriter.Close();
        }
    }
}
