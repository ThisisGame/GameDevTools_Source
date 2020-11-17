namespace UnityRemoveManifestFile
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
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.buttonChoose = new System.Windows.Forms.Button();
            this.buttonGenerate = new System.Windows.Forms.Button();
            this.textBoxDirPath = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // textBox1
            // 
            this.textBox1.Enabled = false;
            this.textBox1.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBox1.Location = new System.Drawing.Point(2, 4);
            this.textBox1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(345, 69);
            this.textBox1.TabIndex = 0;
            this.textBox1.Text = "Unity打Assetbundle后，会生成一个Manifest文件。\r\n游戏不需要这个文件也是正常运行的。\r\n删掉它，减少上传与下载热更新资源的文件数量。";
            // 
            // buttonChoose
            // 
            this.buttonChoose.Location = new System.Drawing.Point(260, 81);
            this.buttonChoose.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.buttonChoose.Name = "buttonChoose";
            this.buttonChoose.Size = new System.Drawing.Size(87, 33);
            this.buttonChoose.TabIndex = 1;
            this.buttonChoose.Text = "选择目录";
            this.buttonChoose.UseVisualStyleBackColor = true;
            this.buttonChoose.Click += new System.EventHandler(this.buttonChoose_Click);
            // 
            // buttonGenerate
            // 
            this.buttonGenerate.Location = new System.Drawing.Point(260, 135);
            this.buttonGenerate.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.buttonGenerate.Name = "buttonGenerate";
            this.buttonGenerate.Size = new System.Drawing.Size(87, 33);
            this.buttonGenerate.TabIndex = 2;
            this.buttonGenerate.Text = "生成";
            this.buttonGenerate.UseVisualStyleBackColor = true;
            this.buttonGenerate.Click += new System.EventHandler(this.buttonGenerate_Click);
            // 
            // textBoxDirPath
            // 
            this.textBoxDirPath.AllowDrop = true;
            this.textBoxDirPath.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBoxDirPath.Location = new System.Drawing.Point(2, 82);
            this.textBoxDirPath.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.textBoxDirPath.Multiline = true;
            this.textBoxDirPath.Name = "textBoxDirPath";
            this.textBoxDirPath.ReadOnly = true;
            this.textBoxDirPath.Size = new System.Drawing.Size(250, 86);
            this.textBoxDirPath.TabIndex = 3;
            this.textBoxDirPath.Text = "先选择目录，然后生成。";
            this.textBoxDirPath.DragDrop += new System.Windows.Forms.DragEventHandler(this.textBoxDirPath_DragDrop);
            this.textBoxDirPath.DragEnter += new System.Windows.Forms.DragEventHandler(this.textBoxDirPath_DragEnter);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 17F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(352, 179);
            this.Controls.Add(this.textBoxDirPath);
            this.Controls.Add(this.buttonGenerate);
            this.Controls.Add(this.buttonChoose);
            this.Controls.Add(this.textBox1);
            this.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "删除Unity Manifest文件";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button buttonChoose;
        private System.Windows.Forms.Button buttonGenerate;
        private System.Windows.Forms.TextBox textBoxDirPath;
    }
}

