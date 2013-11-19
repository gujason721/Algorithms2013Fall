namespace WindowsFormsApplication2
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.dataDirectoryTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.loadDataButton = new System.Windows.Forms.Button();
            this.compareButton = new System.Windows.Forms.Button();
            this.searchFileListBox = new System.Windows.Forms.ListBox();
            this.labelSearchFile = new System.Windows.Forms.Label();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            this.scoreLabel1 = new System.Windows.Forms.Label();
            this.scoreLabel2 = new System.Windows.Forms.Label();
            this.scoreLabel3 = new System.Windows.Forms.Label();
            this.scoreLabel4 = new System.Windows.Forms.Label();
            this.pictureBox5 = new System.Windows.Forms.PictureBox();
            this.scoreLabel5 = new System.Windows.Forms.Label();
            this.pictureBox6 = new System.Windows.Forms.PictureBox();
            this.scoreLabel6 = new System.Windows.Forms.Label();
            this.highlightSimilarityCheckBox = new System.Windows.Forms.CheckBox();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.pictureBox7 = new System.Windows.Forms.PictureBox();
            this.scoreLabel7 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox5)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox6)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox7)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(18, 59);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(4);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(899, 86);
            this.pictureBox1.TabIndex = 2;
            this.pictureBox1.TabStop = false;
            this.pictureBox1.Click += new System.EventHandler(this.pictureBox1_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // dataDirectoryTextBox
            // 
            this.dataDirectoryTextBox.Location = new System.Drawing.Point(176, 1199);
            this.dataDirectoryTextBox.Margin = new System.Windows.Forms.Padding(4);
            this.dataDirectoryTextBox.Name = "dataDirectoryTextBox";
            this.dataDirectoryTextBox.Size = new System.Drawing.Size(741, 31);
            this.dataDirectoryTextBox.TabIndex = 3;
            this.dataDirectoryTextBox.Text = "\\\\psf\\Home\\Desktop\\audio fingerprint\\prints\\";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 1202);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(155, 25);
            this.label2.TabIndex = 7;
            this.label2.Text = "Data Directory:";
            // 
            // loadDataButton
            // 
            this.loadDataButton.Location = new System.Drawing.Point(1120, 1189);
            this.loadDataButton.Margin = new System.Windows.Forms.Padding(4);
            this.loadDataButton.Name = "loadDataButton";
            this.loadDataButton.Size = new System.Drawing.Size(209, 50);
            this.loadDataButton.TabIndex = 10;
            this.loadDataButton.Text = "Load Data";
            this.loadDataButton.UseVisualStyleBackColor = true;
            this.loadDataButton.Click += new System.EventHandler(this.loadDataButtonClick);
            // 
            // compareButton
            // 
            this.compareButton.Location = new System.Drawing.Point(1120, 1123);
            this.compareButton.Name = "compareButton";
            this.compareButton.Size = new System.Drawing.Size(209, 50);
            this.compareButton.TabIndex = 16;
            this.compareButton.Text = "Compare";
            this.compareButton.UseVisualStyleBackColor = true;
            this.compareButton.Visible = false;
            this.compareButton.Click += new System.EventHandler(this.compareButton_Click);
            // 
            // searchFileListBox
            // 
            this.searchFileListBox.FormattingEnabled = true;
            this.searchFileListBox.ItemHeight = 25;
            this.searchFileListBox.Location = new System.Drawing.Point(936, 901);
            this.searchFileListBox.Name = "searchFileListBox";
            this.searchFileListBox.Size = new System.Drawing.Size(393, 204);
            this.searchFileListBox.TabIndex = 17;
            this.searchFileListBox.Visible = false;
            // 
            // labelSearchFile
            // 
            this.labelSearchFile.AutoSize = true;
            this.labelSearchFile.Location = new System.Drawing.Point(931, 872);
            this.labelSearchFile.Name = "labelSearchFile";
            this.labelSearchFile.Size = new System.Drawing.Size(127, 25);
            this.labelSearchFile.TabIndex = 19;
            this.labelSearchFile.Text = "Search File:";
            this.labelSearchFile.Visible = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Location = new System.Drawing.Point(18, 210);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(899, 90);
            this.pictureBox2.TabIndex = 21;
            this.pictureBox2.TabStop = false;
            this.pictureBox2.Click += new System.EventHandler(this.pictureBox2_Click);
            // 
            // pictureBox3
            // 
            this.pictureBox3.Location = new System.Drawing.Point(18, 361);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(899, 88);
            this.pictureBox3.TabIndex = 22;
            this.pictureBox3.TabStop = false;
            this.pictureBox3.Click += new System.EventHandler(this.pictureBox3_Click);
            // 
            // pictureBox4
            // 
            this.pictureBox4.Location = new System.Drawing.Point(18, 510);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(899, 94);
            this.pictureBox4.TabIndex = 23;
            this.pictureBox4.TabStop = false;
            this.pictureBox4.Visible = false;
            this.pictureBox4.Click += new System.EventHandler(this.pictureBox4_Click);
            // 
            // scoreLabel1
            // 
            this.scoreLabel1.AutoSize = true;
            this.scoreLabel1.Location = new System.Drawing.Point(13, 30);
            this.scoreLabel1.Name = "scoreLabel1";
            this.scoreLabel1.Size = new System.Drawing.Size(83, 25);
            this.scoreLabel1.TabIndex = 24;
            this.scoreLabel1.Text = "score 1";
            this.scoreLabel1.Visible = false;
            // 
            // scoreLabel2
            // 
            this.scoreLabel2.AutoSize = true;
            this.scoreLabel2.Location = new System.Drawing.Point(13, 182);
            this.scoreLabel2.Name = "scoreLabel2";
            this.scoreLabel2.Size = new System.Drawing.Size(70, 25);
            this.scoreLabel2.TabIndex = 25;
            this.scoreLabel2.Text = "label3";
            this.scoreLabel2.Visible = false;
            // 
            // scoreLabel3
            // 
            this.scoreLabel3.AutoSize = true;
            this.scoreLabel3.Location = new System.Drawing.Point(12, 333);
            this.scoreLabel3.Name = "scoreLabel3";
            this.scoreLabel3.Size = new System.Drawing.Size(70, 25);
            this.scoreLabel3.TabIndex = 26;
            this.scoreLabel3.Text = "label4";
            this.scoreLabel3.Visible = false;
            // 
            // scoreLabel4
            // 
            this.scoreLabel4.AutoSize = true;
            this.scoreLabel4.Location = new System.Drawing.Point(13, 482);
            this.scoreLabel4.Name = "scoreLabel4";
            this.scoreLabel4.Size = new System.Drawing.Size(70, 25);
            this.scoreLabel4.TabIndex = 27;
            this.scoreLabel4.Text = "label5";
            this.scoreLabel4.Visible = false;
            // 
            // pictureBox5
            // 
            this.pictureBox5.Location = new System.Drawing.Point(18, 664);
            this.pictureBox5.Name = "pictureBox5";
            this.pictureBox5.Size = new System.Drawing.Size(899, 91);
            this.pictureBox5.TabIndex = 28;
            this.pictureBox5.TabStop = false;
            this.pictureBox5.Visible = false;
            this.pictureBox5.Click += new System.EventHandler(this.pictureBox5_Click);
            // 
            // scoreLabel5
            // 
            this.scoreLabel5.AutoSize = true;
            this.scoreLabel5.Location = new System.Drawing.Point(12, 636);
            this.scoreLabel5.Name = "scoreLabel5";
            this.scoreLabel5.Size = new System.Drawing.Size(70, 25);
            this.scoreLabel5.TabIndex = 29;
            this.scoreLabel5.Text = "label1";
            this.scoreLabel5.Visible = false;
            // 
            // pictureBox6
            // 
            this.pictureBox6.Location = new System.Drawing.Point(18, 810);
            this.pictureBox6.Name = "pictureBox6";
            this.pictureBox6.Size = new System.Drawing.Size(899, 87);
            this.pictureBox6.TabIndex = 30;
            this.pictureBox6.TabStop = false;
            this.pictureBox6.Visible = false;
            this.pictureBox6.Click += new System.EventHandler(this.pictureBox6_Click);
            // 
            // scoreLabel6
            // 
            this.scoreLabel6.AutoSize = true;
            this.scoreLabel6.Location = new System.Drawing.Point(13, 782);
            this.scoreLabel6.Name = "scoreLabel6";
            this.scoreLabel6.Size = new System.Drawing.Size(70, 25);
            this.scoreLabel6.TabIndex = 31;
            this.scoreLabel6.Text = "label3";
            this.scoreLabel6.Visible = false;
            // 
            // highlightSimilarityCheckBox
            // 
            this.highlightSimilarityCheckBox.AutoSize = true;
            this.highlightSimilarityCheckBox.Location = new System.Drawing.Point(957, 1135);
            this.highlightSimilarityCheckBox.Name = "highlightSimilarityCheckBox";
            this.highlightSimilarityCheckBox.Size = new System.Drawing.Size(128, 29);
            this.highlightSimilarityCheckBox.TabIndex = 32;
            this.highlightSimilarityCheckBox.Text = "Highlight";
            this.highlightSimilarityCheckBox.UseVisualStyleBackColor = true;
            this.highlightSimilarityCheckBox.Visible = false;
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(17, 1076);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(1312, 29);
            this.progressBar1.TabIndex = 33;
            this.progressBar1.Visible = false;
            // 
            // pictureBox7
            // 
            this.pictureBox7.Location = new System.Drawing.Point(18, 983);
            this.pictureBox7.Name = "pictureBox7";
            this.pictureBox7.Size = new System.Drawing.Size(899, 87);
            this.pictureBox7.TabIndex = 34;
            this.pictureBox7.TabStop = false;
            this.pictureBox7.Visible = false;
            this.pictureBox7.Click += new System.EventHandler(this.pictureBox7_Click);
            // 
            // scoreLabel7
            // 
            this.scoreLabel7.AutoSize = true;
            this.scoreLabel7.Location = new System.Drawing.Point(13, 955);
            this.scoreLabel7.Name = "scoreLabel7";
            this.scoreLabel7.Size = new System.Drawing.Size(70, 25);
            this.scoreLabel7.TabIndex = 35;
            this.scoreLabel7.Text = "label3";
            this.scoreLabel7.Visible = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(1344, 1250);
            this.Controls.Add(this.searchFileListBox);
            this.Controls.Add(this.scoreLabel7);
            this.Controls.Add(this.pictureBox7);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.highlightSimilarityCheckBox);
            this.Controls.Add(this.scoreLabel6);
            this.Controls.Add(this.pictureBox6);
            this.Controls.Add(this.scoreLabel5);
            this.Controls.Add(this.pictureBox5);
            this.Controls.Add(this.scoreLabel4);
            this.Controls.Add(this.scoreLabel3);
            this.Controls.Add(this.scoreLabel2);
            this.Controls.Add(this.scoreLabel1);
            this.Controls.Add(this.pictureBox4);
            this.Controls.Add(this.pictureBox3);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.labelSearchFile);
            this.Controls.Add(this.compareButton);
            this.Controls.Add(this.loadDataButton);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.dataDirectoryTextBox);
            this.Controls.Add(this.pictureBox1);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Form1";
            this.Text = "Audio Comparing Application - SUTD Algorithms Course";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox5)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox6)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox7)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.TextBox dataDirectoryTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button loadDataButton;
        private System.Windows.Forms.Button compareButton;
        private System.Windows.Forms.ListBox searchFileListBox;
        private System.Windows.Forms.Label labelSearchFile;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.PictureBox pictureBox4;
        private System.Windows.Forms.Label scoreLabel1;
        private System.Windows.Forms.Label scoreLabel2;
        private System.Windows.Forms.Label scoreLabel3;
        private System.Windows.Forms.Label scoreLabel4;
        private System.Windows.Forms.PictureBox pictureBox5;
        private System.Windows.Forms.Label scoreLabel5;
        private System.Windows.Forms.PictureBox pictureBox6;
        private System.Windows.Forms.Label scoreLabel6;
        private System.Windows.Forms.CheckBox highlightSimilarityCheckBox;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.PictureBox pictureBox7;
        private System.Windows.Forms.Label scoreLabel7;
    }
}

