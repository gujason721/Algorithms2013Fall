using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Media;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication2
{
    public partial class Form1 : Form
    {
        private List<AudioSampleData> audioSamples;
        private AudioSampleData lastSearchSound;

        public static int generateHash(int freq1, int timeDistance, int freqDistance) { return freq1 + (timeDistance << 12) + (freqDistance << 24); }

        public Form1()
        {
            InitializeComponent();
            audioSamples = new List<AudioSampleData>();
        }

        private void loadData()
        {
            audioSamples.Clear();
            searchFileListBox.Items.Clear();

            // load all files with the correct extension from the specified directory
            DirectoryInfo dataDir = new DirectoryInfo(dataDirectoryTextBox.Text);
            var fileEnumerator = dataDir.EnumerateFiles(@"*.maxima");

            progressBar1.Visible = true;
            progressBar1.Step = 1;
            progressBar1.Maximum = fileEnumerator.Count();

            foreach (FileInfo file in fileEnumerator)
            {
                AudioSampleData newSample = new AudioSampleData();
                loadCSV(file.FullName, ref newSample.maxima);
                loadCSV(file.FullName.Replace(@".maxima",@".spectrogram"), ref newSample.spectrogram);
                newSample.filename = file.Name;
                newSample.sound = new SoundPlayer(file.Directory.Parent.FullName + @"\" + file.Name.Replace(@".maxima",@".wav"));
                newSample.populateHashTable();
                audioSamples.Add(newSample);
                searchFileListBox.Items.Add(file.Name);
                progressBar1.PerformStep();
            }

            progressBar1.Visible = false;

            //displayData(audioSamples[0].maxima);
            if (audioSamples.Count > 0)
            {
                searchFileListBox.Visible = compareButton.Visible = labelSearchFile.Visible = highlightSimilarityCheckBox.Visible = true;
                searchFileListBox.SelectedIndex = 0;
            }
        }



        private void loadCSV(String filename, ref float[,] matrix)
        {
            StreamReader r = new StreamReader(File.OpenRead(filename));

            List<List<float>> csvData = new List<List<float>>();

            // read into a dynamic data structure
            while (!r.EndOfStream)
            {
                String line = r.ReadLine();
                String[] valuesAsText = line.Split(',');
                List<float> valuesAsFloat = new List<float>();
                foreach (String v in valuesAsText) valuesAsFloat.Add(float.Parse(v));
                csvData.Add(valuesAsFloat);
            }

            // copy to a multidimensional array
            matrix = new float[csvData.Count(), csvData[0].Count()];
            int i = 0;
            foreach (List<float> row in csvData)
            {
                int j = 0;
                foreach (float element in row)
                {
                    matrix[i, j++] = element;
                }
                i++;
            }
        }

        private void displayData(float[,] matrix, PictureBox pb)
        {
            Bitmap matrixImage = matrixToImage(matrix);
            pb.Image = matrixImage;
            pb.Height = matrixImage.Height;
            pb.Width = matrixImage.Width;
            pb.Visible = true;
        }

        public class AudioSampleData
        {
            public Dictionary<int, List<int>> fingerprintTable;  // first int is the hash, second int is the time
            public String filename;
            public SoundPlayer sound;
            public bool playing;
            public float[,] similarityMatrix;
            public float similarityScore;
            public float[,] spectrogram;
            public float[,] maxima;
            public int length() { return spectrogram.GetLength(1); }

            public void play()
            {
                if (!playing) { sound.Play(); playing = true; }
                else { sound.Stop(); playing = false; }
            }

            public void populateHashTable()
            {
                // set dimensions of the linking area
                int minHDistance = 4;  // the minimum horizontal distance between two linked keys
                int maxHDistance = 10; // the right boundary of the linking zone
                int maxVDistance = 20; // this is how many bins above or below the first key we can go when searching for the second key

                reduceRangesToMaxPoints();

                //fingerprintList = new List<AudioFingerprint>();
                fingerprintTable = new Dictionary<int, List<int>>();

                // for each column
                for (int j = 0; j < maxima.GetLength(1); j++)
                {
                    // for each element in the column
                    for (int i = 0; i < maxima.GetLength(0); i++)
                    {
                        if (maxima[i, j] != 0)
                        {
                            // construct boundaries of the linking region
                            int lrStart = j + minHDistance;
                            int lrEnd = min(j + maxHDistance, maxima.GetLength(1) - 1);
                            int lrTop = max(i - maxVDistance, 0);
                            int lrBottom = min(i + maxVDistance, maxima.GetLength(0) - 1);

                            // for each row in the linking region (we're going in row-major order to save time)
                            for (int ilr = lrTop; ilr <= lrBottom; ilr++)
                            {
                                // for each element in the row
                                for (int jlr = lrStart; jlr <= lrEnd; jlr++)
                                {
                                    if (maxima[ilr, jlr] != 0)
                                    {
                                        int timeDifference = jlr - j; // the horizontal distance between the point and it's partner in the linking region
                                        int frequencyDifference = (int)ilr - (int)i;
                                        int hashValue = generateHash((int)i, (int)timeDifference, (int)frequencyDifference);
                                       
                                        if (!fingerprintTable.ContainsKey(hashValue)) {// if this hash value has not been added yet, add it
                                            List<int> newList = new List<int>();
                                            newList.Add(j);
                                            fingerprintTable.Add(hashValue,newList);
                                        }
                                        else // the hash value is not unique so add it to the existing list
                                        {
                                            fingerprintTable[hashValue].Add(j);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // within each non-zero region of 'maxima', find the max value in the corresponding region of 'spectrogram'
            public void reduceRangesToMaxPoints()
            {
                // for each column
                for (int j = 0; j < spectrogram.GetLength(1); j++)
                {

                    Boolean inNZRange = false;
                    float localMax = float.MinValue;
                    int localMaxIndex = int.MinValue;
                    // for each element in column j
                    for (int i = 0; i < spectrogram.GetLength(0); i++)
                    {
                        // if it's not zero initiate or continue the search for the maximum in this non-zero region
                        if (maxima[i, j] != 0)
                        {
                            maxima[i, j] = 0; // clear the value
                            float specGramValue = spectrogram[i, j];
                            if (!inNZRange || specGramValue > localMax)
                            {
                                inNZRange = true;
                                localMax = specGramValue;
                                localMaxIndex = i;
                            }
                        }
                        else if (inNZRange) // "else" means the value was already zero; we've found the end of the nonzero range
                        {
                            maxima[localMaxIndex, j] = 1;
                            inNZRange = false;
                            localMax = float.MinValue;
                            localMaxIndex = int.MinValue;
                        }
                    }
                    if (inNZRange) // if the last element was non-zero then we have yet to terminate the search on the last non-zero Range
                    {
                        maxima[localMaxIndex, j] = 1;
                        inNZRange = false;
                    }
                }
            }
        }

        // produce a plot comparing the times at which corresponding fingerprints occured in 
        // each of the two audio samples x and y
        private void similarity(ref AudioSampleData x, ref AudioSampleData y, bool highlightSimilarity)
        {
            x.similarityMatrix = new float[y.length(), x.length()];
            float [] similarityScoreByTime = new float [x.length()];
            x.similarityScore = 0;

            // initialize the matrix to zero
            for (int r = 0; r < y.length(); ++r)
                for (int c = 0; c < x.length(); ++c)
                    x.similarityMatrix[r, c] = 0;

            // initialize the scores to zero for each time
            for (int i = 0; i < x.length(); i++) similarityScoreByTime[i] = 0;

            // some files have a higher density of keys.  We compute the density to help us compensate for this.
            float xKeyDensity = (float)x.fingerprintTable.Count / ((float)x.length());
            float yKeyDensity = (float)y.fingerprintTable.Count / ((float)y.length());
            float totalDensity = xKeyDensity * yKeyDensity; 
            float perKeyWeight = 500000.0f / (float)Math.Pow(totalDensity,2.0);
            float matrixArea = (float)x.length() * (float)y.length();

            // set a 1 for each key that the two files have in common.
            foreach (int xKey in x.fingerprintTable.Keys)
            {
                if (y.fingerprintTable.ContainsKey(xKey))
                {
                    foreach (int xTimeIdx in x.fingerprintTable[xKey])
                        foreach (int yTimeIdx in y.fingerprintTable[xKey])
                        {
                            x.similarityMatrix[y.length() - yTimeIdx - 1, xTimeIdx] += perKeyWeight;
                            if (x.similarityMatrix[y.length() - yTimeIdx - 1, xTimeIdx] > 1) x.similarityMatrix[y.length() - yTimeIdx - 1, xTimeIdx] = 1;
                            int similarityTime = xTimeIdx - yTimeIdx; // the beginning of the diagonal strip of common keys is where teh diagonal line meets y=0
                            if (similarityTime >= 0 && similarityTime < similarityScoreByTime.Length)
                                similarityScoreByTime[similarityTime] += perKeyWeight;
                        }
                }
            }
            int maxSimilarityIndex = maxIndex(similarityScoreByTime);
            x.similarityScore = 50 * similarityScoreByTime[maxSimilarityIndex] / min(y.length(),x.length());
            x.similarityScore = min(x.similarityScore, 100.0f);
            
            if (highlightSimilarity && x.similarityScore > 14)
            {
                int highlightWidth = 5;

                // draw diagonal lines
                for (int i = 0; i < y.length(); i++)
                {
                    int leftSide = maxSimilarityIndex + i - highlightWidth;
                    int rightSide = maxSimilarityIndex + i + highlightWidth;
                    if(leftSide >= 0 && leftSide < x.length()) x.similarityMatrix[y.length() - i -1, leftSide] = float.MinValue;
                    if(rightSide >= 0 && rightSide < x.length()) x.similarityMatrix[y.length() - i -1, rightSide] = float.MinValue;
                }

                // draw top and bottom lines
                for (int i = 0; i < highlightWidth * 2 + 1; i++) {
                    // bottom
                    int j = maxSimilarityIndex - highlightWidth + i;
                    if (j >= 0 && j <x.length()) x.similarityMatrix[y.length() - 1, j] = float.MinValue;

                    // top
                    j = maxSimilarityIndex - highlightWidth + i + y.length() - 1;
                    if (j >= 0 && j < x.length()) x.similarityMatrix[0, j] = float.MinValue;
                }
            }
        }

        private static int maxIndex(float[] a)
        {
            float max = float.MinValue;
            int maxIndex = int.MinValue;
            for (int i = 0; i < a.Length; i++)
                if (a[i] > max)
                {
                    max = a[i];
                    maxIndex = i;
                }
            return maxIndex;
        }

        private Bitmap matrixToImage(float[,] mat)
        {
            int rows = mat.GetLength(0);
            int columns = mat.GetLength(1);

            Bitmap bmp = new Bitmap(columns, rows);

            for (int r = 0; r < rows; ++r)
                for (int c = 0; c < columns; ++c)
                    bmp.SetPixel(c, r, makeMatrixColor(mat[r, c]));

            return bmp;
        }

        private class Range
        {
            public int begin, end;
            public Range(int b, int e)
            {
                begin = b;
                end = e;
            }
        }

        private Color makeMatrixColor(float n)
        {
            // We want 
            //  0 = black
            //  0 < n < 0.25 = dark blue
            //  0 < 0.25 = orange
            //  float.MinValue = bright blue

            if (n == float.MinValue) return Color.FromArgb(160, 160, 255);

            // These formulae define the colour when n is near 1
            int redOr = (int)(255.0 * (Math.Pow(n, 0.25)));
            int greenOr = (int)(160.0 * (Math.Pow(n, 0.25)));
            int blueOr = 0;

            // These formula define the colour when n is near 0
            int redBl = (int)(25.0 * (Math.Pow(n, 0.25))); ;
            int greenBL = (int)(25.0 * (Math.Pow(n, 0.25))); ;
            int blueBl = (int)(255.0 * Math.Pow(n, 0.25));

            // We interpolate between the orange and blue colorings
            int red = lerp(redBl,redOr,Math.Pow(n,0.5));
            int green = lerp(greenBL,greenOr, Math.Pow(n,0.5));
            int blue = lerp(blueBl,blueOr, Math.Pow(n,0.5));

            return Color.FromArgb(red, green, blue);
        }

        private static int lerp(int a, int b, double n) { return (int)((float)b * n + (float)a * (1.0 - n)); }
        private static int min(int a, int b) { return a < b ? a : b; }
        private static int max(int a, int b) { return a > b ? a : b; }
        private static float max(float a, float b) { return a > b ? a : b; }
        private static float min(float a, float b) { return a < b ? a : b; }

        private void loadDataButtonClick(object sender, EventArgs e)
        {
            loadData();
        }

        private void compareButton_Click(object sender, EventArgs e)
        {
            // if the search file list box is hidden we use the previous selection
            AudioSampleData searchSound;
            if (searchFileListBox.Visible)
            {
                searchSound = audioSamples[searchFileListBox.SelectedIndex];
                lastSearchSound = searchSound;
            }
            else
            {
                searchSound = lastSearchSound;
            }

            PictureBox [] pictureBoxen = { pictureBox1, pictureBox2, pictureBox3, pictureBox4, pictureBox5, pictureBox6, pictureBox7 };
            Label [] pictureLabels = { scoreLabel1, scoreLabel2, scoreLabel3, scoreLabel4, scoreLabel5, scoreLabel6, scoreLabel7 };


            // find the similarity between the search term and all the sounds in the database
            for (int i = 0; i < audioSamples.Count; i++)
            {
                AudioSampleData asd = audioSamples[i];
                similarity(ref asd, ref searchSound, highlightSimilarityCheckBox.Checked);
            }

            // sort the audio samples by similarity to the search term
            audioSamples.Sort(delegate(AudioSampleData x, AudioSampleData y) { return -x.similarityScore.CompareTo(y.similarityScore); });

            // display the similarity graph results
            for (int i = 0; i < audioSamples.Count && i < pictureBoxen.Length;i++ )
            {
                displayData(audioSamples[i].similarityMatrix, pictureBoxen[i]);
                pictureLabels[i].Text = audioSamples[i].filename + @", Similarity Score: " + ((int)audioSamples[i].similarityScore).ToString();
                pictureLabels[i].Visible = true;
            }

            labelSearchFile.Visible = searchFileListBox.Visible = false;
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            audioSamples[0].play();
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            audioSamples[1].play();
        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {
            audioSamples[2].play();
        }

        private void pictureBox4_Click(object sender, EventArgs e)
        {
            audioSamples[3].play();
        }

        private void pictureBox5_Click(object sender, EventArgs e)
        {
            audioSamples[4].play();
        }

        private void pictureBox6_Click(object sender, EventArgs e)
        {
            audioSamples[5].play();
        }

        private void pictureBox7_Click(object sender, EventArgs e)
        {
            audioSamples[6].play();
        }
    }
}
