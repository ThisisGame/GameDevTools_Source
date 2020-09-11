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
using MySql.Data.MySqlClient;

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

        private void buttonCreateDB_Click(object sender, EventArgs e)
        {
            //string connstr = "data source=localhost;user id=root;password=111111;pooling=false;charset=utf8";//pooling代表是否使用连接池
            //MySqlConnection conn = new MySqlConnection(connstr);

            ////创建数据库的执行语句
            //MySqlCommand cmd = new MySqlCommand("CREATE DATABASE `test` CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci", conn);
            //conn.Open();
            ////执行语句
            //int res = cmd.ExecuteNonQuery();

            //conn.Close();

            //CreateTable();

            LoadTables();
        }

        private void CreateTable()
        {
            string connstr = "data source=localhost;database=test;user id=root;password=111111;pooling=false;charset=utf8";//pooling代表是否使用连接池
            MySqlConnection conn = new MySqlConnection(connstr);

            //创建数据库的执行语句
            string newTableCMD = "CREATE TABLE `TESTTABLE3` (`Id_` int(11) NOT NULL AUTO_INCREMENT,`NAME` VARCHAR(32) NOT NULL,PRIMARY KEY(`Id_`));";
            MySqlCommand cmd = new MySqlCommand(newTableCMD, conn);
            conn.Open();
            //执行语句
            int res = cmd.ExecuteNonQuery();

            conn.Close();
        }

        public class MyTable
        {
            public string Table { get; set; }
            public List<string> ColumnList { get; set; } = new List<string>();
        }

        /// <summary>
        /// 获取所有表名
        /// </summary>
        private void LoadTables()
        {
            string connstr = "data source=127.0.0.1;user id=root;password=YLTX20200706.;pooling=false;charset=utf8mb4";//pooling代表是否使用连接池
            MySqlConnection conn = new MySqlConnection(connstr);

            Dictionary<string, MyTable> dic = new Dictionary<string, MyTable>();
            MySqlCommand cmd = conn.CreateCommand();
            cmd.CommandText = "select table_name, column_name from information_schema.columns where table_schema = 'platform';";
            using (MySqlDataReader reader = cmd.ExecuteReader())
            {
                while (reader.Read())
                {
                    string table = reader.GetString("table_name");
                    string column = reader.GetString("column_name");
                    if (dic.ContainsKey(table))
                    {
                        dic[table].ColumnList.Add(column);
                    }
                    else
                    {
                        MyTable t = new MyTable();
                        t.Table = table;
                        t.ColumnList.Add(column);
                        dic.Add(t.Table, t);
                    }
                }
            }
        }
    }
}
