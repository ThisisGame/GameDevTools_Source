using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace UnityRemoveManifestFile
{
    public partial class Form1 : Form
    {
        string chooseDirPath = null;

        public Form1()
        {
            InitializeComponent();
        }

        private void buttonChoose_Click(object sender, EventArgs e)
        {
            
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.Description = "请选择文件夹";

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                chooseDirPath = dialog.SelectedPath;
            }
            if (chooseDirPath == null)
            {
                return;
            }

            textBoxDirPath.Text = chooseDirPath;
        }

        private void buttonGenerate_Click(object sender, EventArgs e)
        {
            if (chooseDirPath == null)
            {
                MessageBox.Show("先选择目录，然后生成。");
                return;
            }

            //复制到新文件夹
            string newDirPath =Path.GetDirectoryName(chooseDirPath) + Path.DirectorySeparatorChar + "Clear_" + DateTime.Now.ToString("yyyyMMddHHmmss") + Path.DirectorySeparatorChar + Path.GetFileName(chooseDirPath);

            //要复制以当前文件夹名字命名的Manifest
            string dirName = Path.GetFileName(chooseDirPath);

            string totalManifestFilePath = Path.Combine(chooseDirPath, dirName+ ".manifest");

            string[] filePaths = Directory.GetFiles(chooseDirPath, "*.*", SearchOption.AllDirectories);
            foreach (var filePath in filePaths)
            {
                if(filePath.EndsWith(".manifest") && filePath != totalManifestFilePath)
                {
                    continue;
                }

                //复制
                string newFilePath = filePath.Replace(chooseDirPath, newDirPath);
                string newFileDirPath = Path.GetDirectoryName(newFilePath);
                if(Directory.Exists(newFileDirPath)==false)
                {
                    Directory.CreateDirectory(newFileDirPath);
                }
                File.Copy(filePath, newFilePath);
            }
            MessageBox.Show("处理完成，新的目录："+ newDirPath);
        }

        private void textBoxDirPath_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                chooseDirPath = ((string[])e.Data.GetData(DataFormats.FileDrop.ToString()))[0];
                textBoxDirPath.Text = chooseDirPath;
            }
        }

        private void textBoxDirPath_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effect = DragDropEffects.All;
            }
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }
    }
}
