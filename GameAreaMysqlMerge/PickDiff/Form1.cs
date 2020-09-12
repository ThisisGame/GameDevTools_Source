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
using System.Threading;
using Newtonsoft.Json;
using Renci.SshNet.Common;

namespace PickDiff
{
    public partial class Form1 : Form
    {

        private class Config
        {
            public class ConflictTable
            {
                public string tableName;
                public string[] columns;
                /// <summary>
                /// 解决方式 1.AddServerTag  2.UseLast 仅复制最后一个数据库的 3.AutoIncrReset 自增的字段不赋值
                /// </summary>
                public string resolveType;
            }
            public List<ConflictTable> conflictTables=new List<ConflictTable>();
        }

        private JsonConfigParser<Config> mJsonConfigParser = new JsonConfigParser<Config>();

        private string mDBName1;
        private string mDBName2;

        private int mProgressValue = 0;

        private Queue<string> logQueue = new Queue<string>();

        /// <summary>
        /// 数据库对应的前缀 例如1_gamedb  2_gamedb合并的时候某个表有冲突，那么就在数据前面添加  "S1."  这样的前缀。
        /// </summary>
        private Dictionary<string, string> mDBServerTag = new Dictionary<string, string>();

        /// <summary>
        /// 临时表前缀
        /// </summary>
        private string TmpTablePrefix = "tmp_";

        public Form1()
        {
            InitializeComponent();


            mJsonConfigParser.Load("./config.json");
        }



 
        public class MyTable
        {
            public string Table { get; set; }
            public List<string> ColumnList { get; set; } = new List<string>();
        }


        private void buttonConnect_Click(object sender, EventArgs e)
        {
            string connstr = "data source=localhost;user id=root;password=111111;pooling=false;charset=utf8";//pooling代表是否使用连接池
            MySqlConnection conn = new MySqlConnection(connstr);
            conn.Open();

            string newTableCMD = "SHOW DATABASES;";
            MySqlCommand cmd = CreateMySqlCommand(newTableCMD, conn);
            using (MySqlDataReader reader = cmd.ExecuteReader())
            {
                while (reader.Read())
                {
                    string dbName = reader[0].ToString();
                    comboBoxChooseDB1.Items.Add(dbName);
                    comboBoxChooseDB2.Items.Add(dbName);
                }
            }

            conn.Close();
        }

        private void comboBoxChooseDB1_SelectedIndexChanged(object sender, EventArgs e)
        {
            mDBName1= comboBoxChooseDB1.SelectedItem.ToString();
        }

        /// <summary>
        /// 更新进度条进度
        /// </summary>
        /// <param name="current">当前 从0开始</param>
        /// <param name="total">总的</param>
        private void UpdateProgressValue(int current,int total)
        {
            mProgressValue = (current+1)*100/total;
        }

        private void comboBoxChooseDB2_SelectedIndexChanged(object sender, EventArgs e)
        {
            mDBName2 = comboBoxChooseDB2.SelectedItem.ToString();
        }

        private void buttonMerge_Click(object sender, EventArgs e)
        {
            //收集数据库前缀
            mDBServerTag.Add(mDBName1, textBoxDB1ServerTag.Text);
            mDBServerTag.Add(mDBName2, textBoxDB2ServerTag.Text);

            ThreadPool.QueueUserWorkItem(MergeInThread);
        }

        private void MergeInThread(Object state)
        {
            bool bRet = CreateMergeDB();

            if (bRet)
            {
                if (MessageBox.Show("创建数据库成功，点击继续复制表结构", "选择操作", MessageBoxButtons.OKCancel) == DialogResult.Cancel)
                {
                    return;
                }
                bRet = CreateAllTable();
            }

            if (bRet)
            {
                if (MessageBox.Show("复制表结构完成，点击继续复制表数据", "选择操作", MessageBoxButtons.OKCancel) == DialogResult.Cancel)
                {
                    return;
                }
                bRet = CopyAllTableData();
            }

            if (bRet)
            {
                if (MessageBox.Show("复制表数据完成", "选择操作", MessageBoxButtons.OKCancel) == DialogResult.Cancel)
                {
                    return;
                }
            }
        }

        private MySqlCommand CreateMySqlCommand(string cmdStr,MySqlConnection conn)
        {
            Log(cmdStr);
            return new MySqlCommand(cmdStr, conn);
        }

