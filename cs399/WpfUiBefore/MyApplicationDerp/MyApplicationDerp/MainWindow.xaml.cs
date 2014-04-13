using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace MyApplicationDerp
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        Character MyCharacter;

        public MainWindow()
        {
            InitializeComponent();

            MyCharacter = new Character()
            {
                Name = "Richard III",
                Gold = 10,
                //Experience = 100,
                //Race = Race.Human,
                //Residence = "Stormwind",
            };

            this.NameTextBox.Text = MyCharacter.Name;
            this.GoldTextBox.Text = MyCharacter.Gold.ToString();

            this.NameTextBox.TextChanged += new TextChangedEventHandler(NameTextBox_TextChanged);
            this.GoldTextBox.TextChanged += new TextChangedEventHandler(GoldTextBox_TextChanged);

            MyCharacter.PropertyChanged += new System.ComponentModel.PropertyChangedEventHandler(MyCharacter_PropertyChanged);
        }

        void MyCharacter_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Name")
            {
                if (this.NameTextBox.IsFocused == false)
                    this.NameTextBox.Text = MyCharacter.Name;
            }

            if (e.PropertyName == "Gold")
            {
                if (this.GoldTextBox.IsFocused == false)
                    this.GoldTextBox.Text = MyCharacter.Gold.ToString();
            }
        }

        void GoldTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            MyCharacter.Gold = Int32.Parse(this.GoldTextBox.Text);
        }

        void NameTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            MyCharacter.Name = this.NameTextBox.Text;
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            StringBuilder builder = new StringBuilder();
            builder.AppendLine("Name: " + MyCharacter.Name);
            builder.AppendLine("Gold: " + MyCharacter.Gold);
            this.DebugText.Text = builder.ToString();
        }

        private void GiveGoldButton_Click(object sender, RoutedEventArgs e)
        {
            MyCharacter.Gold += 100;
        }
    }
}
