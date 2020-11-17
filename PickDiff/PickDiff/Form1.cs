using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using Newtonsoft.Json;

namespace PickDiff
{
    public partial class Form1 : Form
    {
        private string chooseOldDirPath;
        private string chooseNewDirPath;

        /// <summary>
        /// 挑选出来的文件，指定文件夹存放。
        /// </summary>
        private string generateDirPath;

        private string configFilePath;

        private class Config
        {
            public string chooseOldDirPath;
            public string chooseNewDirPath;
        }
        private Config config=new Config();

        public Form1()
        {
            InitializeComponent();

            configFilePath = Path.Combine(Path.GetTempPath(), "PickDiff.json");
            //读取配置文件
            if (File.Exists(configFilePath))
            {
                string jsonStr = File.ReadAllText(configFilePath);
                config = JsonConvert.DeserializeObject<Config>(jsonStr);

                chooseOldDirPath = config.chooseOldDirPath;
                textBox1.Text = chooseOldDirPath;

                chooseNewDirPath = config.chooseNewDirPath;
                textBox2.Text = chooseNewDirPath;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.Description = "请选择文件夹";

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                chooseOldDirPath = dialog.SelectedPath;
            }
            if (chooseOldDirPath == null)
            {
                return;
            }

            textBox1.Text = chooseOldDirPath;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.Description = "请选择文件夹";

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                chooseNewDirPath = dialog.SelectedPath;
            }
            if (chooseNewDirPath == null)
            {
                return;
            }

            textBox2.Text = chooseNewDirPath;
        }

        private void buttonPick_Click(object sender, EventArgs e)
        {
            generateDirPath = Path.GetDirectoryName( chooseNewDirPath) + Path.DirectorySeparatorChar + "Pick_" + DateTime.Now.ToString("yyyyMMddHHmmss")+ Path.DirectorySeparatorChar+Path.GetFileName(chooseNewDirPath);

            //获取新文件夹文件列表
            string[] newFilePathArr = Directory.GetFiles(chooseNewDirPath, "*.*",SearchOption.AllDirectories);
            //获取相对选中文件夹的 短路径
            for (int i = 0; i < newFilePathArr.Length; i++)
            {
                string newFilePath = newFilePathArr[i];
                string newFileShortPath = newFilePath.Substring(chooseNewDirPath.Length);

                string oldFilePath = chooseOldDirPath + newFileShortPath;
                if(File.Exists(oldFilePath)==false)
                {
                    //旧文件夹中没有对应文件
                    CopyFile(newFilePath, generateDirPath + newFileShortPath);
                }
                else
                {
                    //对比MD5
                    string oldFileMd5 = GetMD5HashFromFile(oldFilePath);
                    string newFileMd5 = GetMD5HashFromFile(newFilePath);
                    if(oldFileMd5!=newFileMd5)
                    {
                        CopyFile(newFilePath, generateDirPath+newFileShortPath);
                    }
                }
            }

            //保存配置文件
            config.chooseNewDirPath = chooseNewDirPath;
            config.chooseOldDirPath = chooseOldDirPath;
            string jsonStr = JsonConvert.SerializeObject(config);
            File.WriteAllText(configFilePath, jsonStr);
            

            MessageBox.Show("对比完成，不同的文件挑出来存放在："+ generateDirPath);
        }

        /// <summary>
        /// 将新文件夹中新文件 和 有修改的文件，拷贝到Pick目录中。
        /// </summary>
        /// <param name="newFilePath"></param>
        /// <param name="generateFilePath"></param>
        private void CopyFile(string newFilePath, string generateFilePath)
        {
            string generateFileDirPath = Path.GetDirectoryName(generateFilePath);
            if (Directory.Exists(generateFileDirPath) == false)
            {
                Directory.CreateDirectory(generateFileDirPath);
            }
            File.Copy(newFilePath, generateFilePath);
        }

        /// <summary>
        /// 获取文件MD5值
        /// </summary>
        /// <param name="fileName">文件绝对路径</param>
        /// <returns>MD5值</returns>
        public static string GetMD5HashFromFile(string fileName)
        {
            try
            {
                FileStream file = new FileStream(fileName, FileMode.Open);
                System.Security.Cryptography.MD5 md5 = new System.Security.Cryptography.MD5CryptoServiceProvider();
                byte[] retVal = md5.ComputeHash(file);
                file.Close();

                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < retVal.Length; i++)
                {
                    sb.Append(retVal[i].ToString("x2"));
                }
                return sb.ToString();
            }
            catch (Exception ex)
            {
                throw new Exception("GetMD5HashFromFile() fail,error:" + ex.Message);
            }
        }

        private void textBox2_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                chooseNewDirPath = ((string[])e.Data.GetData(DataFormats.FileDrop.ToString()))[0];
                textBox2.Text = chooseNewDirPath;
            }
        }

        private void textBox2_DragEnter(object sender, DragEventArgs e)
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

        private void textBox1_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                chooseOldDirPath = ((string[])e.Data.GetData(DataFormats.FileDrop.ToString()))[0];
                textBox1.Text = chooseOldDirPath;
            }
        }

        private void textBox1_DragEnter(object sender, DragEventArgs e)
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
