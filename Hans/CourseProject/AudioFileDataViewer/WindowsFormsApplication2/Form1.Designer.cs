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
            this.dataDirLabel = new System.Windows.Forms.Label();
            this.loadDataButton = new System.Windows.Forms.Button();
            this.compareButton = new System.Windows.Forms.Button();
            this.searchFileListBox1 = new System.Windows.Forms.ListBox();
            this.labelSearchFile = new System.Windows.Forms.Label();
            this.highlightSimilarityCheckBox = new System.Windows.Forms.CheckBox();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.searchFileListBox2 = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            this.spectrogramButton = new System.Windows.Forms.Button();
            this.maximaButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(13, 13);
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
            // dataDirLabel
            // 
            this.dataDirLabel.AutoSize = true;
            this.dataDirLabel.Location = new System.Drawing.Point(13, 1202);
            this.dataDirLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.dataDirLabel.Name = "dataDirLabel";
            this.dataDirLabel.Size = new System.Drawing.Size(155, 25);
            this.dataDirLabel.TabIndex = 7;
            this.dataDirLabel.Text = "Data Directory:";
            // 
            // loadDataButton
            // 
            this.loadDataButton.Location = new System.Drawing.Point(1353, 1189);
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
            this.compareButton.Location = new System.Drawing.Point(1353, 1123);
            this.compareButton.Name = "compareButton";
            this.compareButton.Size = new System.Drawing.Size(209, 50);
            this.compareButton.TabIndex = 16;
            this.compareButton.Text = "Compare";
            this.compareButton.UseVisualStyleBackColor = true;
            this.compareButton.Visible = false;
            this.compareButton.Click += new System.EventHandler(this.compareButton_Click);
            // 
            // searchFileListBox1
            // 
            this.searchFileListBox1.FormattingEnabled = true;
            this.searchFileListBox1.ItemHeight = 25;
            this.searchFileListBox1.Location = new System.Drawing.Point(1169, 562);
            this.searchFileListBox1.Name = "searchFileListBox1";
            this.searchFileListBox1.Size = new System.Drawing.Size(393, 204);
            this.searchFileListBox1.TabIndex = 17;
            this.searchFileListBox1.Visible = false;
            // 
            // labelSearchFile
            // 
            this.labelSearchFile.AutoSize = true;
            this.labelSearchFile.Location = new System.Drawing.Point(1164, 534);
            this.labelSearchFile.Name = "labelSearchFile";
            this.labelSearchFile.Size = new System.Drawing.Size(71, 25);
            this.labelSearchFile.TabIndex = 19;
            this.labelSearchFile.Text = "File 1:";
            this.labelSearchFile.Visible = false;
            // 
            // highlightSimilarityCheckBox
            // 
            this.highlightSimilarityCheckBox.AutoSize = true;
            this.highlightSimilarityCheckBox.Location = new System.Drawing.Point(1190, 1135);
            this.highlightSimilarityCheckBox.Name = "highlightSimilarityCheckBox";
            this.highlightSimilarityCheckBox.Size = new System.Drawing.Size(128, 29);
            this.highlightSimilarityCheckBox.TabIndex = 32;
            this.highlightSimilarityCheckBox.Text = "Highlight";
            this.highlightSimilarityCheckBox.UseVisualStyleBackColor = true;
            this.highlightSimilarityCheckBox.Visible = false;
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(18, 1135);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(1139, 29);
            this.progressBar1.TabIndex = 33;
            this.progressBar1.Visible = false;
            // 
            // searchFileListBox2
            // 
            this.searchFileListBox2.FormattingEnabled = true;
            this.searchFileListBox2.ItemHeight = 25;
            this.searchFileListBox2.Location = new System.Drawing.Point(1169, 901);
            this.searchFileListBox2.Name = "searchFileListBox2";
            this.searchFileListBox2.Size = new System.Drawing.Size(393, 204);
            this.searchFileListBox2.TabIndex = 34;
            this.searchFileListBox2.Visible = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(1164, 873);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(71, 25);
            this.label1.TabIndex = 35;
            this.label1.Text = "File 2:";
            this.label1.Visible = false;
            // 
            // spectrogramButton
            // 
            this.spectrogramButton.Location = new System.Drawing.Point(1169, 772);
            this.spectrogramButton.Name = "spectrogramButton";
            this.spectrogramButton.Size = new System.Drawing.Size(193, 49);
            this.spectrogramButton.TabIndex = 36;
            this.spectrogramButton.Text = "Spectrogram";
            this.spectrogramButton.UseVisualStyleBackColor = true;
            this.spectrogramButton.Visible = false;
            this.spectrogramButton.Click += new System.EventHandler(this.spectrogramButton_Click);
            // 
            // maximaButton
            // 
            this.maximaButton.Location = new System.Drawing.Point(1368, 772);
            this.maximaButton.Name = "maximaButton";
            this.maximaButton.Size = new System.Drawing.Size(194, 49);
            this.maximaButton.TabIndex = 37;
            this.maximaButton.Text = "Local Maxima";
            this.maximaButton.UseVisualStyleBackColor = true;
            this.maximaButton.Visible = false;
            this.maximaButton.Click += new System.EventHandler(this.maximaButton_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(1574, 1250);
            this.Controls.Add(this.maximaButton);
            this.Controls.Add(this.spectrogramButton);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.searchFileListBox2);
            this.Controls.Add(this.searchFileListBox1);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.highlightSimilarityCheckBox);
            this.Controls.Add(this.labelSearchFile);
            this.Controls.Add(this.compareButton);
            this.Controls.Add(this.loadDataButton);
            this.Controls.Add(this.dataDirLabel);
            this.Controls.Add(this.dataDirectoryTextBox);
            this.Controls.Add(this.pictureBox1);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Form1";
            this.Text = "Audio Comparing Application - SUTD Algorithms Course";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.TextBox dataDirectoryTextBox;
        private System.Windows.Forms.Label dataDirLabel;
        private System.Windows.Forms.Button loadDataButton;
        private System.Windows.Forms.Button compareButton;
        private System.Windows.Forms.ListBox searchFileListBox1;
        private System.Windows.Forms.Label labelSearchFile;
        private System.Windows.Forms.CheckBox highlightSimilarityCheckBox;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.ListBox searchFileListBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button spectrogramButton;
        private System.Windows.Forms.Button maximaButton;
    }
}

