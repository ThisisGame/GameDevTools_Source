using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EncryptResources
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private string mChooseSrc = string.Empty;
        private string mChooseDst = string.Empty;

        private void buttonEncrypt_Click(object sender, EventArgs e)
        {
            //先判断密钥对不对
            string tmpEncryptStr = textBoxEncryptKey.Text;
            int tmpEncryptKey = 0;
            if (int.TryParse(tmpEncryptStr, out tmpEncryptKey) == false)
            {
                MessageBox.Show("错误", "密钥输入错误 整数 且 大于0 小于255");
                return;
            }

            if (tmpEncryptKey <= 0 || tmpEncryptKey >= 255)
            {
                MessageBox.Show("错误", "密钥输入错误 整数 且 大于0 小于255");
                return;
            }


            Encrypt((byte)tmpEncryptKey);
        }

        private void Encrypt(byte varEncryptKey)
        {
            mChooseSrc= mChooseSrc.Replace("\\", "/");
            string[] bundlefiles = Directory.GetFiles(mChooseSrc, "*.*", SearchOption.AllDirectories);
            for (int i = 0; i < bundlefiles.Length; i++)
            {
                string tmpfilepath = bundlefiles[i];

                string tmpEncryptFilePath = tmpfilepath.Replace("\\", "/");
 
                tmpEncryptFilePath = tmpEncryptFilePath.Replace(mChooseSrc, mChooseDst); //Windows

                byte[] tmpData = File.ReadAllBytes(tmpfilepath);

                for (int tmpDataIndex = 0; tmpDataIndex < tmpData.Length; tmpDataIndex++)
                {
                    //加密
                    tmpData[tmpDataIndex] ^= varEncryptKey;
                }


                string tmpEncryptFileDirectoryPath = Path.GetDirectoryName(tmpEncryptFilePath);



                if (Directory.Exists(tmpEncryptFileDirectoryPath) == false)
                {

                    Directory.CreateDirectory(tmpEncryptFileDirectoryPath);
                }
                File.WriteAllBytes(tmpEncryptFilePath, tmpData);
            }

            //写入EncryptKey
            File.WriteAllText(mChooseDst + "/EncryptKey", varEncryptKey.ToString());
        }

        private void buttonChooseSrc_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folder = new FolderBrowserDialog();
            folder.Description = "选择需要加密的目录";
            if (folder.ShowDialog() == DialogResult.OK)
            {
                mChooseSrc = folder.SelectedPath;
                labelChooseSrc.Text = mChooseSrc;
            }
        }

        private void buttonChooseDst_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folder = new FolderBrowserDialog();
            folder.Description = "选择需要加密的目录";
            if (folder.ShowDialog() == DialogResult.OK)
            {
                mChooseDst = folder.SelectedPath;
                labelChooseDst.Text = mChooseDst;
            }
        }
    }
}