        /// <summary>
        /// 创建合服数据库
        /// </summary>
        private bool CreateMergeDB()
        {
            string mergeDBName = textBoxMergeDBName.Text;

            string connstr = "data source=localhost;user id=root;password=111111;pooling=false;charset=utf8";//pooling代表是否使用连接池
            MySqlConnection conn = new MySqlConnection(connstr);

            //创建合服数据库
            string cmdStr = "CREATE DATABASE `" + mergeDBName + "` CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;";
            
            MySqlCommand cmd = CreateMySqlCommand(cmdStr, conn);
            conn.Open();

            try
            {
                //执行语句
                int res = cmd.ExecuteNonQuery();

                conn.Close();

                if (res != 1)
                {
                    MessageBox.Show("create db error:" + cmdStr);
                    return false;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
                return false;
            }

            return true;
        }

        /// <summary>
        /// 创建连接
        /// </summary>
        /// <returns></returns>
        private MySqlConnection CreteConnection()
        {
            string connstr = "data source=localhost;user id=root;password=111111;pooling=false;charset=utf8";//pooling代表是否使用连接池
            MySqlConnection conn = new MySqlConnection(connstr);
            return conn;
        }

        /// <summary>
        /// 创建表并复制表结构
        /// </summary>
        /// <param name="conn"></param>
        /// <param name="mergeDBName"></param>
        /// <param name="tableNameStr"></param>
        /// <param name="dbName"></param>
        /// <returns></returns>
        private bool CloneTableStruct(MySqlConnection conn, string mergeDBName, string tableNameStr,string dbName,bool temp=false)
        {
            //创建表并复制表结构
            string cmdStr = "";
            if(temp)
            {
                cmdStr = "create TEMPORARY table " + mergeDBName + "."+TmpTablePrefix + tableNameStr + " like " + dbName + "." + tableNameStr + ";";
            }
            else
            {
                cmdStr = "create table " + mergeDBName + "." + tableNameStr + " like " + dbName + "." + tableNameStr + ";";
            }
            MySqlCommand cmd = CreateMySqlCommand(cmdStr, conn);

            try
            {
                //执行语句
                int res = cmd.ExecuteNonQuery();
                if (res != 0)
                {
                    MessageBox.Show("create table error:" + cmdStr);
                    conn.Close();
                    return false;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
                conn.Close();
                return false;
            }
            return true;
        }

        /// <summary>
        /// 创建所有表
        /// </summary>
        /// <returns></returns>
        private bool CreateAllTable()
        {
            Dictionary<string, MyTable> tableStructDic = GetAllTableSruct(mDBName1);//这里用DBName1 和 DBName2都可以，既然是合服，那这两个服的db结构肯定是一样的。

            string mergeDBName = textBoxMergeDBName.Text;

            MySqlConnection conn = CreteConnection();

            conn.Open();

            List<string> tableNameList = tableStructDic.Keys.ToList();
            for (int tableNameIndex = 0; tableNameIndex < tableNameList.Count; tableNameIndex++)
            {
                UpdateProgressValue(tableNameIndex, tableNameList.Count);

                string tableNameStr = tableNameList[tableNameIndex];

                //创建表并复制表结构
                if (CloneTableStruct(conn, mergeDBName, tableNameStr, mDBName1)==false)
                {
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// 检查表是否有冲突字段
        /// </summary>
        /// <param name="tableName"></param>
        /// <returns></returns>
        private Config.ConflictTable CheckTableConflict(string tableName)
        {
            var confictTables = mJsonConfigParser.instance.conflictTables;
            for (int i = 0; i < confictTables.Count; i++)
            {
                if(confictTables[i].tableName==tableName)
                {
                    return confictTables[i];
                }
            }
            return null;
        }


        /// <summary>
        /// 复制一个表的数据
        /// </summary>
        /// <param name="conn"></param>
        /// <param name="targetDBName"></param>
        /// <param name="targetTableName"></param>
        /// <param name="srcDBName"></param>
        /// <returns></returns>
        private bool CopyTableData(MySqlConnection conn,string targetDBName,string targetTableName,string srcDBName,string srcTableName)
        {
            string cmdStr = "INSERT INTO " + targetDBName +"."+ targetTableName + " SELECT * FROM " + srcDBName + "." + srcTableName + ";";
            MySqlCommand cmd = CreateMySqlCommand(cmdStr, conn);

            try
            {
                //执行语句
                int res = cmd.ExecuteNonQuery();
                //返回的res是影响的行数，例如复制了2行，那么返回2.
                return true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(cmdStr + Environment.NewLine + ex.ToString());
                conn.Close();
                return false;
            }
        }

        /// <summary>
        /// 在临时表中，对字段添加前缀
        /// </summary>
        /// <param name="conn"></param>
        /// <param name="dbName"></param>
        /// <param name="tableName"></param>
        /// <param name="dbName"></param>
        /// <returns></returns>
        private bool AddServerTagInTmpTable(MySqlConnection conn, string dbName, string tableName,string columnName,string serverTag)
        {
            string cmdStr = "update "+ dbName + "."+ tableName+" set "+ columnName +"= CONCAT('"+ serverTag+"',"+ columnName+"); ";
            MySqlCommand cmd = CreateMySqlCommand(cmdStr, conn);

            try
            {
                //执行语句
                int res = cmd.ExecuteNonQuery();
                return true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(cmdStr + Environment.NewLine + ex.ToString());
                conn.Close();
                return false;
            }
        }

        /// <summary>
        /// 删除表
        /// </summary>
        /// <param name="conn"></param>
        /// <param name="dbName"></param>
        /// <param name="tableName"></param>
        /// <returns></returns>
        private bool DropTable(MySqlConnection conn, string dbName, string tableName)
        {
            string cmdStr = "drop table "+ dbName + "."+ tableName + ";";
            MySqlCommand cmd = CreateMySqlCommand(cmdStr, conn);

            try
            {
                //执行语句
                int res = cmd.ExecuteNonQuery();
                return true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(cmdStr + Environment.NewLine + ex.ToString());
                conn.Close();
                return false;
            }
        }

        /// <summary>
        /// 复制表数据
        /// </summary>
        /// <returns></returns>
        private bool CopyAllTableData()
        {
            Dictionary<string, MyTable> tableStructDic = GetAllTableSruct(mDBName1);//这里用DBName1 和 DBName2都可以，既然是合服，那这两个服的db结构肯定是一样的。

            string mergeDBName = textBoxMergeDBName.Text;

            MySqlConnection conn = CreteConnection();
            conn.Open();

            List<string> tableNameList = tableStructDic.Keys.ToList();

            //按顺序对2个DB执行以下操作
            foreach (var item in mDBServerTag)
            {
                string dbName = item.Key;
                string serverTag = item.Value;

                for (int tableNameIndex = 0; tableNameIndex < tableNameList.Count; tableNameIndex++)
                {
                    UpdateProgressValue(tableNameIndex, tableNameList.Count);

                    string tableName = tableNameList[tableNameIndex];
                    string mergeDBTmpTableName = TmpTablePrefix + tableName;

                    //1.创建临时表
                    if(CloneTableStruct(conn, mergeDBName, tableName, dbName, true)==false)
                    {
                        return false;
                    }
                    //2.复制数据到临时表
                    if(CopyTableData(conn, mergeDBName, mergeDBTmpTableName,dbName,tableName)==false)
                    {
                        return false;
                    }
                    //3.处理冲突
                    Config.ConflictTable conflictTable = CheckTableConflict(tableName);
                    if (conflictTable != null)
                    {
                        //对于会冲突的表,根据配置的处理方式进行处理
                        switch (conflictTable.resolveType)
                        {
                            case "AddServerTag":
                                {
                                    //暂时只处理一个字段
                                    if(AddServerTagInTmpTable(conn, mergeDBName, mergeDBTmpTableName, conflictTable.columns[0], serverTag)==false)
                                    {
                                        return false;
                                    }
                                }
                                break;
                            case "UseLast":
                                {
                                    //仅复制最后一个数据库的

                                }
                                break;
                            default:
                                break;
                        }
                    }
                    //4.复制临时表数据到正式表
                    if(CopyTableData(conn, mergeDBName, tableName, mergeDBName, mergeDBTmpTableName)==false)
                    {
                        return false;
                    }
                    //5.删除临时表
                    if(DropTable(conn,mergeDBName,mergeDBTmpTableName)==false)
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        /// <summary>
        /// 获取所有表名以及字段
        /// </summary>
        /// <param name="dbName"></param>
        /// <returns></returns>
        private Dictionary<string, MyTable> GetAllTableSruct(string dbName)
        {
            string connstr = "data source=localhost;user id=root;password=111111;pooling=false;charset=utf8";//pooling代表是否使用连接池
            MySqlConnection conn = new MySqlConnection(connstr);
            conn.Open();

            //获取所有table以及对应的字段名。
            Dictionary<string, MyTable> dic = new Dictionary<string, MyTable>();
            string newTableCMD = "select table_name, column_name from information_schema.columns where table_schema = '" + dbName + "';";
            MySqlCommand cmd = CreateMySqlCommand(newTableCMD, conn);
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
            conn.Close();

            return dic;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            progressBarMerge.Value = mProgressValue;

            if(logQueue.Count>0)
            {
                string log = logQueue.Dequeue();
                listBoxLogs.Items.Add(log);
            }

        }
    
        private void Log(string str)
        {
            logQueue.Enqueue(str);
        }

        /// <summary>
        /// 日志列表按 C 复制选中行
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void listBoxLogs_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyData == Keys.C)
            {
                Clipboard.SetDataObject(listBoxLogs.SelectedItem.ToString());
            }
        }
    }
}
