namespace EncryptResources
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.buttonEncrypt = new System.Windows.Forms.Button();
            this.textBoxEncryptKey = new System.Windows.Forms.TextBox();
            this.buttonChooseSrc = new System.Windows.Forms.Button();
            this.labelChooseSrc = new System.Windows.Forms.Label();
            this.labelChooseDst = new System.Windows.Forms.Label();
            this.buttonChooseDst = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonEncrypt
            // 
            this.buttonEncrypt.Location = new System.Drawing.Point(37, 198);
            this.buttonEncrypt.Name = "buttonEncrypt";
            this.buttonEncrypt.Size = new System.Drawing.Size(75, 23);
            this.buttonEncrypt.TabIndex = 0;
            this.buttonEncrypt.Text = "加密";
            this.buttonEncrypt.UseVisualStyleBackColor = true;
            this.buttonEncrypt.Click += new System.EventHandler(this.buttonEncrypt_Click);
            // 
            // textBoxEncryptKey
            // 
            this.textBoxEncryptKey.Location = new System.Drawing.Point(37, 152);
            this.textBoxEncryptKey.Name = "textBoxEncryptKey";
            this.textBoxEncryptKey.Size = new System.Drawing.Size(134, 21);
            this.textBoxEncryptKey.TabIndex = 1;
            this.textBoxEncryptKey.Text = "输入整数(1-254)密钥";
            // 
            // buttonChooseSrc
            // 
            this.buttonChooseSrc.Location = new System.Drawing.Point(37, 30);
            this.buttonChooseSrc.Name = "buttonChooseSrc";
            this.buttonChooseSrc.Size = new System.Drawing.Size(134, 23);
            this.buttonChooseSrc.TabIndex = 2;
            this.buttonChooseSrc.Text = "选择需要加密的文件夹";
            this.buttonChooseSrc.UseVisualStyleBackColor = true;
            this.buttonChooseSrc.Click += new System.EventHandler(this.buttonChooseSrc_Click);
            // 
            // labelChooseSrc
            // 
            this.labelChooseSrc.AutoSize = true;
            this.labelChooseSrc.Location = new System.Drawing.Point(37, 60);
            this.labelChooseSrc.Name = "labelChooseSrc";
            this.labelChooseSrc.Size = new System.Drawing.Size(41, 12);
            this.labelChooseSrc.TabIndex = 3;
            this.labelChooseSrc.Text = "label1";
            // 
            // labelChooseDst
            // 
            this.labelChooseDst.AutoSize = true;
            this.labelChooseDst.Location = new System.Drawing.Point(39, 120);
            this.labelChooseDst.Name = "labelChooseDst";
            this.labelChooseDst.Size = new System.Drawing.Size(41, 12);
            this.labelChooseDst.TabIndex = 5;
            this.labelChooseDst.Text = "label2";
            // 
            // buttonChooseDst
            // 
            this.buttonChooseDst.Location = new System.Drawing.Point(39, 90);
            this.buttonChooseDst.Name = "buttonChooseDst";
            this.buttonChooseDst.Size = new System.Drawing.Size(134, 23);
            this.buttonChooseDst.TabIndex = 4;
            this.buttonChooseDst.Text = "选择存放文件夹";
            this.buttonChooseDst.UseVisualStyleBackColor = true;
            this.buttonChooseDst.Click += new System.EventHandler(this.buttonChooseDst_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(315, 306);
            this.Controls.Add(this.labelChooseDst);
            this.Controls.Add(this.buttonChooseDst);
            this.Controls.Add(this.labelChooseSrc);
            this.Controls.Add(this.buttonChooseSrc);
            this.Controls.Add(this.textBoxEncryptKey);
            this.Controls.Add(this.buttonEncrypt);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonEncrypt;
        private System.Windows.Forms.TextBox textBoxEncryptKey;
        private System.Windows.Forms.Button buttonChooseSrc;
        private System.Windows.Forms.Label labelChooseSrc;
        private System.Windows.Forms.Label labelChooseDst;
        private System.Windows.Forms.Button buttonChooseDst;
    }
}

